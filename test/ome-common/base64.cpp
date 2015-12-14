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

#include <algorithm>
#include <cstring>
#include <vector>

#include <ome/common/base64.h>

#include <ome/test/test.h>

class Base64TestParameters
{
public:
  const char *data;
  const char *encoded_data_exact;
  const char *encoded_data_inexact;

  Base64TestParameters(const char *data,
                       const char *encoded_data_exact,
                       const char *encoded_data_inexact):
    data(data),
    encoded_data_exact(encoded_data_exact),
    encoded_data_inexact(encoded_data_inexact)
  {
  }
};

class Base64Test : public ::testing::TestWithParam<Base64TestParameters>
{
};

TEST_P(Base64Test, EncodeRaw)
{
  const Base64TestParameters& params = GetParam();

  std::string result = ome::common::base64_encode(reinterpret_cast<const uint8_t *>(params.data),
                                                  reinterpret_cast<const uint8_t *>(params.data + std::strlen(params.data)));

  ASSERT_EQ(std::string(params.encoded_data_exact), result);
}

TEST_P(Base64Test, EncodeRawNoBreaks)
{
  const Base64TestParameters& params = GetParam();

  std::string result = ome::common::base64_encode(reinterpret_cast<const uint8_t *>(params.data),
                                                  reinterpret_cast<const uint8_t *>(params.data + std::strlen(params.data)),
                                                  0);

  std::string expected(params.encoded_data_exact);
  expected.erase(std::remove(expected.begin(), expected.end(), '\n'), expected.end());

  ASSERT_EQ(expected, result);
}

TEST_P(Base64Test, EncodeVector)
{
  const Base64TestParameters& params = GetParam();

  std::vector<uint8_t> vec(reinterpret_cast<const uint8_t *>(params.data),
                           reinterpret_cast<const uint8_t *>(params.data + std::strlen(params.data)));

  std::string result = ome::common::base64_encode(vec.begin(), vec.end());

  ASSERT_EQ(std::string(params.encoded_data_exact), result);
}


TEST_P(Base64Test, DecodeIter)
{
  const Base64TestParameters& params = GetParam();

  std::vector<uint8_t> expected(reinterpret_cast<const uint8_t *>(params.data),
                                reinterpret_cast<const uint8_t *>(params.data + std::strlen(params.data)));

  std::vector<uint8_t> result;
  ome::common::base64_decode(params.encoded_data_inexact,
                             std::back_inserter(result));
  ASSERT_EQ(expected, result);

  std::vector<uint8_t> result2 = ome::common::base64_decode<std::vector<uint8_t>>(params.encoded_data_exact);
  ASSERT_EQ(expected, result2);
}

TEST_P(Base64Test, DecodeVector)
{
  const Base64TestParameters& params = GetParam();

  std::vector<uint8_t> expected(reinterpret_cast<const uint8_t *>(params.data),
                                reinterpret_cast<const uint8_t *>(params.data + std::strlen(params.data)));

  std::vector<uint8_t> result = ome::common::base64_decode<std::vector<uint8_t>>(params.encoded_data_inexact);
  ASSERT_EQ(expected, result);

  std::vector<uint8_t> result2 = ome::common::base64_decode<std::vector<uint8_t>>(params.encoded_data_exact);
  ASSERT_EQ(expected, result2);
}

TEST(Base64Test, DecodeFail)
{
  // Premature end of input.
  ASSERT_THROW(ome::common::base64_decode<std::vector<uint8_t>>("Invalid "), std::runtime_error);

  // Invalid characters.
  ASSERT_THROW(ome::common::base64_decode<std::vector<uint8_t>>("$#Invalid"), std::runtime_error);

  // Data after padding.
  ASSERT_THROW(ome::common::base64_decode<std::vector<uint8_t>>("VGVzdCBwYWRkaW5nLQ==VGVzdCBwYWRkaW5nLQ=="), std::runtime_error);
}

TEST(Base64Test, LookupRoundTrip)
{
  // Checks consistency of lookup tables to ensure encode and decode behaviour matches.

  for (int i = 0; i < 256; ++i)
    {
      std::vector<uint8_t> input;
      input.push_back(static_cast<uint8_t>(i));
      std::string encoded = ome::common::base64_encode(input.begin(), input.end());
      std::vector<uint8_t> decoded = ome::common::base64_decode<std::vector<uint8_t>>(encoded);

      ASSERT_EQ(input, decoded);
    }
}

