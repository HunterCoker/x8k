# Create a static library for the M480 HAL from the BSP
set(M480BSP_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/libs/M480BSP/Library)

add_library(m480_hal STATIC)

target_sources(m480_hal
    PRIVATE
        ${M480BSP_LIBRARY_DIR}/StdDriver/src/sys.c
        ${M480BSP_LIBRARY_DIR}/StdDriver/src/clk.c
        ${M480BSP_LIBRARY_DIR}/StdDriver/src/gpio.c
        ${M480BSP_LIBRARY_DIR}/StdDriver/src/usbd.c
)

target_include_directories(m480_hal
    PUBLIC
        ${M480BSP_LIBRARY_DIR}/CMSIS/Include
        ${M480BSP_LIBRARY_DIR}/Device/Nuvoton/M480/Include
        ${M480BSP_LIBRARY_DIR}/StdDriver/inc
)

# Expose startup files to parent so executable can use them
set(M480_STARTUP
    ${M480BSP_LIBRARY_DIR}/Device/Nuvoton/M480/Source/system_M480.c
    ${M480BSP_LIBRARY_DIR}/Device/Nuvoton/M480/Source/GCC/startup_M480.S
)