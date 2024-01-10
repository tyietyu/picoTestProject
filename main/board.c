#include "board.h"
#include <string.h>
void radar_gpio_init()
{
    gpio_init(LED0_PIN);
    gpio_set_dir(LED0_PIN, GPIO_OUT);
    gpio_put(LED0_PIN, 1);

    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_put(LED1_PIN, 0);
}

void radar_uart_init()
{
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

void __no_inline_not_in_flash_func(WriteFlashData)(uint8_t *data, size_t size)
{
    // 擦除扇区
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_ADD_OFFSET, FLASH_PAGE_SIZE);
    flash_range_program(FLASH_ADD_OFFSET, data, size);
    restore_interrupts(ints);
}

void ReadFlashData(uint8_t *data, uint8_t size)
{
    const uint8_t *flash_data = (const uint8_t *)(XIP_BASE + FLASH_ADD_OFFSET);
    memcpy(data, flash_data, size);
}