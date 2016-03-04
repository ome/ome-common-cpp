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

#include <boost/filesystem/fstream.hpp>

#include <ome/common/xml/EntityResolver.h>
#include <ome/common/xsl/Platform.h>
#include <ome/common/xsl/Transformer.h>

#include <ome/test/config.h>

#include <ome/test/test.h>
#include <ome/test/io.h>

#include <fstream>
#include <stdexcept>
#include <vector>

namespace xml = ome::common::xml;
namespace xsl = ome::common::xsl;

template<typename T>
struct Input
{
};

template<>
struct Input<boost::filesystem::path>
{
  std::string type;
  boost::filesystem::path path;
  boost::filesystem::path input;

  Input(const boost::filesystem::path& path):
    type("path"),
    path(path),
    input(path)
  {
  }

  std::string
  str()
  {
    std::string ret;
    readFile(path, ret);
    return ret;
  }
};

template<>
struct Input<std::string>
{
  std::string type;
  boost::filesystem::path path;
  std::string input;

  Input(const boost::filesystem::path& path):
    type("string"),
    path(path),
    input()
  {
    try
      {
        // Can throw if the file does not exist.  We just want the
        // empty string for this step.
        readFile(path, input);
      }
    catch (const std::runtime_error&)
      {
      }
  }

  std::string
  str()
  {
    return input;
  }
};

template<>
struct Input<std::istream>
{
  std::string type;
  boost::filesystem::path path;
  boost::filesystem::ifstream input;

  Input(const boost::filesystem::path& path):
    type("stream"),
    path(path),
    input(path)
  {
  }

  std::string
  str()
  {
    std::string ret;
    readFile(path, ret);
    return ret;
  }
};

template<typename T>
struct Output
{
};

template<>
struct Output<boost::filesystem::path>
{
  std::string type;
  boost::filesystem::path path;
  boost::filesystem::path output;

  Output(const boost::filesystem::path& path):
    type("path"),
    path(path),
    output(path)
  {
  }

  std::string
  str()
  {
    std::string ret;
    readFile(path, ret);
    return ret;
  }
};

template<>
struct Output<std::string>
{
  std::string type;
  boost::filesystem::path path;
  std::string output;

  Output(const boost::filesystem::path& path):
    type("string"),
    path(path),
    output()
  {
  }

  std::string
  str()
  {
    return output;
  }
};

template<>
struct Output<std::ostream>
{
  std::string type;
  boost::filesystem::path path;
  boost::filesystem::ofstream output;

  Output(const boost::filesystem::path& path):
    type("stream"),
    path(path),
    output(path)
  {
  }

  std::string
  str()
  {
    if (output)
      {
        std::flush(output);
        output.close();
      }

    std::string ret;
    readFile(path, ret);
    return ret;
  }
};

template <typename T>
class XalanTest : public ::testing::Test
{
public:
  xsl::Platform plat;
  xml::EntityResolver resolver;
  boost::filesystem::path xsl;
  boost::filesystem::path xsl_invalid;
  boost::filesystem::path xsl_invalid2;
  boost::filesystem::path source;
  boost::filesystem::path source_invalid1;
  boost::filesystem::path source_invalid2;
  boost::filesystem::path source_invalid3;
  boost::filesystem::path dest;
  boost::filesystem::path reference;

