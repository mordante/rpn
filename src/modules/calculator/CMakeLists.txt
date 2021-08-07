add_library(calculator
	value.cpp
)
target_compile_options(calculator
	PRIVATE
		${module_compile_options}
)
add_module(calculator.value value.cpp)
add_dependencies(calculator calculator.value.pcm)
