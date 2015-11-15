haesni
======
`haesni` is a small hash function using aes-ni for fast string hashing.
I programmed this prototype in my master thesis and measure speed performance comparing to `boost::hash_range`.
As a result, `haesni` is in most cases similar fast as boost, however `haesni` has less collisions.

Important note: there is no check if aes-ni instruction is available.

requirements
-----------
* scons
* g++

Usage
-----
Please look at `example.cpp`, you can compile the example with a simple `scons` call.

But in general it is easy to use `haesni`:
```
#include "haesni.hpp"
...
std::string str = "hello world";
unsigned long hash = haesni::hash(&str[0], str.length());
```

