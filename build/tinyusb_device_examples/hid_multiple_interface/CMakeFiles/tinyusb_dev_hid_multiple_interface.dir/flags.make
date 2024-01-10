# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

# compile ASM with C:/Users/28670/scoop/10 2021.10/bin/arm-none-eabi-gcc.exe
# compile C with C:/Users/28670/scoop/10 2021.10/bin/arm-none-eabi-gcc.exe
# compile CXX with C:/Users/28670/scoop/10 2021.10/bin/arm-none-eabi-g++.exe
ASM_DEFINES = -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DCFG_TUSB_OS=OPT_OS_PICO -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_BOOTSEL_VIA_DOUBLE_RESET=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FIX_RP2040_USB_DEVICE_ENUMERATION=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_UART=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_UTIL=1 -DLIB_TINYUSB_BOARD=1 -DLIB_TINYUSB_DEVICE=1 -DPICO_BOARD=\"pico\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Debug\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1 -DPICO_RP2040_USB_DEVICE_UFRAME_FIX=1 -DPICO_TARGET_NAME=\"tinyusb_dev_hid_multiple_interface\" -DPICO_USE_BLOCKED_RAM=0

ASM_INCLUDES = -ID:\Pico\pico-sdk\lib\tinyusb\examples\device\hid_multiple_interface\src -ID:\Pico\pico-sdk\src\common\pico_stdlib\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_gpio\include -ID:\Pico\pico-sdk\src\common\pico_base\include -IC:\Users\28670\Desktop\picoTestProject\build\generated\pico_base -ID:\Pico\pico-sdk\src\boards\include -ID:\Pico\pico-sdk\src\rp2_common\pico_platform\include -ID:\Pico\pico-sdk\src\rp2040\hardware_regs\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_base\include -ID:\Pico\pico-sdk\src\rp2040\hardware_structs\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_claim\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_sync\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_irq\include -ID:\Pico\pico-sdk\src\common\pico_sync\include -ID:\Pico\pico-sdk\src\common\pico_time\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_timer\include -ID:\Pico\pico-sdk\src\common\pico_util\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_uart\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_resets\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_clocks\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_pll\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_vreg\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_watchdog\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_xosc\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_divider\include -ID:\Pico\pico-sdk\src\rp2_common\pico_runtime\include -ID:\Pico\pico-sdk\src\rp2_common\pico_printf\include -ID:\Pico\pico-sdk\src\common\pico_bit_ops\include -ID:\Pico\pico-sdk\src\common\pico_divider\include -ID:\Pico\pico-sdk\src\rp2_common\pico_double\include -ID:\Pico\pico-sdk\src\rp2_common\pico_float\include -ID:\Pico\pico-sdk\src\rp2_common\pico_malloc\include -ID:\Pico\pico-sdk\src\rp2_common\pico_bootrom\include -ID:\Pico\pico-sdk\src\common\pico_binary_info\include -ID:\Pico\pico-sdk\src\rp2_common\pico_stdio\include -ID:\Pico\pico-sdk\src\rp2_common\pico_stdio_uart\include -ID:\Pico\pico-sdk\src\rp2_common\pico_int64_ops\include -ID:\Pico\pico-sdk\src\rp2_common\pico_mem_ops\include -ID:\Pico\pico-sdk\src\rp2_common\boot_stage2\include -ID:\Pico\pico-sdk\lib\tinyusb\src -ID:\Pico\pico-sdk\lib\tinyusb\src\common -ID:\Pico\pico-sdk\lib\tinyusb\hw -ID:\Pico\pico-sdk\src\rp2_common\pico_fix\rp2040_usb_device_enumeration\include

ASM_FLAGS = -mcpu=cortex-m0plus -mthumb -Og -g -Wall -Wextra -Werror -Wfatal-errors -Wdouble-promotion -Wfloat-equal -Wshadow -Wwrite-strings -Wsign-compare -Wmissing-format-attribute -Wunreachable-code -Wcast-align -Wcast-qual -Wnull-dereference -Wuninitialized -Wunused -Wredundant-decls -Wconversion -Wcast-function-type -Wstrict-overflow -Wno-strict-aliasing -ffunction-sections -fdata-sections

