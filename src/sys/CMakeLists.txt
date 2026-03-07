add_library(sys
    STATIC
        descriptors.c
)

target_include_directories(drivers
    PUBLIC
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(sys
    PRIVATE
        m480_hal
)