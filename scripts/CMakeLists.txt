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

add_custom_target(format DEPENDS clang-format)
