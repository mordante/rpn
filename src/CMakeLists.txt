include_directories(
		${fltk_SOURCE_DIR} # Default include directory
		${fltk_BINARY_DIR} # Contains the build information header
)
add_subdirectory(modules)
add_executable(rpn
	rpn.cpp
)
target_sources(rpn
	PUBLIC
		FILE_SET CXX_MODULES
		FILES
			gui.cpp
			tui.cpp
)
target_compile_options(rpn
	PRIVATE
		${diagnostic_compile_options}
)
set_target_properties(rpn
	PROPERTIES
		CXX_CLANG_TIDY "${CLANG_TIDY}"
		CMAKE_CXX_MODULE_STD ON
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
		ftxui::screen
		ftxui::dom
		ftxui::component
)
