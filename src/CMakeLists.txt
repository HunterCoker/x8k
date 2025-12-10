set(A8K_TARGET ${PROJECT_NAME})

add_executable(${A8K_TARGET}
    ${M480_STARTUP}
    "main.cpp"
    "sys/sys_init.c"
    "sys/uart_init.c"
    "sys/hid_mouse.c"
    "sys/descriptors.c"
    "sys/retarget.c"
)

target_include_directories(${A8K_TARGET}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${A8K_TARGET}
    PRIVATE m480_hal
)

target_compile_definitions(${A8K_TARGET}
    PRIVATE $<$<CONFIG:Debug>:DEBUG>
    PRIVATE $<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(${A8K_TARGET}
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-register>
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>
)

target_link_options(${A8K_TARGET}
    PRIVATE
        -T${CMAKE_SOURCE_DIR}/config/gcc_arm.ld
        -Wl,--gc-sections
)

set_target_properties(${A8K_TARGET} PROPERTIES SUFFIX ".elf")

# add_custom_command(TARGET ${A8K_TARGET} POST_BUILD
#     COMMAND arm-none-eabi-objcopy -O binary
#             $<TARGET_FILE:${A8K_TARGET}>
#             ${CMAKE_BINARY_DIR}/${A8K_TARGET}.bin
#     COMMENT "Generating ${A8K_TARGET}.bin"
# )
