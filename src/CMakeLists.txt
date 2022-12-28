include_directories(
		${fltk_SOURCE_DIR} # Default include directory
		${fltk_BINARY_DIR} # Contains the build information header
)
add_subdirectory(modules)
add_executable(rpn
	gui.cpp
	rpn.cpp
)
target_compile_options(rpn
	PRIVATE
		${module_compile_options}
		${diagnostic_compile_options}
)
set_target_properties(rpn
	PROPERTIES
		CXX_CLANG_TIDY "${CLANG_TIDY}"
)
# These aren't properly set by fltk itself.
target_include_directories(rpn
	SYSTEM PRIVATE
		${fltk_SOURCE_DIR} # Default include directory
		${fltk_BINARY_DIR} # Contains the build information header
)
target_link_libraries(rpn
	PRIVATE
		calculator
		fltk
)
# Due to the extra include path this can't use the
# module function to create the target.
add_custom_target(gui.pcm
	COMMAND
		${CMAKE_CXX_COMPILER}
		-std=c++2b
		-stdlib=libc++
		-fexperimental-library
		-fmodules
		-fprebuilt-module-path=${PREBUILT_MODULE_PATH}
		-c
		${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
		-isystem ${fltk_SOURCE_DIR}
		-isystem ${fltk_BINARY_DIR}
		-Xclang -emit-module-interface
		-o ${PREBUILT_MODULE_PATH}/gui.pcm
	DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
		calculator.pcm
)
add_dependencies(rpn
	gui.pcm
)
