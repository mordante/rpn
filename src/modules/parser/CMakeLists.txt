add_library(parser)

target_sources(parser
	PUBLIC
		FILE_SET CXX_MODULES
		FILES
			detail/base.cpp
			detail/floating_point.cpp
			detail/invalid_value.cpp
			detail/signed.cpp
			detail/string.cpp
			detail/unsigned.cpp
			parser.cpp
			token.cpp
)
target_compile_options(parser
	PRIVATE
 		${diagnostic_compile_options}
)
set_target_properties(parser
	PROPERTIES
		CXX_CLANG_TIDY "${CLANG_TIDY}"
		CMAKE_CXX_MODULE_STD ON
)
target_link_libraries(parser
	PRIVATE
		lib
)
