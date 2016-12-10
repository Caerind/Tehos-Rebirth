// Assume is a smarter assert replacement (C++03) by r-lyeh

/*

Assume <a href="https://travis-ci.org/r-lyeh/assume"><img src="https://api.travis-ci.org/r-lyeh/assume.svg?branch=master" align="right" /></a>
=======

- Assume is a smarter assert replacement (C++03).
- Assume is handy. LHS/RHS values are printed as long as they are `ostream` friendly.
- Assume is cross-platform. Crash handler fallbacks to `assert()` symbol.
- Assume is header only.
- Assume is zlib/libpng licensed.

## Sample

```c++
#include "assume.hpp"

int main() {
    int a = 1, b = 2;

    assume( a < b );
    assume( a > b );
}
```

## Showcase

```c++
#~/> g++ sample.cc && ./a.out
<assume.hpp> says: expression failed! (a > b) -> (1 > 2) -> (unexpected) at sample.cc:7
```


You can find this project here : https://github.com/r-lyeh/assume

*/

/*

Copyright (c) 2015 r-lyeh (https://github.com/r-lyeh)

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

/* Private API */

#ifndef ASSUME_HPP_HEADER
#define ASSUME_HPP_HEADER

#define ASSUME_VERSION "1.0.1"

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <deque>
#include <string>
#include <sstream>

#include "Log.hpp"

namespace assume {
    class check {
        std::deque< std::string > xpr;
        bool ok;
        template<typename T> static std::string to_str( const T &t ) { std::stringstream ss; return (ss << t) ? ss.str() : "??"; }
    public:
        check( const char *const text, const char *const file, int line, bool result )
        :   xpr(4), ok(result)
        { xpr[0] = std::string(file) + ':' + to_str(line); xpr[2] = text; }

        ~check() {
            if( xpr.empty() ) return;
            operator bool();
            xpr[0] = xpr[0] + xpr[1];
            xpr.erase( xpr.begin() + 1 );
            if( !ok ) {
                xpr[2] = xpr[2].substr( xpr[2][2] == ' ' ? 3 : 4 );
                xpr[1].resize( (xpr[1] != xpr[2]) * xpr[1].size() );
                std::string buf = "Expression failed! (" + xpr[1] + ") -> (" + xpr[2] + ") -> (unexpected) at " + xpr[0];
				NLog::instance() << NLog::LogType::Error << buf;
                // assert fallback here
                assert( !"Wrong assume" );
                // user defined fallbacks here
                for(;;) {}
            };
        }
#       define assume$(OP) \
        template<typename T> check &operator OP( const T &rhs         ) { return xpr[3] += " "#OP" " + to_str(rhs), *this; } \
        template<unsigned N> check &operator OP( const char (&rhs)[N] ) { return xpr[3] += " "#OP" " + to_str(rhs), *this; }
        operator bool() {
            if( xpr.size() >= 3 && xpr[3].size() >= 6 ) {
                char signR = xpr[3].at(2);
                bool equal = xpr[3].substr( 4 + xpr[3].size()/2 ) == xpr[3].substr( 3, xpr[3].size()/2 - 3 );
                ok = ( signR == '=' ? equal : ( signR == '!' ? !equal : ok ) );
            }
            return ok;
        }
        assume$(<) assume$(<=) assume$(>) assume$(>=) assume$(!=) assume$(==) assume$(&&) assume$(||)
#       undef assume$
    };
}

#endif

/* Public API */

#include <cassert>

#ifdef Assume
#undef Assume
#endif

#if !(defined(NDEBUG) || defined(_NDEBUG))
#define Assume(...) ( !!(__VA_ARGS__) ? \
        ( assume::check(#__VA_ARGS__,__FILE__,__LINE__,1) < __VA_ARGS__ ) : \
        ( assume::check(#__VA_ARGS__,__FILE__,__LINE__,0) < __VA_ARGS__ ) )
#else
#define Assume assert
#endif
