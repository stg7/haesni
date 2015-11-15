haesni
======
`haesni` is a small hash function using aes-ni for fast string hashing.
I programmed this prototype for my master thesis and measure speed performance comparing to `boost::hash_range`.
As a result, `haesni` is in most cases similar fast as boost, however `haesni` has less collisions.

Important note: there is no check if aes-ni instruction is available.
Most modern CPU's, e.g. intel i5 series or similar have this instruction,
but e.g. cheaper models like intel i3 are not able for hardware aes, please use
another hash function in these cases (for example [FNVhash](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function), or `boost::hash_range`).

requirements
-----------
* scons
* g++

Usage
-----
Please look at `example.cpp`, you can compile the example with a simple `scons` call.

But in general it is easy to use `haesni`:
```
#include "haesni.hpp"  //< include header
...
std::string str = "hello world";
unsigned long hash = haesni::hash(&str[0], str.length());  //< start hashing
```

