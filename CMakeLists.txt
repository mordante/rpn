cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)
project(rpn
	LANGUAGES CXX
)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CheckCXXCompilerFlag)
include(ClangModule)
include(CodeCoverage)
# Make sure all dependencies use the libc++.
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-stdlib=libc++>)
add_link_options($<$<COMPILE_LANGUAGE:CXX>:-stdlib=libc++>)
include(ExternalProjects)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
add_compile_options(-fexperimental-library)
add_link_options(-fexperimental-library)
set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)
set(module_compile_options
	-fmodules
	-fbuiltin-module-map
	-fimplicit-module-maps
	-fprebuilt-module-path=${PREBUILT_MODULE_PATH}
)
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
)

set(CLANG_TIDY "" CACHE FILEPATH "The clang-tidy executable to use")

enable_testing()
add_code_coverage()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_subdirectory(scripts)
add_subdirectory(src)
add_subdirectory(tests)
