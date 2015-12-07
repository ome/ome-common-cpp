# #%L
# Bio-Formats C++ libraries (cmake build infrastructure)
# %%
# Copyright © 2006 - 2015 Open Microscopy Environment:
#   - Massachusetts Institute of Technology
#   - National Institutes of Health
#   - University of Dundee
#   - Board of Regents of the University of Wisconsin-Madison
#   - Glencoe Software, Inc.
# %%
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are
# those of the authors and should not be interpreted as representing official
# policies, either expressed or implied, of any organization.
# #L%

cmake_minimum_required(VERSION 3.2.0)
cmake_policy(VERSION 3.2.0)

# For MSVC and FindThreads, respectively.
enable_language(CXX C)

list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_LIST_DIR}/cmake")

project(ome-common
        VERSION 5.2.0
        LANGUAGES CXX)
message(STATUS "Configuring ${CMAKE_PROJECT_NAME}")

include(cmake/Version.cmake)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "In-tree builds are not supported; please run cmake from a separate build directory.")
endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

set(BUILD_SHARED_LIBS_DEFAULT ON)
if(MSVC)
  set(BUILD_SHARED_LIBS_DEFAULT OFF)
endif()
set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_DEFAULT} CACHE BOOL "Use shared libraries")

include(Options)
include(GNUInstallDirs)
include(CompilerChecks)
include(PlatformChecks)
include(BoostChecks)
include(RegexChecks)
include(ThreadChecks)
include(XercesChecks)
include(GTest)
include(Doxygen)
include(HeaderTest)

if(MSVC)
  # Debug library suffix.
  set(CMAKE_DEBUG_POSTFIX "d")
  # Require dynamic linking and disable autolinking.
  add_definitions(-DBOOST_ALL_DYN_LINK -DBOOST_ALL_NO_LIB)
  # To prevent a lot of spam warnings about standard POSIX functions
  # and unsafe use of the standard library.
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
endif()


if(NOT DEFINED OME_BIOFORMATS_INSTALL_DATADIR)
  set(OME_BIOFORMATS_INSTALL_DATADIR "${CMAKE_INSTALL_DATADIR}/ome/bioformats"
      CACHE PATH "Bio-Formats-specific datadir")
endif()

# Use standard path if using a prefix.
if(CMAKE_INSTALL_PREFIX)
  if (NOT DEFINED OME_BIOFORMATS_INSTALL_LIBEXECDIR)
    set(OME_BIOFORMATS_INSTALL_LIBEXECDIR "${CMAKE_INSTALL_LIBEXECDIR}/ome/bioformats"
        CACHE PATH "Bio-Formats-specific libexecdir")
  endif()
  if (NOT DEFINED OME_BIOFORMATS_INSTALL_ICONDIR)
    set(OME_BIOFORMATS_INSTALL_ICONDIR "${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable"
        CACHE PATH "Icon directory")
  endif()
else()
  if (NOT DEFINED OME_BIOFORMATS_INSTALL_LIBEXECDIR)
    set(OME_BIOFORMATS_INSTALL_LIBEXECDIR "${CMAKE_INSTALL_LIBEXECDIR}"
        CACHE PATH "Bio-Formats-specific libexecdir")
  endif()
  if (NOT DEFINED OME_BIOFORMATS_INSTALL_ICONDIR)
    set(OME_BIOFORMATS_INSTALL_ICONDIR "${CMAKE_INSTALL_DATADIR}/icons"
        CACHE PATH "Icon directory")
  endif()
endif()

if (NOT DEFINED OME_BIOFORMATS_INSTALL_SCHEMADIR)
  set(OME_BIOFORMATS_INSTALL_SCHEMADIR "${CMAKE_INSTALL_DATADIR}/xml/ome"
      CACHE PATH "Bio-Formats schema directory")
endif()
if (NOT DEFINED OME_BIOFORMATS_INSTALL_TRANSFORMDIR)
  set(OME_BIOFORMATS_INSTALL_TRANSFORMDIR "${CMAKE_INSTALL_DATADIR}/xsl/ome"
      CACHE PATH "Bio-Formats transform directory")
endif()

foreach(dir
    DATADIR
    LIBEXECDIR
    ICONDIR
    SCHEMADIR
    TRANSFORMDIR
    )
  mark_as_advanced(OME_BIOFORMATS_INSTALL_${dir})
  if(NOT IS_ABSOLUTE ${OME_BIOFORMATS_INSTALL_${dir}})
    set(OME_BIOFORMATS_INSTALL_FULL_${dir} "${CMAKE_INSTALL_PREFIX}/${OME_BIOFORMATS_INSTALL_${dir}}")
  else()
    set(OME_BIOFORMATS_INSTALL_FULL_${dir} "${OME_BIOFORMATS_INSTALL_${dir}}")
  endif()
endforeach()

add_subdirectory(docs/doxygen)

set(OME_TOPLEVEL_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_BINARY_DIR}/lib)

# Add test, making sure it's run with a suitable environment.
# If updated, make sure to update the wrappers in cmake.
function(ome_add_test)
  add_test(${ARGV})
endfunction()

add_subdirectory(lib)
add_subdirectory(test)
