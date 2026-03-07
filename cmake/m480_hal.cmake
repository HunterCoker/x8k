set(M480BSP_LIBRARY_DIR
    ${CMAKE_CURRENT_LIST_DIR}/../libs/M480BSP/Library
)

add_library(m480_hal STATIC)

target_sources(m480_hal
    PRIVATE
        "${M480BSP_LIBRARY_DIR}/StdDriver/src/sys.c"
        "${M480BSP_LIBRARY_DIR}/StdDriver/src/clk.c"
        "${M480BSP_LIBRARY_DIR}/StdDriver/src/gpio.c"
        "${M480BSP_LIBRARY_DIR}/StdDriver/src/hsusbd.c"
        "${M480BSP_LIBRARY_DIR}/StdDriver/src/usbd.c"
        "${M480BSP_LIBRARY_DIR}/StdDriver/src/uart.c"
        "${M480BSP_LIBRARY_DIR}/StdDriver/src/spi.c"
)

target_include_directories(m480_hal
    SYSTEM PUBLIC
        "${M480BSP_LIBRARY_DIR}/CMSIS/Include"
        "${M480BSP_LIBRARY_DIR}/Device/Nuvoton/M480/Include"
        "${M480BSP_LIBRARY_DIR}/StdDriver/inc"
)

target_compile_options(m480_hal
    PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-register>
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>
)

set(M480_STARTUP_SRCS
    "${M480BSP_LIBRARY_DIR}/Device/Nuvoton/M480/Source/system_M480.c"
    "${M480BSP_LIBRARY_DIR}/Device/Nuvoton/M480/Source/GCC/startup_M480.S"
)