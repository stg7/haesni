/**
    Copyright 2015 Steve Göring

    haesni

    \author stg7

    \brief haesni is a aes based hash function for performance increasing of hash-table applications

    This implementation should be as fast as boost::hash_range for strings

    In this file is a aes based hash function implemented,
    using aes-ni (an intel instruction extension for aes) and sse to speedup
    hash calculation and reducing hash-collisions.

    The main idea is the Damgård-Merkle-construction for hash functions:
    e.g. see https://en.wikipedia.org/wiki/Merkle%E2%80%93Damg%C3%A5rd_construction and
             https://en.wikipedia.org/wiki/Cryptographic_hash_function


    \date 15.11.2015
**/
/**
    This file is part of haesni.

    haesni is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haesni is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haesni.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef haesni_HEADER_HPP_
#define haesni_HEADER_HPP_

#include <immintrin.h>
#include <wmmintrin.h>

namespace haesni {

    const int blocksize = 16;  //< blocksize in bytes
    const int blockbits = 4;  //< precalculated blockbits = log_2(blocksize)


    /**
    *   compressAES: compress Message M with Key K via 2-rounds of AES
    *
    *   \param M Message to encrypt
    *   \param K Key for encryption
    *
    *   \return "aes" encrypted message
    **/
    inline __m128i compressAES(__m128i* M, const __m128i& K ) {
        __m128i m = _mm_load_si128(M);  //< load message
        m = _mm_xor_si128(m, K);  //< xor msg with key
        m = _mm_aesenc_si128(m, K);  //< do one aes encoding round
        m = _mm_aesenclast_si128(m, K);  //< do last aes encoding round
        return m;
    }

    /**
    *   hash: hash string "s" with length "len" based on DM-Construction via AES as blockchiffre
    *
    *   \param s string to hash
    *   \param len length of string, !carefully no check if string-length is lower than len!
    *
    *   \return 64 bit hash of string "s" with length "len"
    **/
    inline unsigned long hash(char* s, const unsigned long len) {
        __m128i H = _mm_setzero_si128();  //< initial value  for hash, set to 00000
        __m128i* V = reinterpret_cast<__m128i *>(s);  //< transform input string to m128i array

        long handled_len = 0;

        // handle 2 * 16 byte blocks of string s
        for (unsigned long i = 0; i < len >> (blockbits + 1); i++) {
            // compress 16 byte block values
            // H_i = f(s[block i], H_{i-1}); DM Construction, compress function  f = AES(..)

            H = compressAES(V, H);
            V++;

            // next 16 byte block
            H = compressAES(V, H);
            V++;
            handled_len += 2 * blocksize;
        }

        // if there is a not handled remaining 16 byte block
        if (len - handled_len >= blocksize) {
            // next 16 byte block
            H = compressAES(V, H);

            V++;
            handled_len += blocksize;
        }

        // compress remaining (less than 16 byte) part of string
        if (handled_len != len) {
            s += handled_len;

            int rest = len - handled_len;  //< remaining length of string, should be 1<= rest <= 15

            __m128i t;
            // memcpy is too slow, better call mm_set with explicit chars
            switch (rest) {
                case 1:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s[0]); break; }
                case 2:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s[1], s[0]); break;}
                case 3:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s[2], s[1], s[0]); break;}
                case 4:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s[3], s[2], s[1], s[0]); break;}
                case 5:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s[4], s[3], s[2], s[1], s[0]); break;}
                case 6:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 7:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 8:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 9:  {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, s[8], s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 10: {t = _mm_set_epi8(0, 0, 0, 0, 0, 0, s[9], s[8], s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 11: {t = _mm_set_epi8(0, 0, 0, 0, 0, s[10], s[9], s[8], s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 12: {t = _mm_set_epi8(0, 0, 0, 0, s[11], s[10], s[9], s[8], s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 13: {t = _mm_set_epi8(0, 0, 0, s[12], s[11], s[10], s[9], s[8], s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 14: {t = _mm_set_epi8(0, 0, s[13], s[12], s[11], s[10], s[9], s[8], s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
                case 15: {t = _mm_set_epi8(0, s[14], s[13], s[12], s[11], s[10], s[9], s[8], s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]); break;}
            }

            H = compressAES(&t, H);
        }

        // build unsigned long hash value based on 16 byte aes hash with xor-ing of high and low part of H
        return _mm_extract_epi64(H, 0) ^ _mm_extract_epi64(H, 1);  // H_L ^ H_H
    }
}

#endif