Base64TestParameters params[] =
  {
    Base64TestParameters("Test Base64 Encoding",
                         "VGVzdCBCYXNlNjQgRW5jb2Rpbmc=",
                         "VGVzdCBCYXNlNjQgRW5jb2Rpbmc="),
    Base64TestParameters("Test\nBase64\nEncoding",
                         "VGVzdApCYXNlNjQKRW5jb2Rpbmc=",
                         "VGV\nzdApC\nYXNlNjQKRW5j\nb2Rpbmc="),
    Base64TestParameters("Test padding",
                         "VGVzdCBwYWRkaW5n",
                         "VGVzdCB\nwYWRkaW5n"),
    Base64TestParameters("Test padding-",
                         "VGVzdCBwYWRkaW5nLQ==",
                         "VGVzdCBwYWRkaW5nLQ=="),
    Base64TestParameters("Test padding--",
                         "VGVzdCBwYWRkaW5nLS0=",
                         "VGVzdCBwYWRkaW5nLS0="),
    Base64TestParameters("Lorem ipsum dolor sit amet, consectetur adipiscing "
                         "elit, sed do eiusmod tempor incididunt ut labore et "
                         "dolore magna aliqua. Ut enim ad minim veniam, quis "
                         "nostrud exercitation ullamco laboris nisi ut aliquip "
                         "ex ea commodo consequat. Duis aute irure dolor in "
                         "reprehenderit in voluptate velit esse cillum dolore "
                         "eu fugiat nulla pariatur. Excepteur sint occaecat "
                         "cupidatat non proident, sunt in culpa qui officia "
                         "deserunt mollit anim id est laborum.",
                         "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZWxpdCwg\n"
                         "c2VkIGRvIGVpdXNtb2QgdGVtcG9yIGluY2lkaWR1bnQgdXQgbGFib3JlIGV0IGRvbG9yZSBtYWdu\n"
                         "YSBhbGlxdWEuIFV0IGVuaW0gYWQgbWluaW0gdmVuaWFtLCBxdWlzIG5vc3RydWQgZXhlcmNpdGF0\n"
                         "aW9uIHVsbGFtY28gbGFib3JpcyBuaXNpIHV0IGFsaXF1aXAgZXggZWEgY29tbW9kbyBjb25zZXF1\n"
                         "YXQuIER1aXMgYXV0ZSBpcnVyZSBkb2xvciBpbiByZXByZWhlbmRlcml0IGluIHZvbHVwdGF0ZSB2\n"
                         "ZWxpdCBlc3NlIGNpbGx1bSBkb2xvcmUgZXUgZnVnaWF0IG51bGxhIHBhcmlhdHVyLiBFeGNlcHRl\n"
                         "dXIgc2ludCBvY2NhZWNhdCBjdXBpZGF0YXQgbm9uIHByb2lkZW50LCBzdW50IGluIGN1bHBhIHF1\n"
                         "aSBvZmZpY2lhIGRlc2VydW50IG1vbGxpdCBhbmltIGlkIGVzdCBsYWJvcnVtLg==",
                         "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZWxpdCwg"
                         "c2VkIGRvIGVpdXNtb2QgdGVtcG9yIGluY2lkaWR1bnQgdXQgbGFib3JlIGV0IGRvbG9yZSBtYWdu"
                         "YSBhbGlxdWEuIFV0IGVuaW0gYWQgbWluaW0gdmVuaWFtLCBxdWlzIG5vc3RydWQgZXhlcmNpdGF0"
                         "aW9uIHVsbGFtY28gbGFib3JpcyBuaXNpIHV0IGFsaXF1aXAgZXggZWEgY29tbW9kbyBjb25zZXF1"
                         "YXQuIER1aXMgYXV0ZSBpcnVyZSBkb2xvciBpbiByZXByZWhlbmRlcml0IGluIHZvbHVwdGF0ZSB2"
                         "ZWxpdCBlc3NlIGNpbGx1bSBkb2xvcmUgZXUgZnVnaWF0IG51bGxhIHBhcmlhdHVyLiBFeGNlcHRl"
                         "dXIgc2ludCBvY2NhZWNhdCBjdXBpZGF0YXQgbm9uIHByb2lkZW50LCBzdW50IGluIGN1bHBhIHF1"
                         "aSBvZmZpY2lhIGRlc2VydW50IG1vbGxpdCBhbmltIGlkIGVzdCBsYWJvcnVtLg==")
  };

// Disable missing-prototypes warning for INSTANTIATE_TEST_CASE_P;
// this is solely to work around a missing prototype in gtest.
#ifdef __GNUC__
#  if defined __clang__ || defined __APPLE__
#    pragma GCC diagnostic ignored "-Wmissing-prototypes"
#  endif
#  pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

INSTANTIATE_TEST_CASE_P(Base64Variants, Base64Test, ::testing::ValuesIn(params));
