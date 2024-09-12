#ifndef _FLASH_H_
#define _FLASH_H_

#include <stdint.h>

#define FLASH_MAGIC_NUM 0x55 // 魔术数
#define FLASH_ALL_SIZE PICO_FLASH_SIZE_BYTES
#define FLASH_PROGRAM_ADDRESS (1 * 1024 * 1024)                           // 1-1.5M的位置用于存储数据
#define FLASH_PROGRAM_BACKUP_OFFSET (512 * 1024)                          // 备份数据区相对于数据区的偏移量
#define FLASH_PROGRAM_BACKUP_ADDRESS (FLASH_PROGRAM_ADDRESS + 512 * 1024) // 1.5-2M的位置用于备份数据，防止丢失

typedef struct
{
    uint16_t len;    // data length
    uint8_t data[6]; // fixed-sized array
    uint8_t tag;     // magic number
} flash_data_t;

// 通用函数，包括擦除、写入和读取flash
void flashErase(uint32_t flash_offs, uint16_t count);
uint8_t flashProgram(uint32_t flash_offs, const uint8_t *data, uint16_t count);
uint8_t flashProgramWithPadding(uint32_t flash_offs, const uint8_t *data, uint16_t count);
uint8_t getFlashData(uint32_t flash_offs, uint8_t *data, uint16_t count);

// 专用函数，包括魔术数、数据长度的写入、读取、备份、校验等
// 约定如下：写入时，扇区的前2个字节是数据的长度，数据的后一个字节是魔术数
// 0x10700000 - 0x1077FFFF  512KB   数据区
// 0x10780000 - 0x10800000  512KB   备份数据区
void convertToByteArray(uint16_t count, uint8_t *buffer);
uint8_t writeFlashDataWithPadding(uint32_t flash_offs, const uint8_t *data, uint16_t count);
uint8_t checkMagicNum(uint32_t flash_offs);
uint16_t getFlashDataLength(uint32_t flash_offs);
uint8_t backupFlashData(uint32_t flash_offs);

#endif