C_DEFINES = -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DCFG_TUSB_OS=OPT_OS_PICO -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_BOOTSEL_VIA_DOUBLE_RESET=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FIX_RP2040_USB_DEVICE_ENUMERATION=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_UART=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_UTIL=1 -DLIB_TINYUSB_BOARD=1 -DLIB_TINYUSB_DEVICE=1 -DPICO_BOARD=\"pico\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Debug\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1 -DPICO_RP2040_USB_DEVICE_UFRAME_FIX=1 -DPICO_TARGET_NAME=\"tinyusb_dev_hid_multiple_interface\" -DPICO_USE_BLOCKED_RAM=0

C_INCLUDES = -ID:\Pico\pico-sdk\lib\tinyusb\examples\device\hid_multiple_interface\src -ID:\Pico\pico-sdk\src\common\pico_stdlib\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_gpio\include -ID:\Pico\pico-sdk\src\common\pico_base\include -IC:\Users\28670\Desktop\picoTestProject\build\generated\pico_base -ID:\Pico\pico-sdk\src\boards\include -ID:\Pico\pico-sdk\src\rp2_common\pico_platform\include -ID:\Pico\pico-sdk\src\rp2040\hardware_regs\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_base\include -ID:\Pico\pico-sdk\src\rp2040\hardware_structs\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_claim\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_sync\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_irq\include -ID:\Pico\pico-sdk\src\common\pico_sync\include -ID:\Pico\pico-sdk\src\common\pico_time\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_timer\include -ID:\Pico\pico-sdk\src\common\pico_util\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_uart\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_resets\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_clocks\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_pll\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_vreg\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_watchdog\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_xosc\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_divider\include -ID:\Pico\pico-sdk\src\rp2_common\pico_runtime\include -ID:\Pico\pico-sdk\src\rp2_common\pico_printf\include -ID:\Pico\pico-sdk\src\common\pico_bit_ops\include -ID:\Pico\pico-sdk\src\common\pico_divider\include -ID:\Pico\pico-sdk\src\rp2_common\pico_double\include -ID:\Pico\pico-sdk\src\rp2_common\pico_float\include -ID:\Pico\pico-sdk\src\rp2_common\pico_malloc\include -ID:\Pico\pico-sdk\src\rp2_common\pico_bootrom\include -ID:\Pico\pico-sdk\src\common\pico_binary_info\include -ID:\Pico\pico-sdk\src\rp2_common\pico_stdio\include -ID:\Pico\pico-sdk\src\rp2_common\pico_stdio_uart\include -ID:\Pico\pico-sdk\src\rp2_common\pico_int64_ops\include -ID:\Pico\pico-sdk\src\rp2_common\pico_mem_ops\include -ID:\Pico\pico-sdk\src\rp2_common\boot_stage2\include -ID:\Pico\pico-sdk\lib\tinyusb\src -ID:\Pico\pico-sdk\lib\tinyusb\src\common -ID:\Pico\pico-sdk\lib\tinyusb\hw -ID:\Pico\pico-sdk\src\rp2_common\pico_fix\rp2040_usb_device_enumeration\include

C_FLAGS = -mcpu=cortex-m0plus -mthumb -Og -g -Wall -Wextra -Werror -Wfatal-errors -Wdouble-promotion -Wfloat-equal -Wshadow -Wwrite-strings -Wsign-compare -Wmissing-format-attribute -Wunreachable-code -Wcast-align -Wcast-qual -Wnull-dereference -Wuninitialized -Wunused -Wredundant-decls -Wconversion -Wcast-function-type -Wstrict-overflow -Wno-strict-aliasing -ffunction-sections -fdata-sections

