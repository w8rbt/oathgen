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

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "src/global.hpp"

const std::string test_sha1_secret       = "12345678901234567890";
const std::string test_sha256_secret     = "12345678901234567890123456789012";
const std::string test_sha512_secret     = "1234567890123456789012345678901234567890123456789012345678901234";

const std::string test_sha1_b32_secret   = "GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ";
const std::string test_sha256_b32_secret = "GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQGEZA";
const std::string test_sha512_b32_secret = "GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ" \
                                           "GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ" \
                                           "GEZDGNA";

const std::string test_sha1_hex_secret   = "3132333435363738393031323334353637383930";
const std::string test_sha256_hex_secret = "3132333435363738393031323334353637383930313233343536373839303132";
const std::string test_sha512_hex_secret = "3132333435363738393031323334353637383930" \
                                           "3132333435363738393031323334353637383930" \
                                           "3132333435363738393031323334353637383930" \
                                           "31323334";

bool test_hotp( const std::int64_t test_counter, 
                const std::int32_t test_bincode, 
                const std::int32_t test_otp,
                const std::int32_t length,
                const std::string& test_secret,
                const bool hex )
{
    // return 0 on failure
    const std::string               secret   = decode( test_secret, hex );
    const std::vector<std::uint8_t> the_hotp = hotp( secret, test_counter );
    const std::int32_t              bincode  = binary( the_hotp );
    const std::int32_t              gen_otp  = truncate( bincode, length );

    if ( test_otp != gen_otp || test_bincode != bincode )
    {
        if ( DEBUG )
        {
            std::cout << std::setfill('0');
            std::cout << "--------------------------------------------\n";
            std::cout << "test_hotp - length: "            << length                        << "\n";
            std::cout << "test_hotp - count: "             << test_counter                  << "\n";
            std::cout << "test_hotp - test bincode: "      << test_bincode                  << "\n";
            std::cout << "test_hotp - generated bincode: " << bincode                       << "\n";
            std::cout << "test_hotp - KnownGood OTP: "     << std::setw(length) << test_otp << "\n";
            std::cout << "test_hotp - Generated OTP: "     << std::setw(length) << gen_otp  << "\n";
            std::cout << "--------------------------------------------\n";
        }
        return 0;
    }
    return 1;
}

bool test_totp( const std::int64_t test_time, 
                const std::int64_t time_step,
                const std::int64_t unix_epoch,
                const std::int32_t test_otp,
                const std::int32_t length,
                const std::int32_t hmac_type,
                const std::string& test_secret,
                const bool hex )
{
    // return 0 on failure
    const std::string               secret   = decode( test_secret, hex );
    const std::vector<std::uint8_t> the_totp = totp( secret, test_time, time_step, unix_epoch, hmac_type );
    const std::int32_t              bincode  = binary( the_totp );
    const std::int32_t              gen_otp  = truncate( bincode, length );

    if ( test_otp != gen_otp )
    {
        if ( DEBUG )
        {
            std::cout << std::setfill('0');
            std::cout << "--------------------------------------------\n";
            std::cout << "test_totp - test_time: "     << test_time                     << "\n";
            std::cout << "test_totp - test_step: "     << time_step                     << "\n";
            std::cout << "test_totp - unix_epoch: "    << unix_epoch                    << "\n";
            std::cout << "test_totp - length: "        << length                        << "\n";
            std::cout << "test_totp - hmac_type: "     << hmac_type                     << "\n";
            std::cout << "test_totp - KnownGood OTP: " << std::setw(length) << test_otp << "\n";
            std::cout << "test_totp - Generated OTP: " << std::setw(length) << gen_otp  << "\n";
            std::cout << "--------------------------------------------\n";
        }
        return 0;
    }
    return 1;
}

