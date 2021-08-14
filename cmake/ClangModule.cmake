# Adds a module to the system.
#
# This code has originally been copied from
# https://stackoverflow.com/a/62499857.
# The usage of the library modules in the system is heavily inspired by the
# same post and the clang modules documentation.
function(add_module name)
    file(MAKE_DIRECTORY ${PREBUILT_MODULE_PATH})
    add_custom_target(${name}.pcm
            COMMAND
                ${CMAKE_CXX_COMPILER}
                -std=c++20
                -stdlib=libc++
                -fmodules
				-fprebuilt-module-path=${PREBUILT_MODULE_PATH}
                -c
                ${CMAKE_CURRENT_SOURCE_DIR}/${ARGN}
                -Xclang -emit-module-interface
                -o ${PREBUILT_MODULE_PATH}/${name}.pcm

            )
endfunction()
