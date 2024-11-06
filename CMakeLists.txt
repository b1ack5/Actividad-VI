cmake_minimum_required(VERSION 3.13)
project(display_project C CXX ASM)
include(pico_sdk_import.cmake)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(display display.c)

target_link_libraries (display
    pico_stdlib        
    hardware_i2c       
)

pico_enable_stdio_usb(display 1) 
pico_enable_stdio_uart(display 0) 
pico_add_extra_outputs(display)
