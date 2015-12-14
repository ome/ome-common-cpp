# Try to put the compiler into the most recent standard mode.  This
# will generally have the most features, and will remove the need for
# Boost fallbacks if native implementations are available.
option(cxxstd-autodetect "Enable C++14 features if possible, otherwise fall back to C++11, C++03 or C++98" OFF)

# These are annoyingly verbose, produce false positives or don't work
# nicely with all supported compiler versions, so are disabled unless
# explicitly enabled.
option(extra-warnings "Enable extra compiler warnings" OFF)

# This will cause the compiler to fail when an error occurs.
option(fatal-warnings "Compiler warnings are errors" OFF)

# Unit tests.
option(test "Enable unit tests (requires gtest)" ON)
option(extended-tests "Enable extended tests (more comprehensive, longer run time)" ON)
option(embedded-gtest "Use embedded gtest rather than an external build" OFF)

# Doxygen documentation
find_package(Doxygen)
set(DOXYGEN_DEFAULT OFF)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
  set (DOXYGEN_DEFAULT ON)
endif (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
option(doxygen "Enable doxygen documentation" ${DOXYGEN_DEFAULT})
set(BUILD_DOXYGEN ${doxygen})
