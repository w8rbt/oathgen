#!/bin/bash

# usage: build.sh system
#
# notes:
#
#        Change shebang to /bin/sh for OpenBSD and FreeBSD.
#
#        If -std=C++11 fails on some compilers, -std=c++0x may work.
#
#        On Windows, the free Visual Studio Community Edition
#        (MSVC++) compiler is typically used to build oathgen. 
#        Mingw on Windows can compile oathgen too.

if [ $# -eq 0 ]; then
    echo "usage $0 <system>"
    exit 1
fi

if [ $1 == 'b32' -o $1 == 'b32' ]
    then g++ -g -g3 -std=c++11 -Wall -Wextra -Werror \
    -Weffc++ -pedantic-errors base32_test.cpp \
    -o b32_test \
    /usr/lib/libcryptopp.so
fi

if [ $1 == 'Linux' -o $1 == 'linux' ]
    then g++ -static -std=c++11 -Wall -Wextra -Werror \
    -Weffc++ -pedantic-errors main.cpp \
    -o oathgen_linux_$(uname -m) \
    /usr/lib/libcryptopp.a
    strip oathgen_linux_$(uname -m)
fi

if [ $1 == 'Linux_debug' -o $1 == 'linux_debug' ]
    then g++ -std=c++11 -Wall -Wextra -Werror \
    -Weffc++ -pedantic-errors main.cpp \
    -o oathgen_linux_$(uname -m)_debug \
    /usr/lib/libcryptopp.a
fi

if [ $1 == 'OpenBSD' -o $1 == 'openbsd' ]
    then eg++ -static -std=c++0x -Wall -Wextra -Werror \
    -Weffc++ -pedantic-errors main.cpp \
    -o oathgen_openbsd_$(uname -m) \
    /usr/lib/libcryptopp.a
    strip oathgen_openbsd_$(uname -m)
fi

if [ $1 == 'FreeBSD' -o $1 == 'freebsd' ]
    then clang++ -static -std=c++11 -Wall -Wextra -Werror \
    -Weffc++ -pedantic-errors main.cpp \
    -o oathgen_freebsd_$(uname -m) \
    /usr/lib/libcryptopp.a
    strip oathgen_freebsd_$(uname -m)
fi

if [ $1 == 'Mac' -o $1 == 'mac' ]; then
    if [ ! -f /usr/local/lib/libcryptopp.a ]; then
      echo "/usr/local/lib/libcryptopp.a not found."
      echo "Have you installed it, e.g. via brew?"
    fi
    g++ -std=c++11 -Wall -Wextra -Werror \
    -Weffc++ -pedantic-errors main.cpp \
    -isystem /usr/local/include \
    -o oathgen_mac \
    /usr/local/lib/libcryptopp.a
    strip oathgen_mac
fi

if [ $1 == 'Windows' -o $1 == 'windows' ]
    then g++ -static -std=c++11 main.cpp \
    -o oathgen_windows_mingw.exe \
    -Ic:/mingw/msys/1.0/include/ \
    c:/mingw/msys/1.0/lib/libcryptopp.a
    strip oathgen_windows_mingw.exe
fi
