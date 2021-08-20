add_library(calculator
	controller.cpp
	model.cpp
	# TODO Evaluate whether this needs its own module
	math/arithmetic.cpp
	math/bitwise.cpp
	math/core.cpp
	value.cpp
)
target_compile_options(calculator
	PRIVATE
		${module_compile_options}
)
add_module(calculator calculator.cpp)
add_module(calculator.controller controller.cpp)
add_module(calculator.math.core math/core.cpp)
add_module(calculator.math.arithmetic math/arithmetic.cpp)
add_module(calculator.math.bitwise math/bitwise.cpp)
add_module(calculator.model model.cpp)
add_module(calculator.value value.cpp)
add_dependencies(calculator.math.arithmetic.pcm
	calculator.math.core.pcm
)
add_dependencies(calculator.math.bitwise.pcm
	calculator.math.core.pcm
)
add_dependencies(calculator.value.pcm
	calculator.math.arithmetic.pcm
	calculator.math.bitwise.pcm
)
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
