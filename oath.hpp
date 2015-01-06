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

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>

#include "global.hpp"

const std::vector<std::uint8_t> hotp( const std::string& secret,
                                      const std::int64_t counter,
                                      const std::int32_t hmac_type=160 )
{
    // RFC 4226 defines the counter (aka moving factor) as a long type

    std::uint8_t counter_array[ sizeof(std::int64_t) ];

    *reinterpret_cast<std::int64_t*>(counter_array) = counter;
    // The following line breaks on big endian systems...
    // Comment it out when using Sparc64, etc. until a fix is added
    std::reverse( std::begin(counter_array), std::end(counter_array) );

    if( DEBUG )
    {
        std::cout << "hotp - counter: " << counter << "\n";

        for( std::int32_t i = 7; i >= 0; --i )
        {
            std::cout << "hotp - counter_array["     << i << "]: "
                      << (std::uint16_t) counter_array[i] << "\n";
        }
    }

    std::vector<std::uint8_t>        result_160(20);
    std::vector<std::uint8_t>        result_256(32);
    std::vector<std::uint8_t>        result_512(64);

	CryptoPP::HMAC<CryptoPP::SHA1>   mac_160;
	CryptoPP::HMAC<CryptoPP::SHA256> mac_256;
	CryptoPP::HMAC<CryptoPP::SHA512> mac_512;

	mac_160.SetKey( (const std::uint8_t*)secret.c_str(), secret.size() );
    mac_160.CalculateDigest( &result_160[0], counter_array, 8 );

	mac_256.SetKey( (const std::uint8_t*)secret.c_str(), secret.size() );
    mac_256.CalculateDigest( &result_256[0], counter_array, 8 );

	mac_512.SetKey( (const std::uint8_t*)secret.c_str(), secret.size() );
    mac_512.CalculateDigest( &result_512[0], counter_array, 8 );

	if( hmac_type == 160 ) {return result_160;}
	if( hmac_type == 256 ) {return result_256;}
	if( hmac_type == 512 ) {return result_512;}

    return result_160;
}

const std::vector<std::uint8_t> totp( const std::string& secret, 
                                      const std::int64_t time_now,
                                      const std::int64_t time_step,
                                      const std::int64_t unix_epoch,
                                      const std::int32_t hmac_type )
{
    // Note - On some systems, std::time_t may be 32-bits.

    const std::int64_t time_counter = ( time_now - unix_epoch ) / time_step;

    if( DEBUG )
    {
        std::cout << "totp - time (seconds): " << time_now     << "\n";
        std::cout << "totp - time_step: "      << time_step    << "\n";
        std::cout << "totp - unix_epoch: "     << unix_epoch   << "\n";
        std::cout << "totp - time_counter: "   << time_counter << "\n";
        std::cout << "totp - hmac_type: "      << hmac_type    << "\n";
    }

    return hotp( secret, time_counter, hmac_type );
}

std::int32_t binary( std::vector<std::uint8_t> hmac )
{
    const std::int32_t offset  = hmac[ hmac.size() - 1 ] & 0xf;

    const std::int32_t bincode = (( hmac[offset] & 0x7f ) << 24 )
           | (( hmac[offset+1] & 0xff ) << 16 )
           | (( hmac[offset+2] & 0xff ) <<  8 )
           | (( hmac[offset+3] & 0xff ) <<  0 );
        
    if ( DEBUG )
    {
        std::cout << "binary - hmac_size: " << hmac.size() << "\n";
        std::cout << "binary - offset: "    << offset      << "\n";
        std::cout << "binary - bincode: "   << bincode     << "\n";
    }

    return bincode;
}

std::int32_t truncate( const std::int32_t bincode, const std::int32_t length )
{
    std::int32_t m = 0;

    if( length == 6 ) m = 1000000;
    if( length == 7 ) m = 10000000;
    if( length == 8 ) m = 100000000;

    const std::int32_t otp(bincode % m);

    if ( DEBUG )
    {
        std::cout << std::setfill('0');
        std::cout << "truncate - bincode: "   << bincode                  << "\n";
        std::cout << "truncate - modulo by: " << m                        << "\n";
        std::cout << "truncate - length: "    << length                   << "\n";
        std::cout << "truncate - otp: "       << std::setw(length) << otp << "\n";
        std::cout << "--------------------------------------------\n";
    }

    return otp;
}
