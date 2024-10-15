# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(picoTestProject C CXX ASM)
pico_sdk_init()

set(FAMILY rp2040)
set(BOARD pico_sdk)
set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")
add_subdirectory(${PICO_TINYUSB_PATH}/examples/device tinyusb_device_examples)

aux_source_directory(./main SOURCES)
aux_source_directory(./FreeRTOS SOURCES)

add_executable(picoTestProject ${SOURCES})

#  init pio
pico_generate_pio_header(picoTestProject ${CMAKE_CURRENT_LIST_DIR}/main/spi_slave.pio)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/portable)

# pull in common dependencies
# pico_enable_stdio_usb(Infineon_radar_pico 1)
# pico_enable_stdio_uart(Infineon_radar_pico 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(picoTestProject)

target_compile_definitions(picoTestProject PRIVATE
                    PICO_STACK_SIZE=20480 
                    PICO_CORE1_STACK_SIZE=2048)

# hardware_timer
target_link_libraries(picoTestProject
                    pico_time
                    hardware_spi
                    hardware_uart 
                    hardware_resets 
                    hardware_irq 
                    hardware_flash
                    hardware_pio
                    hardware_dma
                    pico_stdlib 
                    pico_unique_id 
                    tinyusb_device 
                    tinyusb_board 
                    pico_multicore)