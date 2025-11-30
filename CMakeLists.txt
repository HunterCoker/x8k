cmake_minimum_required(VERSION 3.23)
project(a8k_firmware C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(cmake/m480_hal.cmake)

add_subdirectory(libs)
add_subdirectory(src)

if(A8K_BUILD_TESTS AND EXISTS "${CMAKE_SOURCE_DIR}/tests")
    enable_testing()
    add_subdirectory(tests)
endif()

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(symlink_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()