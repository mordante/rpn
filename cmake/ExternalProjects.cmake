include(FetchContent)

FetchContent_Declare(
  fltk
  GIT_REPOSITORY https://github.com/fltk/fltk
  GIT_TAG        release-1.3.8
)
FetchContent_GetProperties(fltk)
if(NOT fltk_POPULATED)
  FetchContent_Populate(fltk)
  set(FLTK_BUILD_TEST OFF CACHE BOOL "" FORCE)
  add_subdirectory(${fltk_SOURCE_DIR} ${fltk_BINARY_DIR})
endif()

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
target_compile_options(gtest
	PRIVATE
		-stdlib=libc++
)
target_link_libraries(gtest
	PRIVATE
		c++
)
