/*
 * #%L
 * OME-COMMON C++ library for C++ compatibility/portability
 * %%
 * Copyright © 2006 - 2015 Open Microscopy Environment:
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

#include <iterator>
#include <string>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <ome/compat/cstdint.h>

namespace ome
{
  namespace common
  {

    /**
     * Base64-encode a range of bytes.
     *
     * The iterators may be of any uint8_t iterator type, for example
     * a range in a container, raw pointers or any other compatible
     * type.
     *
     * @param begin the start of the byte range.
     * @param end the end of the byte range.
     * @returns a Base64-encoded string.
     */
    template<typename Iterator>
    std::string
    base64_encode(Iterator begin,
                  Iterator end)
    {
      using namespace boost::archive::iterators;

      // Iterator for encoding.
      typedef
        insert_linebreaks<    // break line every 76 characters
          base64_from_binary< // base64 from binary input
            transform_width<  // get 6-bit integers from octets
              Iterator,
              6,
              8,
              char>
            >,
        76
        >
        base64_encode_iterator;

      // Do the encoding.
      std::string encoded((base64_encode_iterator(begin)),
                          (base64_encode_iterator(end)));

      // Pad out to multiple of three with =.
      unsigned int padcount = (3 - std::distance(begin, end) % 3) % 3;
      std::string pad(padcount, '=');
      encoded += pad;

      return encoded;
    }

    /**
     * Decode a Base64-encoded string.
     *
     * Note that trailing whitespace is counted as invalid input, but
     * newlines and other whitespace breaking up the input is
     * permitted.
     *
     * @param base64 the Base64-encoded string.
     * @returns a container filled with the decoded bytes.
     * @throws boost::archive::iterators::dataflow_exception on invalid input.
     */
    template<typename Container>
    Container
    base64_decode(std::string base64)
    {
      using namespace boost::archive::iterators;

      // Iterator for decoding.
      typedef
        transform_width< // get 8-bit integers from sextets
          binary_from_base64<
            remove_whitespace<std::string::const_iterator> >,
        8,
        6>
        base64_decode_iterator;

      unsigned int padcount = std::count(base64.begin(), base64.end(), '=');

      // replace '=' by base64 encoding of '\0'
      std::replace(base64.begin(), base64.end(),
                   '=', 'A');


      Container decoded;

      // Do the decoding.
      std::copy(base64_decode_iterator(base64.begin()),
                base64_decode_iterator(base64.end()),
                std::back_inserter(decoded));

      decoded.erase(decoded.end() - padcount, decoded.end());

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
