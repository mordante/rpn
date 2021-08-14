add_subdirectory(modules)
add_executable(rpn
	rpn.cpp
)
target_compile_options(rpn
	PRIVATE
		${module_compile_options}
)
# These aren't properly set by fltk itself.
target_include_directories(rpn
	PRIVATE
		${fltk_SOURCE_DIR} # Default include directory
		${fltk_BINARY_DIR} # Contains the build information header
)
target_link_libraries(rpn
	PRIVATE
		calculator
		fltk
		c++
)
