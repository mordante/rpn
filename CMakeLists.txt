cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
project(rpn
	LANGUAGES CXX
)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CheckCXXCompilerFlag)
include(ClangModule)
include(CodeCoverage)
include(ExternalProjects)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
add_compile_options(-stdlib=libc++)
set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)
set(module_compile_options
	-fmodules
	-fbuiltin-module-map
	-fimplicit-module-maps
	-fprebuilt-module-path=${PREBUILT_MODULE_PATH}
)

enable_testing()

# XXX this adds the JSON STUFF...
# BACKPORT TO MASTER???
#add_code_coverage_all_targets()
add_code_coverage()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_subdirectory(src)
add_subdirectory(tests)
