/**
    Copyright 2015 Steve Göring

    haesni example programm

    \author stg7

    \brief small demonstation of haesni usage

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

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "haesni.hpp"

int main(int argc, const char* argv[]) {
    std::cout << "small demo for haesni" << std::endl;

    std::vector<std::string> hash_strings { "hello", "hello world", "world", "haesni", "haesni is fast" };

    for (auto& str : hash_strings) {
        std::cout << "haesni(\"" << str << "\") = "
            << haesni::hash(&str[0], str.length()) << std::endl;
    }

    std::cout << "static tests:" << std::endl;

    {
        std::string s1 = "pppppppppp aaaaaaaaaa wwwwwwwwwwwwwwwww ssssssssssssssssssssss fffffffffff";
        std::string s2 = "pppppppppp aaaaaaaaaa wwwwwwwwwwwwwwwww ssssssssssssssssssssss fffffffffff";
        std::cout << " equal hash: ";
        assert(haesni::hash(&s1[0], s1.length()) == haesni::hash(&s2[0], s2.length()));
        std::cout << "done." << std::endl;
    }
    {
        std::string s1 = "pppppppppp aaaaaaaaaa wwwwwwwwwwwwwwwww ssssssssssssssssssssss fffffffffff";
        std::string s2 = "pppppppppp aaaaaaaaaa wwwwwwwwwwwwwwwww ssssssssssssssssssssss fffffffffffsss";
        std::cout << " unequal hash: ";
        assert(haesni::hash(&s1[0], s1.length()) != haesni::hash(&s2[0], s2.length()));
        std::cout << "done." << std::endl;
    }

    return 0;
}
