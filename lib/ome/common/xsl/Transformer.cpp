/*
 * #%L
 * OME-XALAN C++ library for working with Xalan C++.
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

#include <sstream>
#include <stdexcept>

#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>

#include <ome/common/xml/dom/Document.h>
#include <ome/common/xsl/Transformer.h>

namespace
{

  // Input policy template.
  template<typename T>
  struct Input
  {
  };

  // Input policy for files.
  template<>
  struct Input<boost::filesystem::path>
  {
    boost::filesystem::ifstream stream;
    xalanc::XSLTInputSource source;

    Input(const boost::filesystem::path& path):
      stream(path),
      source(stream)
    {
      if (!stream)
        {
          boost::format fmt("%1%: Invalid file for XSL transform");
          fmt % path;
          throw std::runtime_error(fmt.str());
        }
      source.setSystemId(ome::common::xml::String(path.string()));
    }
  };

  // Input policy for strings.
  template<>
  struct Input<std::string>
  {
    std::istringstream stream;
    xalanc::XSLTInputSource source;

    Input(const std::string& text):
      stream(text),
      source(stream)
    {
      if (text.empty())
        {
          throw std::runtime_error("Invalid stream for XSL transform");
        }
    }
  };

  // Input policy for streams.
  template<>
  struct Input<std::istream>
  {
    xalanc::XSLTInputSource source;

    Input(std::istream& stream):
      source(&stream)
    {
      if (!stream)
        {
          throw std::runtime_error("Invalid stream for XSL transform");
        }
    }
  };

  // Output policy template.
  template<typename T>
  struct Output
  {
  };

  // Output policy for files.
  template<>
  struct Output<boost::filesystem::path>
  {
    boost::filesystem::ofstream stream;
    xalanc::XSLTResultTarget dest;

    Output(const boost::filesystem::path& path):
      stream(path),
      dest(stream)
    {
      if (!stream)
        {
          boost::format fmt("%1%: Invalid file for XSL transform");
          fmt % path;
          throw std::runtime_error(fmt.str());
        }
    }
  };

  // Output policy for strings.
  template<>
  struct Output<std::string>
  {
    std::string& string;
    std::ostringstream stream;
    xalanc::XSLTResultTarget dest;

    Output(std::string& string):
      string(string),
      stream(),
      dest(stream)
    {
    }

    ~Output()
    {
      // Assign destination string on destruction.
      string = stream.str();
    }
  };

  // Output policy for streams.
  template<>
  struct Output<std::ostream>
  {
    xalanc::XSLTResultTarget dest;

    Output(std::ostream& stream):
      dest(stream)
    {
      if (!stream)
        {
          throw std::runtime_error("Invalid stream for XSL transform");
        }
    }
  };

  // Input policy for XSL.  Same as Input, above, but checks XSL is well-formed.
  // Output policy template.
  template<typename T>
  struct XSLInput : public Input<T>
  {
  };

  // Input policy for XSL files.
  template<>
  struct XSLInput<boost::filesystem::path> : public Input<boost::filesystem::path>
  {
    XSLInput(const boost::filesystem::path&    path,
             ome::common::xml::EntityResolver *resolver):
      Input<boost::filesystem::path>(path)
    {
      if (resolver)
        {
          // XSLT 1.0 doesn't have an official XML Schema to validate
          // against, so disable schema checking.  This will still check that
          // the XSL is well-formed.
          ome::common::xml::dom::ParseParameters pp;
          pp.doSchema = false;
          ome::common::xml::dom::Document
            (ome::common::xml::dom::createDocument(path, *resolver, pp));
        }
      else
        {
          throw std::runtime_error("No entity resolver registered");
        }
    }
  };

}

namespace ome
{
  namespace common
  {
    namespace xsl
    {

      Transformer::Transformer():
        transformer(),
        resolver()
      {
      }

      Transformer::~Transformer()
      {
      }

      xml::EntityResolver *
      Transformer::getEntityResolver() const
      {
        return resolver;
      }

      void
      Transformer::setEntityResolver(xml::EntityResolver *resolver)
      {
        this->resolver = resolver;
        transformer.setXMLEntityResolver(&*this->resolver);
      }

      bool
      Transformer::getUseValidation() const
      {
        return transformer.getUseValidation();
      }

      void
      Transformer::setUseValidation(bool validate)
      {
        transformer.setUseValidation(validate);
      }

      void
      Transformer::transform(xalanc::XSLTInputSource&  xsl,
                             xalanc::XSLTInputSource&  input,
                             xalanc::XSLTResultTarget& output)
      {
        if (transformer.transform(input, xsl, output) != 0)
          {
            throw std::runtime_error("XSL transform failed");
          }
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             const boost::filesystem::path& input,
                             const boost::filesystem::path& output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<boost::filesystem::path> in(input);
        Output<boost::filesystem::path> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             std::istream&                  input,
                             const boost::filesystem::path& output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<std::istream> in(input);
        Output<boost::filesystem::path> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             const std::string&             input,
                             const boost::filesystem::path& output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<std::string> in(input);
        Output<boost::filesystem::path> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             const boost::filesystem::path& input,
                             std::ostream&                  output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<boost::filesystem::path> in(input);
        Output<std::ostream> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             std::istream&                  input,
                             std::ostream&                  output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<std::istream> in(input);
        Output<std::ostream> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             const std::string&             input,
                             std::ostream&                  output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<std::string> in(input);
        Output<std::ostream> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             const boost::filesystem::path& input,
                             std::string&                   output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<boost::filesystem::path> in(input);
        Output<std::string> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             std::istream&                  input,
                             std::string&                   output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<std::istream> in(input);
        Output<std::string> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

      void
      Transformer::transform(const boost::filesystem::path& xsl,
                             const std::string&             input,
                             std::string&                   output)
      {
        XSLInput<boost::filesystem::path> xsl_in(xsl, resolver);
        Input<std::string> in(input);
        Output<std::string> out(output);

        transform(xsl_in.source, in.source, out.dest);
      }

    }
  }
}