CXX_DEFINES = -DCFG_TUSB_MCU=OPT_MCU_RP2040 -DCFG_TUSB_OS=OPT_OS_PICO -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_BOOTSEL_VIA_DOUBLE_RESET=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FIX_RP2040_USB_DEVICE_ENUMERATION=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_UART=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_UTIL=1 -DLIB_TINYUSB_BOARD=1 -DLIB_TINYUSB_DEVICE=1 -DPICO_BOARD=\"pico\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Debug\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1 -DPICO_RP2040_USB_DEVICE_UFRAME_FIX=1 -DPICO_TARGET_NAME=\"tinyusb_dev_hid_multiple_interface\" -DPICO_USE_BLOCKED_RAM=0

CXX_INCLUDES = -ID:\Pico\pico-sdk\lib\tinyusb\examples\device\hid_multiple_interface\src -ID:\Pico\pico-sdk\src\common\pico_stdlib\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_gpio\include -ID:\Pico\pico-sdk\src\common\pico_base\include -IC:\Users\28670\Desktop\picoTestProject\build\generated\pico_base -ID:\Pico\pico-sdk\src\boards\include -ID:\Pico\pico-sdk\src\rp2_common\pico_platform\include -ID:\Pico\pico-sdk\src\rp2040\hardware_regs\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_base\include -ID:\Pico\pico-sdk\src\rp2040\hardware_structs\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_claim\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_sync\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_irq\include -ID:\Pico\pico-sdk\src\common\pico_sync\include -ID:\Pico\pico-sdk\src\common\pico_time\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_timer\include -ID:\Pico\pico-sdk\src\common\pico_util\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_uart\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_resets\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_clocks\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_pll\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_vreg\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_watchdog\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_xosc\include -ID:\Pico\pico-sdk\src\rp2_common\hardware_divider\include -ID:\Pico\pico-sdk\src\rp2_common\pico_runtime\include -ID:\Pico\pico-sdk\src\rp2_common\pico_printf\include -ID:\Pico\pico-sdk\src\common\pico_bit_ops\include -ID:\Pico\pico-sdk\src\common\pico_divider\include -ID:\Pico\pico-sdk\src\rp2_common\pico_double\include -ID:\Pico\pico-sdk\src\rp2_common\pico_float\include -ID:\Pico\pico-sdk\src\rp2_common\pico_malloc\include -ID:\Pico\pico-sdk\src\rp2_common\pico_bootrom\include -ID:\Pico\pico-sdk\src\common\pico_binary_info\include -ID:\Pico\pico-sdk\src\rp2_common\pico_stdio\include -ID:\Pico\pico-sdk\src\rp2_common\pico_stdio_uart\include -ID:\Pico\pico-sdk\src\rp2_common\pico_int64_ops\include -ID:\Pico\pico-sdk\src\rp2_common\pico_mem_ops\include -ID:\Pico\pico-sdk\src\rp2_common\boot_stage2\include -ID:\Pico\pico-sdk\lib\tinyusb\src -ID:\Pico\pico-sdk\lib\tinyusb\src\common -ID:\Pico\pico-sdk\lib\tinyusb\hw -ID:\Pico\pico-sdk\src\rp2_common\pico_fix\rp2040_usb_device_enumeration\include

CXX_FLAGS = -mcpu=cortex-m0plus -mthumb -Og -g -Wall -Wextra -Werror -Wfatal-errors -Wdouble-promotion -Wfloat-equal -Wshadow -Wwrite-strings -Wsign-compare -Wmissing-format-attribute -Wunreachable-code -Wcast-align -Wcast-qual -Wnull-dereference -Wuninitialized -Wunused -Wredundant-decls -Wconversion -Wcast-function-type -Wstrict-overflow -Wno-strict-aliasing -ffunction-sections -fdata-sections -fno-exceptions -fno-unwind-tables -fno-rtti -fno-use-cxa-atexit

# Custom flags: tinyusb_device_examples/hid_multiple_interface/CMakeFiles/tinyusb_dev_hid_multiple_interface.dir/D_/Pico/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040/rp2040_usb.c.obj_FLAGS = -Wno-stringop-overflow -Wno-array-bounds

