add_library(calculator)

target_sources(calculator
	PUBLIC
		FILE_SET CXX_MODULES
		FILES
			calculator.cpp
			controller.cpp
			model.cpp
			# TODO Evaluate whether this needs its own module
			math/arithmetic.cpp
			math/bitwise.cpp
			math/core.cpp
			math/logarithm.cpp
			math/round.cpp
			stack.cpp
			transaction.cpp
			undo_handler.cpp
			value.cpp
)
target_compile_options(calculator
	PRIVATE
		${diagnostic_compile_options}
)
set_target_properties(calculator
	PROPERTIES
		CXX_CLANG_TIDY "${CLANG_TIDY}"
		CMAKE_CXX_MODULE_STD ON
)
target_link_libraries(calculator
	PRIVATE
		lib
		parser
)
