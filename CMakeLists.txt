# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico CACHE STRING "Board type")

set(MS_RATE_DEFAULT 100 CACHE STRING "Default mouse sample rate")
set(MS_RATE_HOST_CONTROL ON CACHE BOOL "Allow the host to configure the mouse sample rate")

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.1")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.1 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(ps2x2pico C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(ps2x2pico src/ps2x2pico.c src/scancodesets.c src/ps2kb.c src/ps2ms.c src/ps2out.c src/ps2in.c  src/ps2gamepad.c src/espec_to_ps2.c src/neopixel.c src/xinput_host.c src/ps2x360.c src/DB9.c)

pico_generate_pio_header(ps2x2pico ${CMAKE_CURRENT_LIST_DIR}/src/ps2out.pio)
pico_generate_pio_header(ps2x2pico ${CMAKE_CURRENT_LIST_DIR}/src/ps2in.pio)
pico_generate_pio_header(ps2x2pico ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio  OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

### PS/2 output
add_compile_definitions(LVOUT=13) # Level shifter pull-up voltage //Pin para proporcionar 3v en el LevelShifter
add_compile_definitions(KBOUT=11) # Keyboard data pin 11, clock pin is 12
add_compile_definitions(MSOUT=14) # Mouse data pin 14, clock pin is 15

### PS/2 input //Esto no lo usamos, pero se queda porque no molesta y hay referencias en el codigo, sirve para hacer el passtrough ps2 en el codigo de NoOne
add_compile_definitions(LVIN=5) # Level shifter pull-up voltage
add_compile_definitions(KBIN=3) # Keyboard data pin 3, clock pin is 4
add_compile_definitions(MSIN=6) # Mouse data pin 6, clock pin is 7

add_compile_definitions(PICO_PANIC_FUNCTION=reset)

add_compile_definitions(MS_RATE_DEFAULT=${MS_RATE_DEFAULT})
if (MS_RATE_HOST_CONTROL)
    add_compile_definitions(MS_RATE_HOST_CONTROL)
endif()

###Xinput_host soporte para gamepad xbox 360

add_library(xinput_host INTERFACE)
target_sources(xinput_host INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}src/xinput_host.c
)
target_include_directories(xinput_host INTERFACE ${CMAKE_CURRENT_LIST_DIR}src)
target_link_libraries(xinput_host INTERFACE tinyusb)

### /Xinput_host soporte para gamepad xbox 360/


pico_set_program_name(ps2x2pico "ps2x2pico-joystick_2xDB9_USB(KeyBoard_Mouse_GmPaD_X360(GmPaD¬Mouse))")
pico_set_program_version(ps2x2pico "Version-202500828")

pico_enable_stdio_uart(ps2x2pico 1)
pico_enable_stdio_usb(ps2x2pico 0)

target_include_directories(ps2x2pico PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(ps2x2pico pico_stdlib hardware_pio tinyusb_host tinyusb_board)

pico_add_extra_outputs(ps2x2pico)
