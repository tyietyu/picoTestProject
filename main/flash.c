#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "flash.h"

/* ***** GENERAL FUNCTION ***** */
/**
 * @brief Erase Flash Sector
 *
 * @param flash_offs The Flash offset address to be erased is automatically rounded up to the sector size.
 * @param count      The number of bytes to be erased is automatically rounded up to the sector size
 */
void flashErase(uint32_t flash_offs, uint16_t count)
{
    // sector size
    const size_t sector_size = 4096;

    // calculate the start sector number
    uint32_t start_sector = flash_offs / sector_size;

    // calculate end sector number
    uint32_t end_sector = (flash_offs + count - 1) / sector_size;

    // calculate the number of sectors to erase
    size_t sectors_to_erase = end_sector - start_sector + 1;

    // calculate the actual offset address and the number of bytes to erase
    uint32_t actual_offset = start_sector * sector_size;
    size_t actual_bytes_to_erase = sectors_to_erase * sector_size;

    // Disable interrupts while erasing the flash
    uint32_t ints = save_and_disable_interrupts();

    // Erase the specified range in flash
    flash_range_erase(actual_offset, actual_bytes_to_erase);

    // Restore interrupts
    restore_interrupts(ints);
}

/**
 * @brief 写入Flash数据
 *
 * @param flash_offs    Flash偏移地址，必须是页大小对齐
 * @param data          数据
 * @param count         数据长度，必须是页大小的倍数
 * @return uint8_t      0:成功，1:偏移地址不对齐，2:写入失败
 */
uint8_t flashProgram(uint32_t flash_offs, const uint8_t *data, uint16_t count)
{
    // 确保偏移地址和长度对齐
    if (flash_offs % FLASH_PAGE_SIZE != 0 || count % FLASH_PAGE_SIZE != 0)
    {
        printf("Error: Offset and length must be aligned to FLASH_PAGE_SIZE\n");
        return 1;
    }

    // Disable interrupts while erasing the flash
    uint32_t ints = save_and_disable_interrupts();

    flash_range_program(flash_offs, data, count);

    // Restore interrupts
    restore_interrupts(ints);

    // 检查写入结果
    for (size_t i = 0; i < count; i++)
    {
        if (*(uint8_t *)(XIP_BASE + flash_offs + i) != data[i])
        {
            printf("Error: Write failed at offset 0x%08x\n", flash_offs + i);
            return 2;
        }
    }
    return 0;
}

/**
 * @brief 写入Flash数据，不足页大小部分用0填充，但是地址必须是页大小对齐
 *
 * @param flash_offs 写入的Flash偏移地址，必须是页大小对齐
 * @param data       写入的数据
 * @param count      写入的数据长度，可以是任意大小，不足页大小部分用0填充
 * @return uint8_t   0:成功，1:偏移地址未对齐，2:内存分配失败，3:写入失败
 */
