add_library(calculator
	model.cpp
	value.cpp
)
target_compile_options(calculator
	PRIVATE
		${module_compile_options}
)
add_module(calculator.model model.cpp)
add_module(calculator.value value.cpp)
add_dependencies(calculator
	calculator.model.pcm
	calculator.value.pcm
)
