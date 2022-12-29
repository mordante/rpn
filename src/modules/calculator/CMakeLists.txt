add_library(
  calculator
  calculator.cpp
  controller.cpp
  model.cpp
  # TODO Evaluate whether this needs its own module
  math/arithmetic.cpp
  math/bitwise.cpp
  math/core.cpp
  math/round.cpp
  stack.cpp
  transaction.cpp
  undo_handler.cpp
  value.cpp)
target_compile_options(calculator PRIVATE ${module_compile_options}
                                          ${diagnostic_compile_options})
set_target_properties(calculator PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY}")
target_link_libraries(calculator PRIVATE lib parser)
add_module(calculator calculator.cpp)
add_module(calculator.controller controller.cpp)
add_module(calculator.math.core math/core.cpp)
add_module(calculator.math.arithmetic math/arithmetic.cpp)
add_module(calculator.math.bitwise math/bitwise.cpp)
add_module(calculator.math.round math/round.cpp)
add_module(calculator.model model.cpp)
add_module(calculator.stack stack.cpp)
add_module(calculator.transaction transaction.cpp)
add_module(calculator.undo_handler undo_handler.cpp)
add_module(calculator.value value.cpp)
add_dependencies(calculator.math.arithmetic.pcm calculator.math.core.pcm)
add_dependencies(calculator.math.bitwise.pcm calculator.math.core.pcm)
add_dependencies(calculator.math.round.pcm calculator.math.core.pcm)
add_dependencies(
  calculator.value.pcm calculator.math.arithmetic.pcm
  calculator.math.bitwise.pcm calculator.math.core.pcm
  calculator.math.round.pcm)
add_dependencies(calculator.stack.pcm calculator.value.pcm)
add_dependencies(calculator.model.pcm parser.pcm calculator.stack.pcm)
add_dependencies(calculator.transaction.pcm calculator.model.pcm)
add_dependencies(calculator.undo_handler.pcm calculator.transaction.pcm)
add_dependencies(
  calculator.controller.pcm calculator.model.pcm calculator.transaction.pcm
  calculator.undo_handler.pcm lib.dictionary.pcm)
add_dependencies(calculator.pcm calculator.controller.pcm calculator.model.pcm
                 calculator.value.pcm)
add_dependencies(calculator calculator.pcm)
