# Generic library.
#
# This probably needs a better structure when more library code is added.
# Therefore there's no generic lib module.
add_library(lib
	base.cpp
	dictionary.cpp
	binary_find.cpp
)
target_compile_options(lib
	PRIVATE
		${module_compile_options}
		${diagnostic_compile_options}
)
set_target_properties(lib
	PROPERTIES
		CXX_CLANG_TIDY "${CLANG_TIDY}"
)
add_module(lib.base base.cpp)
add_module(lib.binary_find binary_find.cpp)
add_module(lib.dictionary dictionary.cpp)
add_dependencies(lib.base.pcm
	lib.base.pcm
)
add_dependencies(lib.dictionary.pcm
	lib.binary_find.pcm
)
add_dependencies(lib
	lib.dictionary.pcm
)
