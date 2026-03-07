add_library(drivers
    STATIC
        "drivers.h"
        "drivers.cpp"
        "architecture/clock.h"
        "architecture/clock.cpp"
        "communication/serial/uart.h"
        "communication/serial/uart.cpp"
        "communication/serial/spi.h"
        "communication/serial/spi.cpp"
        "communication/serial/fsusb.h"
        "communication/serial/fsusb.cpp"
        "communication/serial/hsusb.h"
        "communication/serial/hsusb.cpp"
        "communication/sensors/paw3395.h"
        "communication/sensors/paw3395.cpp"
)

target_include_directories(drivers
    PUBLIC
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(drivers
    PRIVATE
        m480_hal
)