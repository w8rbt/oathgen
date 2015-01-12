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
 *
 * The basic option parser "bop.hpp" for C++
 *
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::string> vec;

void load_flags( const int argc, char ** argv, vec& v )
{
    for ( int i = 0; i < argc; ++i )
    {
        try 
        {
            v.push_back( argv[i] );
        }

        catch ( const std::bad_alloc& )
        {
            std::cerr << "load_flags() unable to allocate memory. Aborting.\n";
            std::abort();
        }
    }
}

bool get_value( const vec& flags, const std::string& flag, std::string& value )
{
    vec::const_iterator vit = std::find( flags.begin(), flags.end(), flag );

    if( vit != flags.end() && ++vit != flags.end() )
    {
        value = *vit;
        return true;
    }
    return false;
}
