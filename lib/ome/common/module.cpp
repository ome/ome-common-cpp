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

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <boost/range/size.hpp>

#include <ome/common/config-internal.h>

#define OME_COMMON_MODULE_INTROSPECTION 1
#include <ome/common/module.h>

#include <cstring>
#include <map>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace
{
  using ome::common::RegisterModule;

  typedef std::map<std::string, ome::common::Module> path_map;

  path_map&
  module_paths()
  {
    static path_map pmap;

    return pmap;
  }

  bool
  validate_path(const fs::path& path)
  {
    return (fs::exists(path) && fs::is_directory(path));
  }

  void register_paths()
  {
    // Global paths (not specific to any component)
    static RegisterModule bin
      ("bin",
       "OME_BINDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_BINDIR,
       OME_COMMON_INSTALL_BINDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule sbin
      ("sbin",
       "OME_SBINDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_SBINDIR,
       OME_COMMON_INSTALL_SBINDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule libexec
      ("libexec",
       "OME_LIBEXECDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_LIBEXECDIR,
       OME_COMMON_INSTALL_LIBEXECDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule sysconf
      ("sysconf",
       "OME_SYSCONFDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_SYSCONFDIR,
       OME_COMMON_INSTALL_SYSCONFDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule sharedstate
      ("sharedstate",
       "OME_SHAREDSTATEDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_SHAREDSTATEDIR,
       OME_COMMON_INSTALL_SHAREDSTATEDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule localstate
      ("localstate",
       "OME_LOCALSTATEDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_LOCALSTATEDIR,
       OME_COMMON_INSTALL_LOCALSTATEDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule lib
      ("lib",
       "OME_LIBDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_LIBDIR,
       OME_COMMON_INSTALL_LIBDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule include
      ("include",
       "OME_INCLUDEDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_INCLUDEDIR,
       OME_COMMON_INSTALL_INCLUDEDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule oldinclude
      ("oldinclude",
       "OME_OLDINCLUDEDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_OLDINCLUDEDIR,
       OME_COMMON_INSTALL_OLDINCLUDEDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule dataroot
      ("dataroot",
       "OME_DATAROOTDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_DATAROOTDIR,
       OME_COMMON_INSTALL_DATAROOTDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule data
      ("data",
       "OME_DATADIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_DATADIR,
       OME_COMMON_INSTALL_DATADIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule info
      ("info",
       "OME_INFODIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_INFODIR,
       OME_COMMON_INSTALL_INFODIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule locale
      ("locale",
       "OME_LOCALEDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_LOCALEDIR,
       OME_COMMON_INSTALL_LOCALEDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule man
      ("man",
       "OME_MANDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_MANDIR,
       OME_COMMON_INSTALL_MANDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    static RegisterModule doc
      ("doc",
       "OME_DOCDIR",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_FULL_DOCDIR,
       OME_COMMON_INSTALL_DOCDIR,
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);

    // OME-Common package-specific paths.
    static RegisterModule bf_root
      ("ome-common-root",
       "OME_COMMON_HOME",
       "OME_COMMON_HOME",
       "OME_HOME",
       OME_COMMON_INSTALL_PREFIX,
       "",
       OME_COMMON_INSTALL_PREFIX,
       OME_COMMON_SHLIBDIR,
       module_path);
  }

  struct AutoRegister
  {
    AutoRegister()
    {
      register_paths();
    }
  };

  AutoRegister path_register;

}

namespace ome
{
  namespace common
  {

    Module::Module(const std::string&               name,
                   const std::string&               envvar,
                   const std::string&               module_envvar,
                   const std::string&               root_envvar,
                   const boost::filesystem::path&   abspath,
                   const boost::filesystem::path&   relpath,
                   const boost::filesystem::path&   install_prefix,
                   const boost::filesystem::path&   shlibpath,
                   boost::filesystem::path        (*module_path)()):
      name(name),
      envvar(envvar),
      module_envvar(module_envvar),
      root_envvar(root_envvar),
      abspath(abspath),
      relpath(relpath),
      install_prefix(install_prefix),
      shlibpath(shlibpath),
      realpath(),
      module_path(module_path)
    {
    }

    RegisterModule::RegisterModule(const std::string&               name,
                                   const std::string&               envvar,
                                   const std::string&               module_envvar,
                                   const std::string&               root_envvar,
                                   const boost::filesystem::path&   abspath,
                                   const boost::filesystem::path&   relpath,
                                   const boost::filesystem::path&   install_prefix,
                                   const boost::filesystem::path&   shlibpath,
                                   boost::filesystem::path        (*module_path)()):
      name(name),
      registered(false)
    {
      Module m(name,
               envvar, module_envvar, root_envvar,
               abspath, relpath, install_prefix, shlibpath,
               module_path);
      path_map& map = module_paths();

      registered = map.insert(std::make_pair(name, m)).second;
    }

    RegisterModule::~RegisterModule()
    {
      if (registered)
        {
          path_map& map = module_paths();
          map.erase(name);
        }
    }

    /* TESTING NOTE
     * ────────────
     *
     * This code can't be unit tested since it is used after
     * installation.  This section documents the expected paths for
     * different configurations.
     *
     * CMAKE_INSTALL_PREFIX=$install_path
     * - will fail unless "make install" has run and the install tree
     *   is present and relocatable-install is OFF.
     * - will work in the install tree and build tree if "make"
     *   install has run.
     * - OME_COMMON_HOME or OME_HOME can override the hardcoded
     *   install prefix, but only if the new path contains an install
     *   tree.
     *
     * [install prefix ignored for self-contained distributions]
     * - used for prepackaged builds
     * - will fail in the build tree since there is no valid install
     * - will work in the install tree since it will introspect the
     *   correct path if relocatable-install is OFF or ON and dladdr
     *   or GetModuleFileNameW are available (required to determine
     *   the library path)
     * - OME_COMMON_HOME or OME_HOME can override the
     *   autodetected install prefix, but only if the new path
     *   contains an install tree.
     *
     * Testing:
     * - With and without CMAKE_INSTALL_PREFIX set [default is /usr/local]
     * - With relocatable-install set to OFF and ON
     * - In the install and build trees
     * - With and without OME_COMMON_HOME or OME_HOME
     * - With and without OME_COMMON_HOME or OME_HOME set to a
     *   valid path
     *
     * Testing in the build tree verifies it fails correctly.
     *
     * The sequence of checking dtype "foo" is:
     * - OME_$FOO env var
     * - OME_COMMON_HOME or OME_HOME env var [if set] + FOO_RELATIVE_PATH
     * - FOO_ABSOLUTE_PATH
     * - OME_COMMON_INSTALL_PREFIX [if set] + FOO_RELATIVE_PATH
     * - introspection [if possible] + FOO_RELATIVE_PATH
     * - throw exception
     */

    const fs::path&
    module_runtime_path(const std::string& dtype)
    {
      path_map& paths(module_paths());
      path_map::iterator ipath(paths.find(dtype));

      // Is this a valid dtype?
      if (ipath == paths.end())
        {
          boost::format fmt("Invalid runtime path type “%1%”");
          fmt % dtype;
          throw std::logic_error(fmt.str());
        }

      Module& module = ipath->second;

      // Return cached result if previously determined.
      if(!module.realpath.empty())
        return module.realpath;

      // dtype set explicitly in environment.
      if (getenv(module.envvar.c_str()))
        {
          fs::path dir(getenv(module.envvar.c_str()));
          if (validate_path(dir))
            {
              module.realpath = boost::filesystem::canonical(dir);
              return module.realpath;
            }
        }

      // Full module path in environment + relative component
      if (getenv(module.module_envvar.c_str()))
        {
          fs::path home(getenv(module.module_envvar.c_str()));
          home /= module.relpath;
          if (validate_path(home))
            {
              module.realpath = boost::filesystem::canonical(home);
              return module.realpath;
            }
        }

      // Full root path in environment + relative component
      if (getenv(module.root_envvar.c_str()))
        {
          fs::path home(getenv(module.root_envvar.c_str()));
          home /= module.relpath;
          if (validate_path(home))
            {
              module.realpath = boost::filesystem::canonical(home);
              return module.realpath;
            }
        }

      // Full prefix is available only when configured explicitly.
      if (validate_path(module.install_prefix))
        {
          // Full specific path.
          if (validate_path(module.abspath))
            {
              module.realpath = boost::filesystem::canonical(module.abspath);
              return module.realpath;
            }

          // Full root path + relative component
          fs::path home(module.install_prefix);
          home /= module.relpath;
          if (validate_path(home))
            {
              module.realpath = boost::filesystem::canonical(home);
              return module.realpath;
            }
        }
      else
        {
          fs::path module_lib_path;
          if (module.module_path)
            {
              module_lib_path = module.module_path();
            }
          if (module_lib_path.has_parent_path())
            {
              fs::path moduledir(module_lib_path.parent_path());
              bool match = true;

              fs::path libdir(module.shlibpath);

              while(!libdir.empty())
                {
                  if (libdir.filename() == moduledir.filename())
                    {
                      libdir = libdir.parent_path();
                      moduledir = moduledir.parent_path();
                    }
                  else
                    {
                      match = false;
                      break;
                    }
                }
              if (match && validate_path(moduledir))
                {
                  moduledir /= module.relpath;
                  if (validate_path(moduledir))
                    {
                      module.realpath = boost::filesystem::canonical(moduledir);
                      return module.realpath;
                    }
                }
            }
        }
      boost::format fmt("Could not determine Bio-Formats runtime path for “%1%” directory");
      fmt % dtype;
      throw std::runtime_error(fmt.str());
    }

    void
    register_module_paths()
    {
      register_paths();
    }

  }
}
