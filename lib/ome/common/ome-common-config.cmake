include(CMakeFindDependencyMacro)
find_dependency(Boost 1.46 REQUIRED
                COMPONENTS date_time filesystem system iostreams
                program_options regex thread)
find_dependency(XercesC 3.0.0 REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/ome-common-internal.cmake)

add_library(OME::Common INTERFACE IMPORTED)
set_target_properties(OME::Common PROPERTIES INTERFACE_LINK_LIBRARIES ome-common)
