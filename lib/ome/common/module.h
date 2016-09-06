/*
 * #%L
 * OME-COMMON C++ library for C++ compatibility/portability
 * %%
 * Copyright © 2006 - 2016 Open Microscopy Environment:
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

/**
 * @file ome/common/module.h Module loading and introspection.
 *
 * This header defines functions for introspecting the runtime
 * location of modules.
 */

#ifndef OME_COMMON_MODULE_H
#define OME_COMMON_MODULE_H

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <ome/common/config.h>

/**
 * Open Microscopy Environment C++.
 */
namespace ome
{
  namespace common
  {

    /**
     * Get the runtime installation prefix path for a module.
     *
     * This is intended primarily for internal use, to allow discovery
     * of the location of datafiles, loadable modules, etc.  However,
     * it may be freely used by additional components, both OME and
     * third-party, to register paths.
     *
     * @param dtype the directory type to query.
     * @returns the installation prefix path.
     * @throws a @c std::runtime_error if the path could not be
     * determined.
     */
    const boost::filesystem::path&
    module_runtime_path(const std::string& dtype);

    /**
     * Register OME-Common module paths.
     *
     * This function forces path registration.
     *
     * @note This is a hack to allow static linking to work on
     * Windows; without this, the module object is omitted and the
     * paths aren't automatically registered.  This will no longer be
     * required once it is built as a DLL.  Its only purpose is to
     * force object inclusion when static linking, and ensure that the
     * registration happens independently of object static
     * construction order to allow use prior to main() entry.  You
     * should not use this.
     */
    void
    register_module_paths();

    /**
     * A run-time path for a given module.
     *
     * This is used to find the location of in installation path at
     * runtime, handling relocatable installs by introspecting the
     * installation prefix and also by allowing overriding of the path
     * by an environment variable.
     */
    struct Module
    {
      /// Name of the path, e.g. "bin" or "ome-xml-schema".
      std::string name;
      /// Name of the environment variable used to override the
      /// autodetected path.
      std::string envvar;
      /// Name of the environment variable used to specify the
      /// module installation root.
      std::string module_envvar;
      /// Name of the environment variable used to specify the
      /// installation root.
      std::string root_envvar;
      /// Absolute path (used when configured to use an absolute
      /// install path).
      boost::filesystem::path abspath;
      /// Relative path (used for relocatable installs).
      boost::filesystem::path relpath;
      /// Absolute installation path (used for non-relocatable installs).
      boost::filesystem::path install_prefix;
      /// Shared library path (used for relocatable installs).
      boost::filesystem::path shlibpath;
      /// The detected path (used to cache search result).
      boost::filesystem::path realpath;
      /// Function to obtain the absolute path of the module providing
      /// the path (from the shared library or DLL); this won't work
      /// when static libraries are in use.  Used to introspect the
      /// installation path.
      boost::filesystem::path (*module_path)();

      /**
       * Constructor.
       *
       * @param name the name of the module path.
       * @param envvar the environment variable to override the path.
       * @param module_envvar the environment variable to override the
       * module installation path.
       * @param root_envvar the environment variable to override the
       * root installation path.
       * @param abspath the absolute path.
       * @param relpath the relative path.
       * @param install_prefix the absolute installation path.
       * @param shlibpath the absolute path to the shared library.
       * @param module_path a function pointer to provide the module
       * installation path, or null to skip introspection.
       */
      Module(const std::string&               name,
             const std::string&               envvar,
             const std::string&               module_envvar,
             const std::string&               root_envvar,
             const boost::filesystem::path&   abspath,
             const boost::filesystem::path&   relpath,
             const boost::filesystem::path&   install_prefix,
             const boost::filesystem::path&   shlibpath,
             boost::filesystem::path        (*module_path)());
    };

    /**
     * Register a module to make it available to module_runtime_path().
     *
     * The arguments are used to construct a Module object and insert
     * it into a map for module lookups.
     */
    struct RegisterModule
    {
      /// Name of the path, e.g. "bin" or "ome-xml-schema".
      std::string name;
      /// Is the path registered in the path map?
      bool registered;

      /**
       * Constructor.
       *
       * Register the named module.
       *
       * @param name the name of the module path.
       * @param envvar the environment variable to override the path.
       * @param module_envvar the environment variable to override the
       * module installation path.
       * @param root_envvar the environment variable to override the
       * root path.
       * @param abspath the absolute path.
       * @param relpath the relative path.
       * @param install_prefix the absolute installation path.
       * @param shlibpath the absolute path to the shared library.
       * @param module_path a function pointer to provide the module
       * installation path.
       */
      RegisterModule(const std::string&               name,
                     const std::string&               envvar,
                     const std::string&               module_envvar,
                     const std::string&               root_envvar,
                     const boost::filesystem::path&   abspath,
                     const boost::filesystem::path&   relpath,
                     const boost::filesystem::path&   install_prefix,
                     const boost::filesystem::path&   shlibpath,
                     boost::filesystem::path        (*module_path)());

      /**
       * Destructor.
       *
       * Unregister the named module.
       */
      ~RegisterModule();
    };
  }
}

// Set to include introspection functionality (used for registering
// paths; not for normal use).
#ifdef OME_COMMON_MODULE_INTROSPECTION

#ifdef OME_HAVE_DLADDR
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
#include <dlfcn.h>
#endif // OME_HAVE_DLADDR

#ifdef _MSC_VER
# include <windows.h>
#endif

namespace
{

#ifdef OME_HAVE_DLADDR
  Dl_info this_module;

  __attribute__((constructor))
  void
  find_module(void)
  {
    if(!dladdr(reinterpret_cast<void *>(find_module), &this_module))
      {
        this_module.dli_fname = 0;
      }
  }

  boost::filesystem::path
  module_path()
  {
    if (this_module.dli_fname)
      return boost::filesystem::canonical(boost::filesystem::path(this_module.dli_fname));
    return boost::filesystem::path();
  }
#elif _MSC_VER
  HMODULE
  find_module(void)
  {
    static bool found_module = false;
    static HMODULE this_module;

    if (!found_module)
      {
        if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                reinterpret_cast<LPCWSTR>(&find_module),
                                &this_module))
          {
            this_module = 0;
          }
        found_module = true;
      }
    return this_module;
  }

  boost::filesystem::path
  module_path()
  {
    HMODULE this_module = find_module();
    if (this_module)
      {
        WCHAR win_wide_path[MAX_PATH];
        GetModuleFileNameW(this_module, win_wide_path, sizeof(win_wide_path));
        return boost::filesystem::path(win_wide_path);
      }
    return boost::filesystem::path();
  }
#else // No introspection available
  boost::filesystem::path
  module_path()
  {
    return boost::filesystem::path();
  }
#endif // _MSC_VER
}
#endif // OME_COMMON_MODULE_INTROSPECTION

#endif // OME_COMMON_MODULE_H

/*
 * Local Variables:
 * mode:C++
 * End:
 */
