include(CMakeFindDependencyMacro)
find_dependency(Boost 1.46 REQUIRED
                COMPONENTS date_time filesystem system iostreams
                program_options regex thread)
find_dependency(XercesC 3.0.0 REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/ome-compat-internal.cmake)

add_library(OME::Compat INTERFACE IMPORTED)
set_target_properties(OME::Compat PROPERTIES INTERFACE_LINK_LIBRARIES ome-compat)