void test()
{
    // Note - Remove leading zeros from test OTPs (octal)
    bool pass = true;

    //          count, bincode,    hotp,   length, secret,          hex 
    if ( !test_hotp(0, 1284755224, 755224, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(1, 1094287082, 287082, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(2, 137359152,  359152, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(3, 1726969429, 969429, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(4, 1640338314, 338314, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(5, 868254676,  254676, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(6, 1918287922, 287922, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(7, 82162583,   162583, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(8, 673399871,  399871, 6, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_hotp(9, 645520489,  520489, 6, test_sha1_hex_secret, 1) ) {pass = false;}

    if ( !test_hotp(0, 1284755224, 755224, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(1, 1094287082, 287082, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(2, 137359152,  359152, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(3, 1726969429, 969429, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(4, 1640338314, 338314, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(5, 868254676,  254676, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(6, 1918287922, 287922, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(7, 82162583,   162583, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(8, 673399871,  399871, 6, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_hotp(9, 645520489,  520489, 6, test_sha1_b32_secret, 0) ) {pass = false;}

    //      time (sec),        step, epoch, totp, length, hmac_type, secret, hex           
    if ( !test_totp(59,          30,   0,   94287082, 8, 160, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1111111109,  30,   0,   7081804,  8, 160, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1111111111,  30,   0,   14050471, 8, 160, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1234567890,  30,   0,   89005924, 8, 160, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(2000000000,  30,   0,   69279037, 8, 160, test_sha1_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(20000000000, 30,   0,   65353130, 8, 160, test_sha1_hex_secret, 1) ) {pass = false;}

    if ( !test_totp(59,          30,   0,   94287082, 8, 160, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1111111109,  30,   0,   7081804,  8, 160, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1111111111,  30,   0,   14050471, 8, 160, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1234567890,  30,   0,   89005924, 8, 160, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(2000000000,  30,   0,   69279037, 8, 160, test_sha1_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(20000000000, 30,   0,   65353130, 8, 160, test_sha1_b32_secret, 0) ) {pass = false;}

    if ( !test_totp(59,          30,   0,   46119246, 8, 256, test_sha256_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1111111109,  30,   0,   68084774, 8, 256, test_sha256_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1111111111,  30,   0,   67062674, 8, 256, test_sha256_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1234567890,  30,   0,   91819424, 8, 256, test_sha256_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(2000000000,  30,   0,   90698825, 8, 256, test_sha256_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(20000000000, 30,   0,   77737706, 8, 256, test_sha256_hex_secret, 1) ) {pass = false;}

    if ( !test_totp(59,          30,   0,   46119246, 8, 256, test_sha256_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1111111109,  30,   0,   68084774, 8, 256, test_sha256_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1111111111,  30,   0,   67062674, 8, 256, test_sha256_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1234567890,  30,   0,   91819424, 8, 256, test_sha256_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(2000000000,  30,   0,   90698825, 8, 256, test_sha256_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(20000000000, 30,   0,   77737706, 8, 256, test_sha256_b32_secret, 0) ) {pass = false;}

    if ( !test_totp(59,          30,   0,   90693936, 8, 512, test_sha512_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1111111109,  30,   0,   25091201, 8, 512, test_sha512_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1111111111,  30,   0,   99943326, 8, 512, test_sha512_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(1234567890,  30,   0,   93441116, 8, 512, test_sha512_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(2000000000,  30,   0,   38618901, 8, 512, test_sha512_hex_secret, 1) ) {pass = false;}
    if ( !test_totp(20000000000, 30,   0,   47863826, 8, 512, test_sha512_hex_secret, 1) ) {pass = false;}

    if ( !test_totp(59,          30,   0,   90693936, 8, 512, test_sha512_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1111111109,  30,   0,   25091201, 8, 512, test_sha512_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1111111111,  30,   0,   99943326, 8, 512, test_sha512_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(1234567890,  30,   0,   93441116, 8, 512, test_sha512_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(2000000000,  30,   0,   38618901, 8, 512, test_sha512_b32_secret, 0) ) {pass = false;}
    if ( !test_totp(20000000000, 30,   0,   47863826, 8, 512, test_sha512_b32_secret, 0) ) {pass = false;}

    if ( pass ) {std::cout << "All tests passed.\n";}
    else {std::cout << "Some tests failed.\n";}
}
