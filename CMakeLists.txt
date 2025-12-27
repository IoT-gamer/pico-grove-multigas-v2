cmake_minimum_required(VERSION 3.13)

# Define the library target
add_library(grove_multigas_v2 INTERFACE)

# Add the source file to a backing target (SDK style)
target_sources(grove_multigas_v2 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/grove_multigas_v2.c
)

# Export the include directory
target_include_directories(grove_multigas_v2 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Link against Pico SDK I2C and standard libraries
target_link_libraries(grove_multigas_v2 INTERFACE
    pico_stdlib
    hardware_i2c
)