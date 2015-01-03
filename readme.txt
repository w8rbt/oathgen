* Introduction

Oathgen is a command line HOTP and TOTP one-time password generator for BSD, 
Linux, Mac and Windows operating systems. The goal of oathgen is to be 
complete, standard and portable.

Oathgen generates OTPs in six, seven or eight digit lengths in the following 
formats.
    
    1. HOTP HMAC-SHA1
    2. TOTP HMAC-SHA1 
    3. TOTP HMAC-SHA256 
    4. TOTP HMAC-SHA512 

* Groups, Standards and RFCs

    http://www.openauthentication.org/
    http://tools.ietf.org/html/rfc4226
    http://tools.ietf.org/html/rfc6238

* Some usage examples

Show the help menu or see what version of oathgen you have.

    oathgen -h
    oathgen -v

Run verbose tests.

    oathgen -d -t

Generate a six digit HMAC-SHA1 TOTP from a base32 encoded secret using a 30 
second timestep. This is basically 'Google Authenticator' and is the default 
mode for oathgen.

    oathgen -s GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ 

Generate an eight digit HMAC-SHA1 TOTP from a hex encoded secret. In this 
example, we override the default OTP length (-l 8) and the default encoding 
(-hex) and the default time step (-ts 10). Once again, TOTP is implied.
    
    oathgen -s 3132333435363738393031323334353637383930 -l 8 -hex -ts 10

Generate a seven digit HMAC-SHA1 HOTP. Here we override TOTP by specifying 
(-hotp) and the secret is stored in a file (-f) rather than passed on the 
command line. We also pass the HOTP count (-c 1).

    oathgen -s /home/user/.oathgen/hex_test_secret.txt -f -l 7 -c 1 -hotp -hex

Generate a six digit HMAC-SHA512 TOTP. In this example, we specify (-hmac sha512) 
so that HMAC-SHA512 is used rather than HMAC-SHA1. The -hmac flag accepts sha1 
(the default) sha256 and sha512.

    oathgen -s /home/user/.oathgen/base32_test_secret.txt -f -hmac sha512 

* The Test Secret (the unencoded secret is the HMAC key)

    B32:       GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ 
    Hex:       3132333435363738393031323334353637383930 
    Unencoded: 12345678901234567890

* Secret and Secret File Notes

The secret is used as the HMAC key. The counter (aka moving factor) is used as 
the HMAC message. The TOTP RFC states, "Keys SHOULD be of the length of the 
HMAC output to facilitate interoperability." This means that the decoded secret
(which is the key) should be 20 bytes for HMAC-SHA1, 32 bytes for HMAC-SHA256
and 64 bytes for HMAC-SHA512. However, many TOTP implementations, use smaller
keys. In fact, 10 byte secrets seem more common than 20 byte secrets. Of the
five services I tested, three used 10 byte secrets. The other two services
used 20 byte secrets, both were Google. I wrote an 'extend' function (which
is a hack) to take a 20 byte secret and extend it for use with HMAC-SHA256
and HMAC-SHA512. While this does extend a 20 byte secret to pass the TOTP RFC
test cases, it will likely fail in real-world scenarios with implementations
that use smaller secrets. Really, the RFC should mandate the secret size to
avoid this sort of thing.

When storing secrets in text files, only store one secret per file. The secret 
should be on one line by itself with no spaces or dashes between the 
characters. The secret file should not contain any other lines. See the example 
secret files included with the oathgen source code.

The secret files should be read-only by the file's owner. No other user should 
be able to read the secret files. Secret files should have 0400 permissions on 
most systems and should be stored in user home folders. It's good practice to 
treat OATH secret files as if they were private ssh key files.

For portability, you may place the secret files on an encrypted USB memory 
stick and carry that with you from system to system. If you do this, it's also 
a good idea to carry statically linked copies of the oathgen binary on the same 
memory stick.

* Type Notes

The HOTP 'counter' (aka moving factor), the Unix epoch time and the time step
are 8-byte 64-bit signed integers. All of the other integer types in oathgen
are 4-byte, 32-bit signed.

With C and C++, some 32-bit systems may use a 32-bit std::time_t while others 
may use 64-bits. Also, the 'long' type could be 32-bits on some systems while 
64-bits on others. So functions such as stol may not work consistently across 
systems. I tried to account for these things in oathgen, but if you find a bug 
or a system where the built-in test cases do not work, submit a bug report.

Oathgen has been built and tested on i386, amd64, Sparc64 and 32-bit ARM. Other
platforms should work too.

* 1.0 Options

oathgen version 1.0 flags:
 -c     set the HOTP counter value (requires -hotp flag)
 -d     show verbose debug output
 -e     set the Unix epoch time in seconds (default is 0)
 -f     read the secret from a text file rather than the command line
 -h     show this help message
 -hex   use hex to decode the secret (default is base32)
 -hmac  set the TOTP HMAC type. sha1, sha256 or sha512 (default is sha1)
 -hotp  use hotp (default is totp)
 -l     set the length of OTP (6, 7 or 8 digits) default is 6
 -s     the hex or base32 encoded secret or the file that contains it
 -t     run tests (use with -d flag for verbose output)
 -ts    set the TOTP time step duration (default is 30)
 -v     show version

