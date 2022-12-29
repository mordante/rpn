function(format VAR name)
  find_program(VAR ${name})
  if(NOT VAR)
    message("${name} not found, formatting target may not work properly")
    add_custom_target(
      ${name}
      COMMAND ./${name}.sh
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  else()
    add_custom_target(
      ${name}
      COMMAND ./${name}.sh ${${VAR}}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
endfunction()

format(CLANG_FORMAT clang-format)
format(CMAKE_FORMAT cmake-format)
format(BLACK black)
add_custom_target(format DEPENDS clang-format cmake-format black)
