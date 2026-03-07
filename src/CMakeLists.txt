set(A8K_TARGET ${PROJECT_NAME})

add_executable(${A8K_TARGET}
    "main.cpp"
    "retarget.cpp"
    ${M480_STARTUP_SRCS}
)

add_subdirectory(drivers)
add_subdirectory(sys)

target_include_directories(${A8K_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${A8K_TARGET}
    PRIVATE
        m480_hal
        drivers
        sys
)

target_compile_definitions(${A8K_TARGET}
    PRIVATE $<$<CONFIG:Debug>:DEBUG>
    PRIVATE $<$<CONFIG:Release>:NDEBUG>
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
