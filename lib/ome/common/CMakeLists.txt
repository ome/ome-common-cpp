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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-internal.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config-internal.h @ONLY)

set(ome_common_static_headers
    base64.h
    boolean.h
    filesystem.h
    endian.h
    log.h
    module.h
    mstream.h
    string.h
    units.h
    variant.h)

set(ome_common_endian_static_headers
    endian/conversion.hpp
    endian/std_pair.hpp
    endian/types.hpp)

set(ome_common_endian_detail_static_headers
    endian/detail/config.hpp
    endian/detail/cover_operators.hpp
    endian/detail/disable_warnings.hpp
    endian/detail/disable_warnings_pop.hpp
    endian/detail/intrinsic.hpp)

set(ome_common_units_static_headers
    units/angle.h
    units/electric-potential.h
    units/frequency.h
    units/length.h
    units/power.h
    units/pressure.h
    units/temperature.h
    units/time.h
    units/types.h)

set(ome_common_xml_static_headers
    xml/EntityResolver.h
    xml/ErrorReporter.h
    xml/Platform.h
    xml/String.h)

set(ome_common_xsl_static_headers
    xsl/Platform.h
    xsl/Transformer.h)

set(ome_common_xml_dom_static_headers
    xml/dom/Base.h
    xml/dom/Document.h
    xml/dom/Element.h
    xml/dom/NamedNodeMap.h
    xml/dom/Node.h
    xml/dom/NodeList.h
    xml/dom/Wrapper.h)

set(ome_common_generated_private_headers
   ${CMAKE_CURRENT_BINARY_DIR}/config-internal.h)

set(ome_common_generated_headers
    ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(ome_common_headers
    ${ome_common_static_headers}
    ${ome_common_xml_static_headers}
    ${ome_common_xml_dom_static_headers}
    ${ome_common_xsl_static_headers}
    ${ome_common_generated_headers}
    ${ome_common_generated_private_headers})

set(ome_common_sources
    log.cpp
    module.cpp
    xml/EntityResolver.cpp
    xml/ErrorReporter.cpp
    xml/Platform.cpp
    xml/String.cpp
    xml/dom/Document.cpp
    xml/dom/NamedNodeMap.cpp
    xml/dom/NodeList.cpp
    xsl/Platform.cpp
    xsl/Transformer.cpp)

add_library(ome-common ${ome_common_sources} ${ome_common_headers})

target_include_directories(ome-common PUBLIC
                           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
                           $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/lib>
                           $<BUILD_INTERFACE:${LibDl_INCLUDE_DIRS}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ome-common
                      PUBLIC
                      OME::Compat
                      Boost::log_setup
                      Boost::log
                      Boost::iostreams
                      Boost::filesystem
                      Boost::system
                      XercesC::XercesC
                      XalanC::XalanC
                      PRIVATE
                      ${LibDl_LIBRARIES})

set_target_properties(ome-common PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ome-common PROPERTIES VERSION ${ome-common_VERSION})

add_library(OME::Common ALIAS ome-common)

if(WIN32)
  set(ome_common_config_dir "cmake")
else()
  set(ome_common_config_dir "${CMAKE_INSTALL_LIBDIR}/cmake/OMECommon")
endif()

install(TARGETS ome-common
        EXPORT OMECommonInternal
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT "runtime"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(EXPORT OMECommonInternal
        DESTINATION "${ome_common_config_dir}"
        NAMESPACE "ome_common_"
        COMPONENT "development")

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/OMECommonConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OMECommonConfig.cmake"
  INSTALL_DESTINATION "${ome_common_config_dir}")
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/OMECommonConfigVersion.cmake
  VERSION "${ome-common_VERSION}"
  COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/OMECommonConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/OMECommonConfigVersion.cmake
        DESTINATION "${ome_common_config_dir}"
        COMPONENT "development")

set(ome_common_includedir "${CMAKE_INSTALL_INCLUDEDIR}/ome/common")

install(FILES ${ome_common_static_headers} ${ome_common_generated_headers}
        DESTINATION ${ome_common_includedir}
        COMPONENT "development")
install(FILES ${ome_common_endian_static_headers}
        DESTINATION ${ome_common_includedir}/endian
        COMPONENT "development")
install(FILES ${ome_common_endian_detail_static_headers}
        DESTINATION ${ome_common_includedir}/endian/detail
        COMPONENT "development")
install(FILES ${ome_common_units_static_headers}
        DESTINATION ${ome_common_includedir}/units
        COMPONENT "development")
install(FILES ${ome_common_xml_static_headers}
        DESTINATION ${ome_common_includedir}/xml
        COMPONENT "development")
install(FILES ${ome_common_xml_dom_static_headers}
        DESTINATION ${ome_common_includedir}/xml/dom
        COMPONENT "development")
install(FILES ${ome_common_xsl_static_headers}
        DESTINATION ${ome_common_includedir}/xsl
        COMPONENT "development")

# Dump header list for testing
header_include_list_write(ome_common_static_headers ome_common_generated_headers ome/common ${PROJECT_BINARY_DIR}/test/ome-common)