  virtual void SetUp()
  {
    resolver.registerCatalog(boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/schema/catalog.xml"));

    xsl = boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/2012-06-to-2013-06.xsl");
    xsl_invalid = boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/2012-06-to-2013-06-invalid.xsl");
    xsl_invalid2 = boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/2012-06-to-2013-06-nonexistent.xsl");
    source = boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/18x24y5z5t2c8b-text.ome");
    source_invalid1 = boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/18x24y5z5t2c8b-text-invalid.ome");
    source_invalid2 = boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/18x24y5z5t2c8b-text-invalid2.ome");
    source_invalid3 = boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/18x24y5z5t2c8b-text-nonexistent.ome");
    dest = boost::filesystem::path(PROJECT_BINARY_DIR "/test/ome-common/data/18x24y5z5t2c8b-text-2013.ome");
    reference = boost::filesystem::path(PROJECT_SOURCE_DIR "/test/ome-common/data/18x24y5z5t2c8b-text-2013-expected.ome");
  }

  void TestTransform(const boost::filesystem::path& xsl,
                     const boost::filesystem::path& infile,
                     const boost::filesystem::path& outfile,
                     bool transformPass,
                     bool comparePass,
                     bool validate)
  {
    xsl::Transformer t;
    t.setUseValidation(validate);
    t.setEntityResolver(&resolver);

    Input<typename T::from_type> input(infile);
    Output<typename T::to_type> output(outfile);

    std::cout << "Testing transform of " << input.type
              << " to " << output.type
              << " using " << infile
              << "and " << xsl << '\n';

    if (!transformPass)
      {
        if (validate)
          {
            ASSERT_THROW(t.transform(xsl, input.input, output.output),
                         std::runtime_error);
          }
      }
    else
      {
        ASSERT_NO_THROW(t.transform(xsl, input.input, output.output));

        std::string transform_text = output.str();
        std::string reference_text;
        ASSERT_NO_THROW(readFile(this->reference, reference_text));

	bool substitute_cr = false;
#if defined(XALAN_NEWLINE_IS_CRLF)
	  substitute_cr = true;
#endif
	if (substitute_cr)
	  {
	    std::string tmp;
	    for (std::string::const_iterator i = reference_text.begin();
		 i != reference_text.end();
		 ++i)
	      {
		if (*i == '\n')
		  tmp += '\r';
		tmp += *i;
	      }
	    reference_text = tmp;
	  }

        ASSERT_FALSE(transform_text.empty());
        ASSERT_FALSE(reference_text.empty());
        if (comparePass)
          {
            if (validate)
              {
                ASSERT_EQ(reference_text, transform_text);
              }
          }
        else
          {
              ASSERT_NE(reference_text, transform_text);
          }
      }
  }

};

TYPED_TEST_CASE_P(XalanTest);

TYPED_TEST_P(XalanTest, Platform)
{
}

TYPED_TEST_P(XalanTest, PlatformRefCount)
{
  xsl::Platform plat1(false);
  xsl::Platform plat2;
  xsl::Platform plat3;
}

TYPED_TEST_P(XalanTest, TransformConstruct)
{
  xsl::Transformer t;
}

TYPED_TEST_P(XalanTest, TransformSetEntityResolver)
{
  xsl::Transformer t;

  ASSERT_NE(&this->resolver, t.getEntityResolver());

  t.setEntityResolver(&this->resolver);

  ASSERT_EQ(&this->resolver, t.getEntityResolver());
}

TYPED_TEST_P(XalanTest, TransformApply)
{
  this->TestTransform(this->xsl, this->source, this->dest,
                      true, true, true);
  this->TestTransform(this->xsl, this->source, this->dest,
                      true, true, false);
}

TYPED_TEST_P(XalanTest, TransformApplyInvalidInput1)
{
  this->TestTransform(this->xsl, this->source_invalid1, this->dest,
                      false, false, true);
  this->TestTransform(this->xsl, this->source_invalid1, this->dest,
                      false, false, false);
}

TYPED_TEST_P(XalanTest, TransformApplyInvalidInput2)
{
  this->TestTransform(this->xsl, this->source_invalid2, this->dest,
                      false, false, true);
  this->TestTransform(this->xsl, this->source_invalid2, this->dest,
                      false, false, false);
}

TYPED_TEST_P(XalanTest, TransformApplyInvalidInput3)
{
  this->TestTransform(this->xsl, this->source_invalid3, this->dest,
                      false, false, true);
  this->TestTransform(this->xsl, this->source_invalid3, this->dest,
                      false, false, false);
}

TYPED_TEST_P(XalanTest, TransformApplyInvalidXSL1)
{
  this->TestTransform(this->xsl_invalid, this->source, this->dest,
                      false, false, true);
  this->TestTransform(this->xsl_invalid, this->source, this->dest,
                      false, false, false);
}

TYPED_TEST_P(XalanTest, TransformApplyInvalidXSL2)
{
  this->TestTransform(this->xsl_invalid2, this->source, this->dest,
                      false, false, true);
  this->TestTransform(this->xsl_invalid2, this->source, this->dest,
                      false, false, false);
}

// Xalan initialised externally.
TEST(XalanSkipTest, PlatformRefCountSkipInit)
{
  xml::Platform xmlplatform;
  xalanc::XalanTransformer::initialize();
  xsl::Platform plat1(true);
  xsl::Platform plat2;
  xsl::Platform plat3;
  xalanc::XalanTransformer::terminate();
}

REGISTER_TYPED_TEST_CASE_P(XalanTest,
                           Platform,
                           PlatformRefCount,
                           TransformConstruct,
                           TransformSetEntityResolver,
                           TransformApply,
                           TransformApplyInvalidInput1,
                           TransformApplyInvalidInput2,
                           TransformApplyInvalidInput3,
                           TransformApplyInvalidXSL1,
                           TransformApplyInvalidXSL2);

template<typename From, typename To>
struct TestTypes
{
  typedef From from_type;
  typedef To to_type;
};

typedef ::testing::Types<
  TestTypes<boost::filesystem::path, boost::filesystem::path>,
  TestTypes<std::istream,            boost::filesystem::path>,
  TestTypes<std::string,             boost::filesystem::path>,
  TestTypes<boost::filesystem::path, std::ostream>,
  TestTypes<std::istream,            std::ostream>,
  TestTypes<std::string,             std::ostream>,
  TestTypes<boost::filesystem::path, std::string>,
  TestTypes<std::istream,            std::string>,
  TestTypes<std::string,             std::string>
  > TransformTestTypes;

INSTANTIATE_TYPED_TEST_CASE_P(XalanTransformTypes, XalanTest, TransformTestTypes);