uint8_t flashProgramWithPadding(uint32_t flash_offs, const uint8_t *data, uint16_t count)
{
    // 确保偏移地址对齐到页大小
    if (flash_offs % FLASH_PAGE_SIZE != 0)
    {
        return 1; // 返回错误码，偏移地址未对齐
    }

    // 计算需要写入的总字节数（对齐到页大小）
    uint16_t total_bytes = (count + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE * FLASH_PAGE_SIZE;

    // 创建一个临时缓冲区，用于存储对齐后的数据
    uint8_t *aligned_data = (uint8_t *)malloc(total_bytes);
    if (aligned_data == NULL)
    {
        return 2; // 返回错误码，内存分配失败
    }

    // 将原始数据复制到临时缓冲区，并在不足页大小时补0
    memcpy(aligned_data, data, count);
    memset(aligned_data + count, 0, total_bytes - count);

    // 禁用中断，以防止并发访问
    uint32_t int_status = save_and_disable_interrupts();

    // 执行写入操作
    flash_range_program(flash_offs, aligned_data, total_bytes);

    // 恢复中断状态
    restore_interrupts(int_status);

    // 释放临时缓冲区
    free(aligned_data);

    // 检查写入结果
    for (size_t i = 0; i < count; i++)
    {
        if (*(uint8_t *)(XIP_BASE + flash_offs + i) != data[i])
        {
            return 3; // 返回错误码，写入失败
        }
    }

    return 0; // 返回成功码
}

/**
 * @brief 读取Flash数据
 *
 * @param flash_offs  Flash偏移地址
 * @param data        数据存储位置
 * @param count       数据长度
 * @return uint8_t    0:成功，1:参数错误
 */
uint8_t getFlashData(uint32_t flash_offs, uint8_t *data, uint16_t count)
{
    // 检查参数有效性
    if (flash_offs >= FLASH_ALL_SIZE || count == 0 || data == NULL)
    {
        return 1; // 返回错误码
    }

    // 从Flash存储器读取数据
    memcpy(data, (uint8_t *)(XIP_BASE + flash_offs), count);

    return 0; // 返回成功码
}
/* ***** GENERAL FUNCTION ***** */

/* ***** 专用函数 ***** */

/**
 * @brief Convert 16-bit unsigned integers to byte arrays
 *
 * @param count   16-bit unsigned integer
 * @param buffer  byte array
 */
void convertToByteArray(uint16_t count, uint8_t *buffer)
{
    buffer[0] = (count >> 8) & 0xFF; // high byte
    buffer[1] = count & 0xFF;        // low byte
}

/**
 * @brief Get the Flash Data Length object
 *
 * @param flash_offs  Flash offset address
 * @return uint16_t   Data length
 */
uint16_t getFlashDataLength(uint32_t flash_offs)
{
    uint8_t high_byte = *(uint8_t *)(XIP_BASE + flash_offs);
    uint8_t low_byte = *(uint8_t *)(XIP_BASE + flash_offs + 1);
    return ((high_byte << 8) | low_byte);
}

/**
 * @brief Verify whether Flash data is valid
 *
 * @param flash_offs  Flash offset address
 * @return uint8_t    0: invalid, 1: valid
 */
uint8_t checkMagicNum(uint32_t flash_offs)
{
    uint16_t data_len = getFlashDataLength(flash_offs);
    uint8_t magic_num = *(uint8_t *)(XIP_BASE + flash_offs + data_len + 2);

    return (magic_num == FLASH_MAGIC_NUM);
}

/**
 * @brief 写入Flash数据，不足页大小部分用0填充，但是地址必须是页大小对齐，
 *        并在数据开头添加2字节数据长度，在数据结尾添加1字节的魔术数
 *
 * @param flash_offs Flash偏移地址
 * @param data       数据
 * @param count      数据长度
 * @return uint8_t   0:成功，1:偏移地址未对齐，2:内存分配失败，3:写入失败，4:备份失败
 */
uint8_t writeFlashDataWithPadding(uint32_t flash_offs, const uint8_t *data, uint16_t count)
{
    // 确保偏移地址对齐到页大小
    if (flash_offs % FLASH_PAGE_SIZE != 0)
    {
        return 1; // 返回错误码，偏移地址未对齐
    }

    // 计算需要写入的总字节数（对齐到页大小），额外的3字节（2字节数据长度，1字节魔术数）
    uint16_t total_bytes = (count + FLASH_PAGE_SIZE - 1 + 3) / FLASH_PAGE_SIZE * FLASH_PAGE_SIZE;

    // 备份数据
    uint8_t res;
    res = backupFlashData(FLASH_PROGRAM_ADDRESS);
    // 备份成功或者格式不对无需备份则继续
    if (!(res == 0 || res == 4))
    {
        return 4; // 返回错误码，备份失败
    }

    // 创建一个临时缓冲区，用于存储对齐后的数据
    uint8_t *aligned_data = (uint8_t *)malloc(total_bytes);
    if (aligned_data == NULL)
    {
        return 2; // 返回错误码，内存分配失败
    }

    // 填充数据长度
    convertToByteArray(count, aligned_data);
    // 将原始数据复制到临时缓冲区
    memcpy(aligned_data + 2, data, count);
    // 填充魔术数
    aligned_data[2 + count] = FLASH_MAGIC_NUM;
    // 如果数据长度不足页大小，则用0填充
    memset(aligned_data + count + 3, 0, total_bytes - count - 3);

    // 禁用中断，以防止并发访问
    uint32_t int_status = save_and_disable_interrupts();

    // 执行写入操作
    flash_range_program(flash_offs, aligned_data, total_bytes);

    // 恢复中断状态
    restore_interrupts(int_status);

    // 释放临时缓冲区
    free(aligned_data);

    // 检查写入结果
    for (size_t i = 0; i < count; i++)
    {
        if (*(uint8_t *)(XIP_BASE + flash_offs + i + 2) != data[i]) // 注意2字节数据长度的偏移
        {
            return 3; // 返回错误码，写入失败
        }
    }

    return 0; // 返回成功码
}

/**
 * @brief 用于备份Flash数据（必须是规定的格式，包含数据长度和魔术数），将数据备份到备份区
 *
 * @param flash_offs 原数据的Flash偏移地址，必须是页大小对齐
 * @return uint8_t   0:成功，1:偏移地址未对齐，2:内存分配失败，3:写入失败，4：该扇区数据格式不正确
 */
uint8_t backupFlashData(uint32_t flash_offs)
{
    // Ensure that the offset address is aligned to the page size
    if (flash_offs % FLASH_PAGE_SIZE != 0)
    {
        // Returns an error code, the offset address is not aligned
        return 1;
    }

    // 检测Flash数据有效性
    if (!checkMagicNum(flash_offs))
    {
        return 4; // 返回错误码，数据格式不正确
    }

    uint16_t data_len = getFlashDataLength(flash_offs); // 获取Flash数据长度
    data_len += 3;                                      // 包括数据长度和魔术数

    // 创建一个临时缓冲区，用于存储数据
    uint8_t *temp_buffer = (uint8_t *)malloc(data_len);
    if (temp_buffer == NULL)
    {
        return 2; // 返回错误码，内存分配失败
    }
    // Read data from Flash memory
    memcpy(temp_buffer, (uint8_t *)(XIP_BASE + flash_offs), data_len);
    // Empty the backup area
    flashErase(flash_offs + FLASH_PROGRAM_BACKUP_OFFSET, data_len);

    uint8_t state = flashProgramWithPadding(flash_offs + FLASH_PROGRAM_BACKUP_OFFSET, temp_buffer, data_len);

    free(temp_buffer); // Release temporary buffer

    return state; // Return result code
}
/* ***** 专用函数 ***** */
// void flash_test()
// {
//     flashErase(FLASH_PROGRAM_ADDRESS, FLASH_PAGE_SIZE);
//     flashProgram(FLASH_PROGRAM_ADDRESS, databuf, sizeof(databuf));
//     getFlashData(FLASH_PROGRAM_ADDRESS, Flash_Buf, sizeof(Flash_Buf));
// }