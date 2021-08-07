include(FetchContent)

#FetchContent_Declare(
#  imgui
#  GIT_REPOSITORY https://github.com/ocornut/imgui
#  GIT_TAG v1.83
#)
#FetchContent_GetProperties(imgui)
#if(NOT imgui_POPULATED)
#  FetchContent_Populate(imgui)
##  add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR})
#endif()

FetchContent_Declare(
  fltk
  GIT_REPOSITORY https://github.com/fltk/fltk
  GIT_TAG        release-1.3.7
)
FetchContent_GetProperties(fltk)
if(NOT fltk_POPULATED)
  FetchContent_Populate(fltk)
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
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
