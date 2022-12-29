find_program(CLANG_FORMAT clang-format)
if(NOT CLANG_FORMAT)
  message("clang-format not found, formatting target may not work properly")
  add_custom_target(
    clang-format
    COMMAND ./clang-format.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
  add_custom_target(
    clang-format
    COMMAND ./clang-format.sh ${CLANG_FORMAT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

find_program(CMAKE_FORMAT cmake-format)
if(NOT CMAKE_FORMAT)
  message("cmake-format not found, formatting target may not work properly")
  add_custom_target(
    cmake-format
    COMMAND ./cmake-format.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
  add_custom_target(
    cmake-format
    COMMAND ./cmake-format.sh ${CMAKE_FORMAT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_custom_target(format DEPENDS clang-format cmake-format)
