#add_library(calculator
#	calculator.cppm
#	controller.cppm
#	model.cppm
#	# TODO Evaluate whether this needs its own module
#	math/arithmetic.cppm
#	math/bitwise.cppm
#	math/core.cppm
#	math/round.cppm
#	stack.cppm
#	transaction.cppm
#	undo_handler.cppm
#	value.cppm
#)
#target_compile_options(calculator
#	PRIVATE
#		${module_compile_options}
#		${diagnostic_compile_options}
#)
#set_target_properties(calculator
#	PROPERTIES
#		CXX_CLANG_TIDY "${CLANG_TIDY}"
#)
#target_link_libraries(calculator
#	PRIVATE
#		lib
#		parser
#)
add_module(calculator calculator.cppm)
add_module(calculator-controller controller.cppm)
add_module(calculator-math-core math/core.cppm)
add_module(calculator-math-arithmetic math/arithmetic.cppm)
add_module(calculator-math-bitwise math/bitwise.cppm)
add_module(calculator-math-round math/round.cppm)
add_module(calculator-model model.cppm)
add_module(calculator-stack stack.cppm)
add_module(calculator-transaction transaction.cppm)
add_module(calculator-undo_handler undo_handler.cppm)
add_module(calculator-value value.cppm)
add_dependencies(calculator-math-arithmetic.pcm
	calculator-math-core.pcm
)
add_dependencies(calculator-math-bitwise.pcm
	calculator-math-core.pcm
)
add_dependencies(calculator-math-round.pcm
	calculator-math-core.pcm
)
add_dependencies(calculator-value.pcm
	calculator-math-arithmetic.pcm
	calculator-math-bitwise.pcm
	calculator-math-core.pcm
	calculator-math-round.pcm
)
add_dependencies(calculator-stack.pcm
	calculator-value.pcm
)
add_dependencies(calculator-model.pcm
	#parser.pcm
	calculator-stack.pcm
)
add_dependencies(calculator-transaction.pcm
	calculator-model.pcm
)
add_dependencies(calculator-undo_handler.pcm
	calculator-transaction.pcm
)
add_dependencies(calculator-controller.pcm
	calculator-model.pcm
	calculator-transaction.pcm
	calculator-undo_handler.pcm
	#lib-dictionary.pcm
)
add_dependencies(calculator.pcm
	calculator-controller.pcm
	calculator-model.pcm
	calculator-value.pcm
)

compile_module(core)

https://github.com/ChuanqiXu9/stdmodules/blob/2f709df98110f3b2ff44d18585e2daf3d1b4511f/Makefile#L45-L81

#dd_dependencies(calculator
#	calculator.pcm
#)
