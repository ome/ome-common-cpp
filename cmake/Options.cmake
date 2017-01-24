# These are annoyingly verbose, produce false positives or don't work
# nicely with all supported compiler versions, so are disabled unless
# explicitly enabled.
option(extra-warnings "Enable extra compiler warnings" OFF)

# This will cause the compiler to fail when an error occurs.
option(fatal-warnings "Compiler warnings are errors" OFF)

# Unit tests.
option(test "Enable unit tests (requires gtest)" ON)
option(extended-tests "Enable extended tests (more comprehensive, longer run time)" ON)

# The installation is relocatable; this affects path lookups (if OFF,
# paths are assumed to be their configured absolute install location;
# paths will still be introspected as a fallback); if ON paths will be
# introspected if possible.  In all cases the paths may be overridden
# by the environment.
option(relocatable-install OFF)
set(OME_RELOCATABLE_INSTALL "Install tree will be relocatable" ${relocatable-install})
set(OME_COMMON_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
if(relocatable-install)
  set(OME_COMMON_INSTALL_PREFIX "")
endif()

# Doxygen documentation
find_package(Doxygen)
set(DOXYGEN_DEFAULT OFF)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
  set (DOXYGEN_DEFAULT ON)
endif (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
option(doxygen "Enable doxygen documentation" ${DOXYGEN_DEFAULT})
set(BUILD_DOXYGEN ${doxygen})
