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
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "bop.hpp"
#include "global.hpp"
#include "main.hpp"
#include "oath.hpp"
#include "strings.hpp"
#include "test.hpp"

std::int32_t main( std::int32_t argc, char * argv[] )
{
    // Parse the user provided flags

    vec flags;
    load_flags( argc, argv, flags );

    if( std::find( flags.begin(), flags.end(), "-d" ) != flags.end() )
    {
        DEBUG = true;
    }

    if( std::find( flags.begin(), flags.end(), "-f" ) != flags.end() )
    {
        do_file = true;
    }

    if( std::find( flags.begin(), flags.end(), "-t" ) != flags.end() )
    {
        test();
        return 0;
    }

    if( std::find( flags.begin(), flags.end(), "-v" ) != flags.end() )
    {
        std::cout << version << "\n";
        return 0;
    }

    if( argc == 1 || std::find( flags.begin(), flags.end(), "-h" ) != flags.end() )
    {
        std::cout << help << "\n";
        return 0;
    }

    if( std::find( flags.begin(), flags.end(), "-hex" ) != flags.end() )
    {
        do_hex = true;
    }

    if( std::find( flags.begin(), flags.end(), "-hotp" ) != flags.end() )
    {
        do_hotp = true;
    }

    if( get_value( flags, "-l", m_len ) )
    {
        const std::int32_t tmp = strtonum<std::int32_t>( m_len );
        if ( tmp < 6 || tmp > 8 )
        {
            std::cerr << "The -l flag must be 6, 7 or 8.\n";
            return 1;
        }
        length = tmp;
    }

    if( get_value( flags, "-c", m_count ) )
    {
        hotp_count = strtonum<std::int64_t>( m_count );
    }

    if( get_value( flags, "-hmac", m_hmac ) )
    {
        if     ( m_hmac == "sha1"   || m_hmac == "SHA1"   ) {hmac_type = 160;}
        else if( m_hmac == "sha256" || m_hmac == "SHA256" ) {hmac_type = 256;}
        else if( m_hmac == "sha512" || m_hmac == "SHA512" ) {hmac_type = 512;}

        else 
        { 
            std::cerr << "The -hmac flag must be sha1, sha256 or sha512.\n";
            return 1;
        }
    }

    if( get_value( flags, "-e", m_epoch ) )
    {
        unix_epoch = strtonum<std::int64_t>( m_epoch );
    }

    if( get_value( flags, "-ts", m_ts ) )
    {
        time_step = strtonum<std::int64_t>( m_ts );
    }

    if( get_value( flags, "-s", m_sec ) )
    {
        if ( do_file )
        {
            std::ifstream fp ( m_sec );

            if ( !fp.is_open() )
            {
                std::cerr << "Cannot open the file '" << m_sec << "'.\n";
                return 1;
            }

            else
            {
                std::getline( fp, secret );
                fp.close();
            }
        }

        else
        {
            secret = m_sec;
        }
        
        if( do_hex ) {decoded = extend( decode( secret, true ), hmac_type );}
        else          {decoded = extend( decode( secret, false ), hmac_type );}
    }

    // Sanity Checks
    if( do_hotp && hmac_type != 160 )
    {
        std::cerr << "The HOTP standard only supports SHA1.\n";
        return 1;
    }

    if( !do_hotp && hotp_count )
    {
        std::cerr << "-c requires the -hotp flag.\n";
        return 1;
    }

    if( do_hotp && unix_epoch )
    {
        std::cerr << "-e may not be used with -hotp.\n";
        return 1;
    }

    if( do_hotp && time_step != 30 )
    {
        std::cerr << "-ts may not be used with -hotp.\n";
        return 1;
    }

    // Do HOTP or TOTP

    if ( do_hotp && hmac_type == 160 )
    {
        otp = truncate( binary( hotp( decoded, hotp_count ) ), length );
    }
    else
    {
        otp = truncate( binary( totp( decoded, std::time( nullptr ), time_step, unix_epoch, hmac_type ) ), length );
    }

    std::cout << std::setfill( '0' ) << std::setw( length ) << otp << "\n";
  
    return 0;
}
