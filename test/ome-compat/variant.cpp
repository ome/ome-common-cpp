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

#include <ome/compat/variant.h>

#include <ome/test/test.h>

#include <cstdint>
#include <string>

typedef ome::compat::variant<int,double,std::string> var;

TEST(Variant, Create)
{
  var v;
}

TEST(Variant, SetInt)
{
  var v1(int(32354));
  ASSERT_EQ(32354, ome::compat::get<int>(v1));

  var v2;
  v2 = int(32354);
  ASSERT_EQ(32354, ome::compat::get<int>(v2));
}

TEST(Variant, SetString)
{
  var v1(std::string("test"));
  ASSERT_EQ(std::string("test"), ome::compat::get<std::string>(v1));

  var v2;
  v2 = std::string("test");
  ASSERT_EQ(std::string("test"), ome::compat::get<std::string>(v2));
}

class test_visitor
{
public:
  template<typename T>
  void
  operator()(const T& value)
  {
    std::cout << value << std::endl;
  }
};

TEST(Variant, ApplyStaticVisitor)
{
  test_visitor visitor;
  var v;

  v = 32;
  ome::compat::visit(visitor, v);

  v = std::string("V");
  ome::compat::visit(visitor, v);
}

typedef ome::compat::variant<std::string, bool> non_numeric_variant;

TEST(Variant, NonNumeric)
{
  non_numeric_variant v1(std::string("String value"));
  ASSERT_EQ(std::string("String value"), ome::compat::get<std::string>(v1));
  non_numeric_variant v2(false);
  ASSERT_FALSE(ome::compat::get<bool>(v2));
}

typedef ome::compat::variant<uint8_t,
                             uint16_t,
                             uint32_t,
                             uint64_t,
                             int8_t,
                             int16_t,
                             int32_t,
                             int64_t> integer_variant;

TEST(Variant, Integer)
{
  integer_variant v1(uint64_t(238220U));
  ASSERT_EQ(uint64_t(238220U), ome::compat::get<uint64_t>(v1));
  integer_variant v2(int16_t(432));
  ASSERT_EQ(int16_t(432), ome::compat::get<int16_t>(v2));
}
