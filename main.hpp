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
#include <string>

bool              do_hex             = false;
bool              do_hotp            = false;
bool              do_file            = false;

std::int32_t      hmac_type          = 160;
std::int64_t      hotp_count         = 0;
std::int32_t      length             = 6;
std::int32_t      otp                = 0;
std::int64_t      time_step          = 30;
std::int64_t      unix_epoch         = 0;

std::string       decoded            = "";
std::string       m_count            = "";
std::string       m_epoch            = "";
std::string       m_hmac             = "";
std::string       m_len              = "";
std::string       m_ts               = "";
std::string       m_sec              = "";
std::string       secret             = "";

const std::string version            = "oathgen version 1.0";
const std::string help               = version + " flags:\n \
-c     set the HOTP counter value (requires -hotp flag)\n \
-d     show verbose debug output\n \
-e     set the Unix epoch time in seconds (default is 0)\n \
-f     read the secret from a text file rather than the command line\n \
-h     show this help message\n \
-hex   use hex to decode the secret (default is base32)\n \
-hmac  set the TOTP HMAC type. sha1, sha256 or sha512 (default is sha1)\n \
-hotp  use hotp (default is totp)\n \
-l     set the length of OTP (6, 7 or 8 digits) default is 6\n \
-s     the hex or base32 encoded secret or the file that contains it\n \
-t     run tests (use with -d flag for verbose output)\n \
-ts    set the TOTP time step duration (default is 30)\n \
-v     show version";
