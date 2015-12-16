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

#include <ome/common/config-internal.h>
#include <ome/common/module.h>

#include <ome/test/test.h>

class ModulePathTestParameters
{
public:
  std::string dtype;
  std::string envvar;
  bool        logic_error;

  ModulePathTestParameters(std::string dtype,
                           std::string envvar,
                           bool        logic_error):
    dtype(dtype),
    envvar(envvar),
    logic_error(logic_error)
  {}
};

class ModulePathTest : public ::testing::TestWithParam<ModulePathTestParameters>
{
};

TEST_P(ModulePathTest, CheckPath)
{
  const ModulePathTestParameters& params = GetParam();

  // This won't necessarily work in the build tree, so catch any
  // exception.  It will only error on segfaults.
  try
    {
      boost::filesystem::path path(ome::common::module_runtime_path(params.dtype));
      if (verbose())
        std::cout << params.dtype << " path is: " << path << '\n';
    }
  catch (const std::runtime_error& e)
    {
      if (verbose())
        std::cout << params.dtype << " threw a runtime_error: " << e.what() << '\n';
      ASSERT_FALSE(params.logic_error);
    }
  catch (const std::logic_error& e)
    {
      if (verbose())
        std::cout << params.dtype << " threw a logic_error: " << e.what() << '\n';
      ASSERT_TRUE(params.logic_error);
    }
}

TEST_P(ModulePathTest, ValidEnv)
{
  const ModulePathTestParameters& params = GetParam();

#ifdef _MSC_VER
  _putenv_s(params.envvar.c_str(), PROJECT_BINARY_DIR);
#else
  setenv(params.envvar.c_str(), PROJECT_BINARY_DIR, 1);
#endif

  if (!params.logic_error)
    ASSERT_NO_THROW(ome::common::module_runtime_path(params.dtype));
  else
    ASSERT_THROW(ome::common::module_runtime_path(params.dtype), std::logic_error);
}

TEST_P(ModulePathTest, InvalidEnv)
{
  const ModulePathTestParameters& params = GetParam();

#ifdef _MSC_VER
  _putenv_s(params.envvar.c_str(), PROJECT_BINARY_DIR "/invalid-path");
#else
  setenv(params.envvar.c_str(), PROJECT_BINARY_DIR "/invalid-path", 1);
#endif

  try
    {
      ome::common::module_runtime_path(params.dtype);
    }
  catch (const std::runtime_error& e)
    {
      if (verbose())
        std::cout << params.dtype << " threw a runtime_error: " << e.what() << '\n';
      ASSERT_FALSE(params.logic_error);
    }
  catch (const std::logic_error& e)
    {
      if (verbose())
        std::cout << params.dtype << " threw a logic_error: " << e.what() << '\n';
      ASSERT_TRUE(params.logic_error);
    }
}

const std::vector<ModulePathTestParameters> params =
  {
    {"bin",             "OME_BINDIR",                false},
    {"sbin",            "OME_SBINDIR",               false},
    {"libexec",         "OME_LIBEXECDIR",            false},
    {"sysconf",         "OME_SYSCONFDIR",            false},
    {"sharedstate",     "OME_SHAREDSTATEDIR",        false},
    {"localstate",      "OME_LOCALSTATEDIR",         false},
    {"lib",             "OME_LIBDIR",                false},
    {"include",         "OME_INCLUDEDIR",            false},
    {"oldinclude",      "OME_OLDINCLUDEDIR",         false},
    {"dataroot",        "OME_DATAROOTDIR",           false},
    {"data",            "OME_DATADIR",               false},
    {"info",            "OME_INFODIR",               false},
    {"locale",          "OME_LOCALEDIR",             false},
    {"man",             "OME_MANDIR",                false},
    {"doc",             "OME_DOCDIR",                false},

    {"ome-common-root", "OME_HOME",                  false},

    {"bin",             "OME_COMMON_BINDIR",         false},
    {"sbin",            "OME_COMMON_SBINDIR",        false},
    {"libexec",         "OME_COMMON_LIBEXECDIR",     false},
    {"sysconf",         "OME_COMMON_SYSCONFDIR",     false},
    {"sharedstate",     "OME_COMMON_SHAREDSTATEDIR", false},
    {"localstate",      "OME_COMMON_LOCALSTATEDIR",  false},
    {"lib",             "OME_COMMON_LIBDIR",         false},
    {"include",         "OME_COMMON_INCLUDEDIR",     false},
    {"oldinclude",      "OME_COMMON_OLDINCLUDEDIR",  false},
    {"dataroot",        "OME_COMMON_DATAROOTDIR",    false},
    {"data",            "OME_COMMON_DATADIR",        false},
    {"info",            "OME_COMMON_INFODIR",        false},
    {"locale",          "OME_COMMON_LOCALEDIR",      false},
    {"man",             "OME_COMMON_MANDIR",         false},
    {"doc",             "OME_COMMON_DOCDIR",         false},

    {"ome-common-root", "OME_COMMON_HOME",           false},

    // Invalid dtype throws logic_error
    {"ome-files-invalid", "OME_INVALID",             true}
  };

// Disable missing-prototypes warning for INSTANTIATE_TEST_CASE_P;
// this is solely to work around a missing prototype in gtest.
#ifdef __GNUC__
#  if defined __clang__ || defined __APPLE__
#    pragma GCC diagnostic ignored "-Wmissing-prototypes"
#  endif
#  pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

INSTANTIATE_TEST_CASE_P(ModulePathVariants, ModulePathTest, ::testing::ValuesIn(params));
