/*
 * #%L
 * OME-COMMON C++ library for C++ compatibility/portability
 * %%
 * Copyright © 2016 Open Microscopy Environment:
 *   - Massachusetts Institute of Technology
 *   - National Institutes of Health
 *   - University of Dundee
 *   - Board of Regents of the University of Wisconsin-Madison
 *   - Glencoe Software, Inc.
 * %%
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of any organization.
 * #L%
 */

#ifndef OME_COMMON_BASE64_H
#define OME_COMMON_BASE64_H

#include <cctype>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <string>

namespace ome
{
  namespace common
  {

    namespace detail
    {

      /**
       * Append a Base64-encoded character to a Base64-encoded string.
       *
       * Line breaks will be automatically appended if required.
       *
       * @param encoded the Base64-encoded string.
       * @param value the value to append.
       * @param chars the number of Base64 characters written (used
       * for linebreaking).
       * @param linebreak the position at which to break a line; zero
       * to disable line breaks.
       */
      inline void
      output_base64_char(std::string& encoded,
                         uint8_t      value,
                         uint64_t     chars,
                         uint8_t      linebreak)
      {
        // Static mapping using simple offsets.
        static const char * const base64_chars =
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ" //  0-25
          "abcdefghijklmnopqrstuvwxyz" // 26-51
          "0123456789+/=";             // 52-63

        encoded += base64_chars[value];
        if (linebreak && (chars + 1) % linebreak == 0)
          encoded += '\n';
      }

      /**
       * Get the value corresponding to a Base64-encoded character.
       *
       * The returned value will be in the range 0-63 for valid
       * Base64 characters.  98 is used for padding.
       *
       * @param c the character to look up.
       * @returns the value.
       * @throws std::runtime_error if an invalid character is used.
       */
      inline uint8_t
      base64_value(char c)
      {
        // Static mapping; due to the nature of the encoding, it's sparse.
        // Without C0 (-0x20) to reduce size. 98=pad, 99=invalid.
        static const uint8_t base64_codes[] =
          {
            // ' ' ! " # $ % & ' ( ) * + , - . /
            99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 62, 99, 99, 99, 63,
            // 0 1 2 3 4 5 6 7 8 9 : ; < = > ?
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 99, 99, 99, 98, 99, 99,
            // @ A B C D E F G H I J K L M N O
            99, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
            // P Q R S T U V W X Y Z [ \ ] ^ _
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 99, 99, 99, 99, 99,
            // ` a b c d e f g h i j k l m n o
            99, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            // p q r s t u v w x y z
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
          };

        if (c < 0x20 || c > 'z')
          throw std::runtime_error("Invalid Base64 input: character outside permitted range");
        uint8_t v = base64_codes[c - 0x20];
        if (v == 99)
          throw std::runtime_error("Invalid Base64 input: character outside permitted range");
        return v;
      }

      /**
       * Get the next character value from a Base64-encoded string.
       *
       * Any whitespace in the string will be automatically skipped.
       *
       * The returned value will be in the range 0-63 for valid Base64
       * characters.  98 is used for padding. 255 is used if the end
       * of the string was reached (used to detect end of valid input
       * in the presence of trailing whitespace).
       *
       * @param pos the current position in the string.
       * @param end the end of the string.
       * @returns the character value, padding or end of string marker.
       * @throws std::runtime_error if an invalid character is encountered.
       */
      inline uint8_t
      next_base64_value(std::string::const_iterator& pos,
                        std::string::const_iterator  end)
      {
        while (pos != end)
          {
            if (!std::isspace(*pos))
              {
                uint8_t value = base64_value(*pos++);
                return value;
              }
            ++pos;
          }
        return 255;
      }

    }

