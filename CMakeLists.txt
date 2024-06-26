cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(picoTestProject C CXX ASM)
pico_sdk_init()

set(FAMILY rp2040)
set(BOARD pico_sdk)
set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")
add_subdirectory(${PICO_TINYUSB_PATH}/examples/device tinyusb_device_examples)

aux_source_directory(./main SOURCES)
add_executable(picoTestProject ${SOURCES})
#  init pio
pico_generate_pio_header(picoTestProject ${CMAKE_CURRENT_LIST_DIR}/main/spi_slave.pio)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/main)

# pull in common dependencies
# pico_enable_stdio_usb(Infineon_radar_pico 1)
# pico_enable_stdio_uart(Infineon_radar_pico 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(picoTestProject)


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