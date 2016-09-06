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

#ifndef OME_COMMON_XSL_TRANSFORMER_H
#define OME_COMMON_XSL_TRANSFORMER_H

#include <boost/filesystem/path.hpp>

#include <ome/compat/memory.h>

#include <ome/common/xml/EntityResolver.h>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

namespace ome
{
  namespace common
  {
    namespace xsl
    {

      /**
       * XSL Transformer.  This class wraps calls to the
       * xalanc::Transformer transform functions, to allow their use
       * in a safe manner.  Create an instance of this class prior to
       * performing any work with Xalan, and ensure it will remain in
       * scope for all work to complete.  When the scope is exited, or
       * an exception is thrown, Xalan will be automatically
       * terminated.  Any number of instances of this class may be
       * created; Xalan will only be terminated when the last instance
       * is destroyed.
       */
      class Transformer
      {
      public:
        /**
         * Construct a Transformer instance.
         */
        Transformer();

        /**
         * Destructor.
         */
        ~Transformer();

        /**
         * Get the entity resolver in use.
         *
         * @returns the entity resolver.
         */
        xml::EntityResolver *
        getEntityResolver() const;

        /**
         * Set the entity resolver to use.
         *
         * This will be used to fetch the XML schemas required to
         * validate the XML documents being transformed.
         *
         * @param resolver the EntityResolver to use.
         */
        void
        setEntityResolver(xml::EntityResolver *resolver);

        /**
         * Check if validation is enabled.
         *
         * @returns @c true if enabled, @c false if disabled.
         */
        bool
        getUseValidation() const;

        /**
         * Enable or disable validation.
         *
         * Validation is disabled by default.
         *
         * @param validate @c true to enable validation, @c false to
         * disable.
         */
        void
        setUseValidation(bool validate);

        /**
         * Apply transform (XSLT abstract input and output).
         *
         * This generic method is called internally by all the other
         * transform() methods.  The other methods will be more
         * convenient to use, since they allow direct use of files,
         * streams and strings for both input and output.
         *
         * @param xsl the XSL transform to apply.
         * @param input the source XML to transform.
         * @param output where to store the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(xalanc::XSLTInputSource&  xsl,
                  xalanc::XSLTInputSource&  input,
                  xalanc::XSLTResultTarget& output);

        /**
         * Apply transform (file path to file path).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the file containing the XML to transform.
         * @param output the file to which to write the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  const boost::filesystem::path& input,
                  const boost::filesystem::path& output);

        /**
         * Apply transform (stream to file path).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the stream containing the XML to transform.
         * @param output the file to which to write the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  std::istream& input,
                  const boost::filesystem::path& output);

        /**
         * Apply transform (string to file path).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the string containing the XML to transform.
         * @param output the file to which to write the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  const std::string& input,
                  const boost::filesystem::path& output);

        /**
         * Apply transform (file path to stream).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the file containing the XML to transform.
         * @param output the stream to which to write the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  const boost::filesystem::path& input,
                  std::ostream& output);

        /**
         * Apply transform (stream to stream).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the stream containing the XML to transform.
         * @param output the stream to which to write the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  std::istream& input,
                  std::ostream& output);

        /**
         * Apply transform (string to stream).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the string containing the XML to transform.
         * @param output the stream to which to write the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  const std::string& input,
                  std::ostream& output);

        /**
         * Apply transform (path to string).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the file containing the XML to transform.
         * @param output the string in which to store the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  const boost::filesystem::path& input,
                  std::string& output);

        /**
         * Apply transform (stream to string).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the stream containing the XML to transform.
         * @param output the string in which to store the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  std::istream& input,
                  std::string& output);

        /**
         * Apply transform (string to string).
         *
         * @param xsl the XSL transform file to apply.
         * @param input the string containing the XML to transform.
         * @param output the string in which to store the result of the transformation.
         * @throws std::runtime_error on failure.
         */
        void
        transform(const boost::filesystem::path& xsl,
                  const std::string& input,
                  std::string& output);

      private:
        /// Xalan-C transformer being wrapped.
        xalanc::XalanTransformer transformer;
        /// EntityResolver to use with the transformer.
        xml::EntityResolver *resolver;
      };

    }
  }
}

#endif // OME_COMMON_XSL_TRANSFORMER_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