    /**
     * Base64-encode a range of bytes.
     *
     * The iterators may be of any uint8_t iterator type, for example
     * a range in a container, raw pointers or any other compatible
     * type.
     *
     * @param begin the start of the byte range.
     * @param end the end of the byte range.
     * @param linebreak the position at which to break a line; zero
     * to disable line breaks.
     * @returns a Base64-encoded string.
     */
    template<typename Iterator>
    std::string
    base64_encode(Iterator begin,
                  Iterator end,
                  uint8_t  linebreak = 76)
    {
      std::string encoded;
      encoded.reserve((std::distance(begin, end) * 4) / 3);

      uint64_t bytes = 0U;
      uint64_t chars = 0U;
      uint8_t accum = 0U;
      for (Iterator i = begin; i != end; ++bytes)
        {
          uint8_t byte = *i; // Byte 1
          ++i;
          accum = byte >> 2;
          detail::output_base64_char(encoded, accum, chars++, linebreak); // Char 1
          accum = (byte & 0x3) << 4;
          if (i != end)
            {
              byte = *i; // Byte 2
              ++i;
              accum |= (byte & 0xF0) >> 4;
              detail::output_base64_char(encoded, accum, chars++, linebreak); // Char 2
              accum = (byte & 0x0F) << 2;
              if (i != end)
                {
                  byte = *i; // Byte 3
                  ++i;
                  accum |= (byte & 0xC0) >> 6;
                  detail::output_base64_char(encoded, accum, chars++, linebreak); // Char 3
                  accum = byte & 0x3F;
                  detail::output_base64_char(encoded, accum, chars++, linebreak); // Char 4
                }
              else
                {
                  detail::output_base64_char(encoded, accum, chars++, linebreak); // Char 3 (+pad)
                  encoded += '='; // Char 4 (pad)
                }
            }
          else
            {
              detail::output_base64_char(encoded, accum, chars++, linebreak); // Char 2 (+pad)
              encoded += "=="; // Chars 3 and 4
            }
        }

      return encoded;
    }

    /**
     * Decode a Base64-encoded string.
     *
     * Newlines and other whitespace breaking up the input are
     * permitted.
     *
     * @param base64 the Base64-encoded string.
     * @param dest the insert iterator for the destination
     * @throws std::runtime_error on invalid input.
     */
    template<typename InsertIterator>
    void
    base64_decode(std::string    base64,
                  InsertIterator dest)
    {
      bool pad_seen = false;
      uint8_t bytes[4];
      uint8_t output;
      for (std::string::const_iterator i = base64.begin();
           i != base64.end();)
        {
          // Get next 4 bytes.  If only whitespace remains, the first
          // byte will be 255.  Since the input is blocked into groups
          // of four characters, fetch four at once.
          for (int j = 0; j < 4; ++j)
            {
              bytes[j] = detail::next_base64_value(i, base64.end());
              if(j == 0 && bytes[j] == 255)
                break;
            }

          if (bytes[0] == 255) // End of input (expected)
            break;
          if (bytes[1] == 255 || bytes[2] == 255 || bytes[3] == 255) // End of input (unexpected)
            throw std::runtime_error("Invalid Base64 input: unexpected end of input");

          if (bytes[0] < 64 && bytes[1] < 64) // Valid input
            {
              if(pad_seen) // Padding only allowed at end.
                throw std::runtime_error("Invalid Base64 input: padding only permitted at end of input");

              output = bytes[0] << 2 | bytes[1] >> 4;
              *dest = output; // Byte 1
              if (bytes[2] < 64) // Skip if padded
                {
                  output = bytes[1] << 4 | bytes[2] >> 2;
                  *dest = output; // Byte 2
                  if (bytes[3] < 64) // Skip if padded
                    {
                      output = bytes[2] << 6 | bytes[3];
                      *dest = output; // Byte 3
                    }
                  else
                    {
                      pad_seen = true;
                    }
                }
              else
                {
                  pad_seen = true;
                }
            }
          else
            {
              throw(std::runtime_error("Invalid Base64 input: padding encountered unexpectedly"));
            }
        }
    }

    /**
     * Decode a Base64-encoded string into a container.
     *
     * Newlines and other whitespace breaking up the input are
     * permitted.
     *
     * @param base64 the Base64-encoded string.
     * @returns a container filled with the decoded bytes.
     * @throws std::runtime_error on invalid input.
     */
    template<typename Container>
    Container
    base64_decode(const std::string& base64)
    {
      Container decoded;

      decoded.reserve((base64.size() * 3) / 4);

      base64_decode(base64, std::back_inserter(decoded));

      return decoded;
    }

  }
}

#endif // OME_COMMON_BASE64_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
