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

#ifndef OME_COMMON_XSL_PLATFORM_H
#define OME_COMMON_XSL_PLATFORM_H

#include <cstdint>

#include <boost/thread.hpp>

#include <ome/common/xml/Platform.h>

#include <xalanc/XalanTransformer/XalanTransformer.hpp>

namespace ome
{
  namespace common
  {
    /**
     * Xalan-C modern C++ wrapper.  All classes in this namespace wrap
     * the Xalan-C classes and functions to provide RAII and
     * exception-safe equivalents, and which also handle memory
     * management transparently.
     */
    namespace xsl
    {

      /**
       * XSL Platform.  This class wraps calls to the
       * xalanc::Transformer initialize() and terminate()
       * functions, to allow their use in an exception-safe manner.
       * Create an instance of this class prior to performing any work
       * with Xalan, and ensure it will remain in scope for all work to
       * complete.  When the scope is exited, or an exception is thrown,
       * Xalan will be automatically terminated.  Any number of
       * instances of this class may be created; Xalan will only be
       * terminated when the last instance is destroyed.
       *
       * Internally, it will also initialize and terminate the
       * Xerces-C++ Platform using the xml::Platform wrapper.
       */
      class Platform
      {
      public:
        /**
         * Construct a Platform.  Calls xalanc::Transformer::initialize().
         *
         * @param skip Skip calls to xalanc::Transformer::initialize
         * and xalanc::Transformer::terminate.  Use if other code
         * already called initialize; this will make Platform usage in
         * other code skip these operations, making it a null
         * operation.
         */
        Platform(bool skip = false):
	  xmlplatform(),
          skip(skip)
        {
	  boost::lock_guard<boost::mutex> lock(mutex);

          // Only call initialize for first instance.
          if (refcount == 0 && !skip)
            xalanc::XalanTransformer::initialize();
          ++refcount;
        }

        /**
         * Destructor. Calls xalanc::Transformer::terminate().
         */
        ~Platform()
        {
	  boost::lock_guard<boost::mutex> lock(mutex);

          // Only call terminate for last instance.
          // refcount will never be zero at this point.
          if (refcount == 1 && !skip)
            xalanc::XalanTransformer::terminate();
          --refcount;
        }

      private:
	/// Xerces-C++ platform.
	xml::Platform xmlplatform;
        /// Skip initialize and terminate calls.
        bool skip;
        /// Mutex to lock libxalan access.
        static boost::mutex mutex;
        /// Reference count.
        static uint32_t refcount;
      };

    }
  }
}

#endif // OME_COMMON_XSL_PLATFORM_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
