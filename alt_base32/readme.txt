The files 'alt_base32.cpp' and 'alt_base32.h' were originally hosted by the 
Crypto++ website at this URL http://cryptopp.com/wiki/Base32. That URL has
been removed.

Currently, oathgen requires that Crypto++ is built with alt_base32 support. 

To add alt_base32 functionality to Crypto++, download and extract the Crypto++ 
source code, then copy 'alt_base32.cpp' and 'alt_base32.h' into the extracted 
Crypto++ source code directory and build Crypto++ as you normally would. After 
doing that, you should be able to compile and link oathgen.

I may switch to a different base32 library sometime in the future.

