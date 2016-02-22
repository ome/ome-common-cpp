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

#include <boost/format.hpp>
#include <boost/range/size.hpp>

#include <ome/common/config-internal.h>
#include <ome/common/filesystem.h>

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
  validate_root_path(const fs::path& path)
  {
    fs::path schema(path / fs::path(OME_BIOFORMATS_INSTALL_SCHEMADIR));
    return (fs::exists(path) && fs::is_directory(path) &&
            fs::exists(schema) && fs::is_directory(schema));
  }

  bool
  validate_path(const fs::path& path)
  {
    return (fs::exists(path) && fs::is_directory(path));
  }

  // Global paths (not specific to any component)
  RegisterModule bin("bin", "BIOFORMATS_BINDIR", INSTALL_FULL_BINDIR, INSTALL_BINDIR, module_path);
  RegisterModule sbin("sbin", "BIOFORMATS_SBINDIR", INSTALL_FULL_SBINDIR, INSTALL_SBINDIR, module_path);
  // Note envvar SYS prefix to avoid clash with package path.
  RegisterModule libexec("libexec", "BIOFORMATS_SYSLIBEXECDIR", INSTALL_FULL_LIBEXECDIR, INSTALL_LIBEXECDIR, module_path);
  RegisterModule sysconf("sysconf", "BIOFORMATS_SYSCONFDIR", INSTALL_FULL_SYSCONFDIR, INSTALL_SYSCONFDIR, module_path);
  RegisterModule sharedstate("sharedstate", "BIOFORMATS_SHAREDSTATEDIR", INSTALL_FULL_SHAREDSTATEDIR, INSTALL_SHAREDSTATEDIR, module_path);
  RegisterModule localstate("localstate", "BIOFORMATS_LOCALSTATEDIR", INSTALL_FULL_LOCALSTATEDIR, INSTALL_LOCALSTATEDIR, module_path);
  RegisterModule lib("lib", "BIOFORMATS_LIBDIR", INSTALL_FULL_LIBDIR, INSTALL_LIBDIR, module_path);
  RegisterModule include("include", "BIOFORMATS_INCLUDEDIR", INSTALL_FULL_INCLUDEDIR, INSTALL_INCLUDEDIR, module_path);
  RegisterModule oldinclude("oldinclude", "BIOFORMATS_OLDINCLUDEDIR", INSTALL_FULL_OLDINCLUDEDIR, INSTALL_OLDINCLUDEDIR, module_path);
  RegisterModule dataroot("dataroot", "BIOFORMATS_DATAROOTDIR", INSTALL_FULL_DATAROOTDIR, INSTALL_DATAROOTDIR, module_path);
  // Note envvar SYS prefix to avoid clash with package path.
  RegisterModule data("data", "BIOFORMATS_SYSDATADIR", INSTALL_FULL_DATADIR, INSTALL_DATADIR, module_path);
  RegisterModule info("info", "BIOFORMATS_INFODIR", INSTALL_FULL_INFODIR, INSTALL_INFODIR, module_path);
  RegisterModule locale("locale", "BIOFORMATS_LOCALEDIR", INSTALL_FULL_LOCALEDIR, INSTALL_LOCALEDIR, module_path);
  RegisterModule man("man", "BIOFORMATS_MANDIR", INSTALL_FULL_MANDIR, INSTALL_MANDIR, module_path);
  RegisterModule doc("doc", "BIOFORMATS_DOCDIR", INSTALL_FULL_DOCDIR, INSTALL_DOCDIR, module_path);

  // OME-Common package-specific paths.
  RegisterModule bf_root("bf-root", "BIOFORMATS_HOME", INSTALL_PREFIX, "", module_path);
}

namespace ome
{
  namespace common
  {

    Module::Module(const std::string&               name,
                   const std::string&               envvar,
                   const boost::filesystem::path&   abspath,
                   const boost::filesystem::path&   relpath,
                   boost::filesystem::path        (*module_path)()):
      name(name),
      envvar(envvar),
      abspath(abspath),
      relpath(relpath),
      realpath(),
      module_path(module_path)
    {
    }

    RegisterModule::RegisterModule(const std::string&               name,
                                   const std::string&               envvar,
                                   const boost::filesystem::path&   abspath,
                                   const boost::filesystem::path&   relpath,
                                   boost::filesystem::path        (*module_path)()):
      name(name),
      registered(false)
    {
      Module m(name, envvar, abspath, relpath, module_path);
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
     * - BIOFORMATS_HOME can override the hardcoded install prefix,
     *   but only if the new path contains an install tree.
     *
     * CMAKE_INSTALL_PREFIX=  [no install prefix for self-contained distributions]
     * - used for prepackaged zips
     * - will fail in the build tree since there is no valid install
     * - will work in the install tree since it will introspect the
     *   correct path if relocatable-install is OFF or ON and dladdr
     *   or GetModuleFileNameW are available (required to determine
     *   the library path)
     * - BIOFORMATS_HOME can override the autodetected install prefix,
     *   but only if the new path contains an install tree.
     *
     * Testing:
     * - With and without CMAKE_INSTALL_PREFIX set [default is /usr/local]
     * - With relocatable-install set to OFF and ON
     * - In the install and build trees
     * - With and without BIOFORMATS_HOME
     * - With and without BIOFORMATS_HOME set to a valid path
     *
     * Testing in the build tree verifies it fails correctly.
     *
     * The sequence of checking dtype "foo" is:
     * - BIOFORMATS_$FOO env var
     * - BIOFORMATS_HOME env var [if set] + FOO_RELATIVE_PATH
     * - FOO_ABSOLUTE_PATH
     * - INSTALL_PREFIX [if set] + FOO_RELATIVE_PATH
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
              module.realpath = ome::common::canonical(dir);
              return module.realpath;
            }
        }

      // Full root path in environment + relative component
      if (getenv("BIOFORMATS_HOME"))
        {
          fs::path home(getenv("BIOFORMATS_HOME"));
          if (validate_root_path(home))
            {
              home /= module.relpath;
              if (validate_path(home))
                {
                  module.realpath = ome::common::canonical(home);
                  return module.realpath;
                }
            }
        }

      // Full prefix is available only when configured explicitly.
      if (strlen(INSTALL_PREFIX) > 0)
        {
          // Full specific path.
          if (validate_path(module.abspath))
            {
              module.realpath = ome::common::canonical(module.abspath);
              return module.realpath;
            }

          // Full root path + relative component
          fs::path home(INSTALL_PREFIX);
          if (validate_root_path(home))
            {
              home /= module.relpath;
              if (validate_path(home))
                {
                  module.realpath = ome::common::canonical(home);
                  return module.realpath;
                }
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

#ifdef _MSC_VER
              fs::path libdir(INSTALL_BINDIR);
#else
              fs::path libdir(INSTALL_LIBDIR);
#endif

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
                      module.realpath = ome::common::canonical(moduledir);
                      return module.realpath;
                    }
                }
            }
        }
      boost::format fmt("Could not determine Bio-Formats runtime path for “%1%” directory");
      fmt % dtype;
      throw std::runtime_error(fmt.str());
    }

  }
}
