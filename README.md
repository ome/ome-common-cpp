# OME Common C++

OME Common is a standalone C++ library required by other OME C++
projects for common functionality which is not readily available from
the C++ Standard Library.  This includes basic portability functions,
to wrapping other libraries to make them usable with Modern C++
programming practices.

It serves a similar purpose to the OME Common Java library, with some
shared functionality, though for the most part they are quite
different.


Purpose
-------

OME Common's primary purpose is to ensure that a certain level of
basic functionality is provided for all platforms and compilers
supported by OME C++ projects.  It currently includes:

- Missing C++ standard library functionality:

  * array
  * cstdint
  * memory
  * regex
  * tuple

- Missing Boost functionality:

  * Endian conversions
  * Filesystem absolute path determination
  * Variant and MPL set up
  * Units (extended datatypes for all units used by the OME data
    model)

- Extra functionality:

  * Boolean type for iterable 8-bit mask pixel data
  * logging
  * installation path determination
  * memory streams
  * string portability and helper functions
  * XML parsing (Xerces-C++ Modern C++ RAII wrappers)


More information
----------------

For more information, see the [Bio-Formats C++ web
site](http://www.openmicroscopy.org/site/support/bio-formats5.2/developers/cpp/overview.html).


Pull request testing
--------------------

We welcome pull requests from anyone, but ask that you please verify the
following before submitting a pull request:

 * verify that the branch merges cleanly into ```develop```
 * verify that the branch compiles
 * run the unit tests (```ctest -V```) and correct any failures
 * make sure that your commits contain the correct authorship information and,
   if necessary, a signed-off-by line
 * make sure that the commit messages or pull request comment contains
   sufficient information for the reviewer(s) to understand what problem was
   fixed and how to test it
