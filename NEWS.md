5.5.0 (2017-11-27)
------------------

* Use CMP0067 to enable standard setting in feature tests ([#47](https://github.com/ome/ome-common-cpp/pull/47))
* CMake 3.4 is the minimum version ([#48](https://github.com/ome/ome-common-cpp/pull/48))
* Use C++11 \<thread\> and \<mutex\> ([#46](https://github.com/ome/ome-common-cpp/pull/46))
* Restore support for Boost 1.53 ([#49](https://github.com/ome/ome-common-cpp/pull/49))
* Add support for Boost 1.65 and 1.65.1 ([#50](https://github.com/ome/ome-common-cpp/pull/50))
* Conditionally add log and log_setup to exported configuration ([#51](https://github.com/ome/ome-common-cpp/pull/51))

5.4.2 (2017-06-12)
------------------

* Doxygen improvements ([#38](https://github.com/ome/ome-common-cpp/pull/38), [#39](https://github.com/ome/ome-common-cpp/pull/39))
* CMake: use C++ standard variables as documented ([#40](https://github.com/ome/ome-common-cpp/pull/40))

5.4.1 (2017-05-23)
------------------

* Add documentation links ([#35](https://github.com/ome/ome-common-cpp/pull/35))
* Add support for Boost 1.64 ([#37](https://github.com/ome/ome-common-cpp/pull/37))

5.4.0 (2017-02-09)
------------------

* C++11 is now the minimum required language version, with C++14 being used when available ([#29](https://github.com/ome/ome-common-cpp/pull/29))
* enabled the use of a restricted number of C++11 features, including enum class, nullptr, initializer lists, range-based for loops and type traits ([#31](https://github.com/ome/ome-common-cpp/pull/31))
* enabled the use of C++11 syntax changes including “<::” not being interpreted as a trigraph and “»” being used to close nested templates instead of “> >” ([#31](https://github.com/ome/ome-common-cpp/pull/31))
* Google Test (gtest) is no longer built separately in each source component; the latest gtest release now allows use as a conventional library ([#28](https://github.com/ome/ome-common-cpp/pull/28))
* source releases are now made directly from git with “git archive” ([#32](https://github.com/ome/ome-common-cpp/pull/32))
* Add support for Boost 1.63 ([#30](https://github.com/ome/ome-common-cpp/pull/30))
* Boost MPL size limits are now set with MSVC only; they are no longer required for other compilers with C++11
* <ome/compat/array.h>, <ome/compat/cstdint.h>, <ome/compat/memory.h> and <ome/compat/tuple.h> are now deprecated in favor of the standard headers; they wrap the standard headers for backward compatibility but will be removed in the next breaking release ([#31](https://github.com/ome/ome-common-cpp/pull/31))

5.3.2 (2016-10-12)
------------------

* Boost 1.54 is the minimum supported version ([#25](https://github.com/ome/ome-common-cpp/pull/25))
* Add support for Boost 1.62 ([#26](https://github.com/ome/ome-common-cpp/pull/26))

5.3.1 (2016-09-18)
------------------

* Document all namespaces in API reference ([#24](https://github.com/ome/ome-common-cpp/pull/24))

5.3.0 (2016-07-29)
------------------

* Added Base64 conversion functions ([#16](https://github.com/ome/ome-common-cpp/pull/16))
* Corrected Xerces UTF8 to UTF-16 transcoding for empty strings ([#18](https://github.com/ome/ome-common-cpp/pull/18))
* Added rankine temperature conversions ([#17](https://github.com/ome/ome-common-cpp/pull/17))
* Added MSVC workarounds for system macros with names matching unit names ([#22](https://github.com/ome/ome-common-cpp/pull/22))
* Removed VS2012 workarounds ([#15](https://github.com/ome/ome-common-cpp/pull/15))
* Improved CMake configuration export ([#21](https://github.com/ome/ome-common-cpp/pull/21))
* Added BSD licence text ([#23](https://github.com/ome/ome-common-cpp/pull/23))


5.2.0 (2016-04-23)
------------------

* Decouple ome-common-cpp component from Bio-Formats ([#1](https://github.com/ome/ome-common-cpp/pull/1), [#9](https://github.com/ome/ome-common-cpp/pull/9))
* Use external gtest ([#2](https://github.com/ome/ome-common-cpp/pull/2))
* Add support for Boost 1.60 ([#4](https://github.com/ome/ome-common-cpp/pull/4))
* Add Xalan-C support ([#7](https://github.com/ome/ome-common-cpp/pull/7))
* Rename all uses of bioformats to ome-common ([#10](https://github.com/ome/ome-common-cpp/pull/10))
* Add source-archive script to generate a source release ([#12](https://github.com/ome/ome-common-cpp/pull/12), [#13](https://github.com/ome/ome-common-cpp/pull/13), [#14](https://github.com/ome/ome-common-cpp/pull/14))
