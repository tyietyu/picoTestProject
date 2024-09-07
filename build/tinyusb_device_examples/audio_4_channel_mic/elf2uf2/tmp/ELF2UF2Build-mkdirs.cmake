# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Pico/pico-sdk/tools/elf2uf2"
  "D:/Lc-Work/Pico_Software/picoTestProject/build/elf2uf2"
  "D:/Lc-Work/Pico_Software/picoTestProject/build/tinyusb_device_examples/audio_4_channel_mic/elf2uf2"
  "D:/Lc-Work/Pico_Software/picoTestProject/build/tinyusb_device_examples/audio_4_channel_mic/elf2uf2/tmp"
  "D:/Lc-Work/Pico_Software/picoTestProject/build/tinyusb_device_examples/audio_4_channel_mic/elf2uf2/src/ELF2UF2Build-stamp"
  "D:/Lc-Work/Pico_Software/picoTestProject/build/tinyusb_device_examples/audio_4_channel_mic/elf2uf2/src"
  "D:/Lc-Work/Pico_Software/picoTestProject/build/tinyusb_device_examples/audio_4_channel_mic/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Lc-Work/Pico_Software/picoTestProject/build/tinyusb_device_examples/audio_4_channel_mic/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Lc-Work/Pico_Software/picoTestProject/build/tinyusb_device_examples/audio_4_channel_mic/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
