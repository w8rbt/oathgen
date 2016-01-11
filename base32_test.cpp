#include <iostream>
#include <sstream>
#include <string>

#include <cryptopp/base32.h>

// This file is intended to debug base32 decoding issues in CryptoPP
// when using an alternate alphabet. By default, CryptoPP uses DUDE
// for base32 encoding/decoding. Most other libraries use RFC4648.
// This creates compatibility issues.

// DUDE - http://tools.ietf.org/html/draft-ietf-idn-dude-02
// RFC 4648 - http://tools.ietf.org/html/rfc4648

// Note - Alternate alphabet encoding works, but decoding fails
// with a stack overflow.


static const byte ALPHABET[]          = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"; // RFC4648
static const std::string test_encoded = "GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ";
static const std::string test_decoded = "12345678901234567890";

const std::string encode( const std::string& raw )
{
    std::string encoded;

    CryptoPP::Base32Encoder b32encoder;
    CryptoPP::AlgorithmParameters ep = CryptoPP::MakeParameters(
                                       CryptoPP::Name::EncodingLookupArray(),
                                       (const byte *)ALPHABET,
                                       false);
    b32encoder.IsolatedInitialize(ep);

    b32encoder.Attach( new CryptoPP::StringSink( encoded ) );
    b32encoder.Put( (std::uint8_t*)raw.c_str(), raw.size() );
    b32encoder.MessageEnd();

    return encoded;
}

const std::string decode( const std::string& encoded )
{
    std::string decoded;

    static int decoding_array[256];
    CryptoPP::Base32Decoder::InitializeDecodingLookupArray(decoding_array, 
                               ALPHABET, 
                               32, 
                               true); // false = case insensitive

    CryptoPP::Base32Decoder b32decoder;
    CryptoPP::AlgorithmParameters dp = CryptoPP::MakeParameters(
                                       CryptoPP::Name::DecodingLookupArray(),
                                       (const int *)decoding_array,
                                       false);
    b32decoder.IsolatedInitialize(dp); 

    b32decoder.Attach( new CryptoPP::StringSink( decoded ) );
    b32decoder.Put( (std::uint8_t*)encoded.c_str(), encoded.size() );
    b32decoder.MessageEnd();

    return decoded;
}


int main()
{
    // Test encoding
    const std::string e = encode("12345678901234567890");
    if( e == test_encoded )
    {
        std::cout << e << " RFC4648 Encoding works!\n";
    }
    else
    {
        std::cout << e << " RFC4648 Encoding failed\n";
    }

    // Test decoding
    const std::string d = decode("GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ");
    if( d == test_decoded )
    {
        std::cout << d << " RFC4648 Decoding works!\n";
    }
    else
    {
        std::cout << d << " RFC4648 Decoding failed\n";
    }

    return 0;
}

