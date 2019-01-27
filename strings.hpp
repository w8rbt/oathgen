/*
 * Copyright (c) 2015 Richard B Tilley <brad@w8rbt.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <iostream>
#include <sstream>
#include <string>

#include <cryptopp/base32.h>
#include <cryptopp/hex.h>

#include "global.hpp"

template <typename number>
number strtonum( const std::string& s )
{
    // Convert string to number "123456789" -> 123456789
    std::stringstream ss( s );
    number n = 0;
    ss >> n;

    if ( DEBUG )
    {
        std::cout << "strtonum - string: "    << s         << "\n";
        std::cout << "strtonum - number: "    << n         << "\n";
        std::cout << "strtonum - sizeof(n): " << sizeof(n) << "\n";
    }

    if ( ss.fail() )
    {
        std::cerr << "Failed to convert " << s << " to a number.\n";
        std::exit(1);
    } 

    return n;
}

const std::string decode( const std::string& secret, const bool hex_encode )
{
    // Hex or base32 decode the secret.
    std::string decoded;

    if ( hex_encode )
    {
        CryptoPP::HexDecoder hexdecoder;
        hexdecoder.Attach( new CryptoPP::StringSink( decoded ) );
        hexdecoder.Put( (std::uint8_t*)secret.c_str(), secret.size() );
        hexdecoder.MessageEnd();

        if( DEBUG )
        {
            std::cout << "decode - using hex encoding\n";
            std::cout << "decode - encoded secret: "      << secret         << "\n";
            std::cout << "decode - encoded secret size: " << secret.size()  << "\n";
            std::cout << "decode - decoded secret: "      << decoded        << "\n";
            std::cout << "decode - decoded secret size: " << decoded.size() << "\n";
        }
    }

    else
    {
        CryptoPP::Base32Decoder b32decoder;
        b32decoder.Attach( new CryptoPP::StringSink( decoded ) );
        b32decoder.Put( (std::uint8_t*)secret.c_str(), secret.size() );
        b32decoder.MessageEnd();

        if( DEBUG )
        {
            std::cout << "decode - using b32 encoding\n";
            std::cout << "decode - encoded secret: "      << secret         << "\n";
            std::cout << "decode - encoded secret size: " << secret.size()  << "\n";
            std::cout << "decode - decoded secret: "      << decoded        << "\n";
            std::cout << "decode - decoded secret size: " << decoded.size() << "\n";
        }
    }

	return decoded;
}
