# oathgen

[Oathgen](https://www.go350.com/posts/oathgen/) is a command line [HOTP](https://tools.ietf.org/html/rfc4226) and [TOTP](https://tools.ietf.org/html/rfc6238) one-time password generator for BSD, Linux, Mac and Windows operating systems. The goal of oathgen is to be complete, standard and portable.

Oathgen is written in C++11. In order to build oathgen, you must use a C++11 compiler. See the **build.sh** script for the exact compiler options. Oathgen has been built on g++, clang++ and MSVC++ compilers, but other C++11 compilers may work too.

Oathgen uses the [Crypto++ 8.0](https://cryptopp.com/) library for hexadecimal and base32 decoding and HMAC-SHA1 and SHA2. Older versions of Crypto++ may not work due to a [bug with base32 encoding](https://github.com/weidai11/cryptopp/issues/108)

To learn more about oathgen, read my [blog post](https://www.go350.com/posts/oathgen/).
