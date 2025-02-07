include(FetchContent)

FetchContent_Declare(
  fltk
  GIT_REPOSITORY https://github.com/fltk/fltk
  GIT_TAG        release-1.3.8
  GIT_SHALLOW    1
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
  GIT_SHALLOW    1
)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # The 3.0.0 release doesn't build with libc++16 due to usage of deprecated
  # constructs. Using a tag prevents using GIT_SHALLOW
  GIT_TAG 60b9e49
)
FetchContent_MakeAvailable(fltk googletest ftxui)
