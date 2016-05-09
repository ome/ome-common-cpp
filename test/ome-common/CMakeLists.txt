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

if(BUILD_TESTS)
  if(extended-tests)
    header_test_from_file(ome-common OME::Common ome/common)
    ome_add_test(ome-common/headers ome-common-headers)
  endif(extended-tests)

  add_executable(base64 base64.cpp)
  target_link_libraries(base64 OME::Common)
  target_link_libraries(base64 OME::Test)

  ome_add_test(ome-common/base64 base64)

  add_executable(boolean boolean.cpp)
  target_link_libraries(boolean OME::Common)
  target_link_libraries(boolean OME::Test)

  ome_add_test(ome-common/boolean boolean)

  add_executable(endian endian.cpp)
  target_link_libraries(endian OME::Common)
  target_link_libraries(endian OME::Test)

  ome_add_test(ome-common/endian endian)

  add_executable(filesystem filesystem.cpp)
  target_link_libraries(filesystem OME::Common)
  target_link_libraries(filesystem OME::Test)

  ome_add_test(ome-common/filesystem filesystem)

  add_executable(module module.cpp)
  target_link_libraries(module OME::Common)
  target_link_libraries(module OME::Test)

  ome_add_test(ome-common/module module)

  add_executable(mstream mstream.cpp)
  target_link_libraries(mstream OME::Common)
  target_link_libraries(mstream OME::Test)

  ome_add_test(ome-common/mstream mstream)

  add_executable(thread thread.cpp)
  target_link_libraries(thread OME::Common ${THREAD_LIBRARY})
  target_link_libraries(thread OME::Test)

  ome_add_test(ome-common/thread thread)

  add_executable(string string.cpp)
  target_link_libraries(string OME::Common)
  target_link_libraries(string OME::Test)

  ome_add_test(ome-common/string string)

  add_executable(units
    units.h
    units.cpp
    units-angle.cpp
    units-electric-potential.cpp
    units-frequency.cpp
    units-length.cpp
    units-power.cpp
    units-pressure.cpp
    units-temperature.cpp
    units-time.cpp)
  target_link_libraries(units OME::Common)
  target_link_libraries(units OME::Test Boost::iostreams)

  ome_add_test(ome-common/units units)

  add_executable(variant variant.cpp)
  target_link_libraries(variant OME::Common)
  target_link_libraries(variant OME::Test)

  ome_add_test(ome-common/variant variant)

  add_executable(xerces xerces.cpp)
  target_link_libraries(xerces OME::Common)
  target_link_libraries(xerces OME::Test)

  ome_add_test(ome-common/xerces xerces)

  add_executable(xalan xalan.cpp)
  target_link_libraries(xalan OME::Common)
  target_link_libraries(xalan OME::Test)

  ome_add_test(ome-common/xalan xalan)

endif(BUILD_TESTS)
