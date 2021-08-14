add_library(calculator
	controller.cpp
	model.cpp
	value.cpp
)
target_compile_options(calculator
	PRIVATE
		${module_compile_options}
)
target_link_libraries(calculator
	PRIVATE
		math
)
add_module(calculator calculator.cpp)
add_module(calculator.controller controller.cpp)
add_module(calculator.model model.cpp)
add_module(calculator.value value.cpp)
add_dependencies(calculator.model.pcm
	calculator.value.pcm
)
add_dependencies(calculator.controller.pcm
	calculator.model.pcm
)
add_dependencies(calculator.pcm
	calculator.value.pcm
	calculator.model.pcm
	calculator.controller.pcm
)
add_dependencies(calculator
	calculator.pcm
)