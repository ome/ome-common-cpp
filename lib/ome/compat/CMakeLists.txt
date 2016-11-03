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

set(ome_compat_headers
    array.h
    cstdint.h
    memory.h
    regex.h
    tuple.h)

add_library(ome-compat INTERFACE)

target_include_directories(ome-compat INTERFACE
                           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
                           $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/lib>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ome-compat INTERFACE
                      Boost::boost
                      ${REGEX_LIBRARY})

add_library(OME::Compat ALIAS ome-compat)

if(WIN32)
  set(ome_compat_config_dir "cmake")
else()
  set(ome_compat_config_dir "${CMAKE_INSTALL_LIBDIR}/cmake/OMECompat")
endif()

install(TARGETS ome-compat
        EXPORT OMECompatInternal
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT "runtime"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(EXPORT OMECompatInternal
        DESTINATION "${ome_compat_config_dir}"
        NAMESPACE "ome_compat_"
        COMPONENT "development")

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/OMECompatConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OMECompatConfig.cmake"
  INSTALL_DESTINATION "${ome_compat_config_dir}")
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/OMECompatConfigVersion.cmake
  VERSION "${ome-common_VERSION}"
  COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/OMECompatConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/OMECompatConfigVersion.cmake
        DESTINATION "${ome_compat_config_dir}"
        COMPONENT "development")

set(ome_compat_includedir "${CMAKE_INSTALL_INCLUDEDIR}/ome/compat")

install(FILES ${ome_compat_headers}
        DESTINATION ${ome_compat_includedir}
        COMPONENT "development")

# Dump header list for testing
header_include_list_write(ome_compat_headers "" ome/compat ${PROJECT_BINARY_DIR}/test/ome-compat)
