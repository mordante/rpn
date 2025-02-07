set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
cmake_minimum_required(VERSION 3.30.0 FATAL_ERROR)
# Hack needed to tell CMake we use libc++ for modules.
string(APPEND CMAKE_CXX_FLAGS_INIT " -stdlib=libc++")
project(rpn
	LANGUAGES CXX
)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CheckCXXCompilerFlag)
include(CodeCoverage)
# Make sure all dependencies use the libc++.
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-stdlib=libc++>)
add_link_options($<$<COMPILE_LANGUAGE:CXX>:-stdlib=libc++>)
include(ExternalProjects)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS YES)
set(CMAKE_CXX_MODULE_STD ON)
set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)
set(diagnostic_compile_options
	-Werror
	-Weverything
	-Wno-exit-time-destructors # Don't care about exit time destructors.
	-Wno-padded                # Don't care about the padding.
	-Wno-missing-prototypes    # False postives with export.
	-Wno-switch-enum           # False positives with default.
	# GoogleTest generated errors. TODO make GoogleTest a system header
	-Wno-global-constructors
	# Needs more investigation; anchoring them gives issues with a modular build.
	-Wno-weak-vtables
	# Ignore all compatibility with older C++ standards.
	-Wno-pre-c++20-compat
	-Wno-c++98-compat
	-Wno-c++98-compat-pedantic
	-Wno-c++11-compat
	-Wno-c++14-compat
	-Wno-c++17-compat
	-Wno-c++20-compat
	# TODO Investigate whether the issues are real or false positives.
	-Wno-unsafe-buffer-usage
	-Wno-integer-overflow
)

# The set of disagnostics above leads to an ICE.
# This set shows less issues, but compiles
#
# TODO investigate the ICE.
set(diagnostic_compile_options
	-Werror
	-Wall
	-Wextra
	-Wno-integer-overflow
	-D_LIBCPP___UTILITY_AS_CONST_H
)

set(CLANG_TIDY "" CACHE FILEPATH "The clang-tidy executable to use")

enable_testing()
add_code_coverage()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_subdirectory(scripts)
add_subdirectory(src)
add_subdirectory(tests)
