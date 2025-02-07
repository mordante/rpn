# Generic library.
#
# This probably needs a better structure when more library code is added.
# Therefore there's no generic lib module.
add_library(lib)
target_sources(lib
	PUBLIC
		FILE_SET CXX_MODULES
		FILES
			base.cpp
			dictionary.cpp
			binary_find.cpp
)
target_compile_options(lib
	PRIVATE
		${diagnostic_compile_options}
)
set_target_properties(lib
	PROPERTIES
		CXX_CLANG_TIDY "${CLANG_TIDY}"
		CMAKE_CXX_MODULE_STD ON
)
