add_library(
  parser
  detail/base.cpp
  detail/floating_point.cpp
  detail/invalid_value.cpp
  detail/signed.cpp
  detail/string.cpp
  detail/unsigned.cpp
  parser.cpp
  token.cpp)
target_compile_options(parser PRIVATE ${module_compile_options}
                                      ${diagnostic_compile_options})
set_target_properties(parser PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY}")
add_module(parser.detail.base detail/base.cpp)
add_module(parser.detail.floating_point detail/floating_point.cpp)
add_module(parser.detail.invalid_value detail/invalid_value.cpp)
add_module(parser.detail.signed_value detail/signed.cpp)
add_module(parser.detail.string detail/string.cpp)
add_module(parser.detail.unsigned_value detail/unsigned.cpp)
add_module(parser.token token.cpp)
add_module(parser parser.cpp)
add_dependencies(parser.detail.base.pcm parser.token.pcm)
add_dependencies(parser.detail.floating_point.pcm parser.detail.base.pcm
                 parser.detail.invalid_value.pcm)
add_dependencies(parser.detail.invalid_value.pcm parser.detail.base.pcm)
add_dependencies(parser.detail.signed_value.pcm parser.detail.base.pcm
                 parser.detail.invalid_value.pcm parser.detail.string.pcm)
add_dependencies(parser.detail.string.pcm parser.detail.base.pcm)
add_dependencies(
  parser.detail.unsigned_value.pcm calculator.stack.pcm parser.detail.base.pcm
  parser.detail.invalid_value.pcm parser.detail.floating_point.pcm)
add_dependencies(
  parser.pcm
  parser.detail.base.pcm
  parser.detail.floating_point.pcm
  parser.detail.invalid_value.pcm
  parser.detail.signed_value.pcm
  parser.detail.string.pcm
  parser.detail.unsigned_value.pcm
  parser.token.pcm)
add_dependencies(parser parser.pcm)
