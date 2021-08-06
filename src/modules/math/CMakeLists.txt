add_library(math
	math.cpp
)
target_compile_options(math
	PRIVATE
		${module_compile_options}
)
add_module(math math.cpp)
add_dependencies(math math.pcm)
