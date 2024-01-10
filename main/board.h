#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "hardware/irq.h"

#define TIMER_INDEX 0
#define TIMER_INTERVAL_MS 20000 // 10s
#define IRQ_PIN 5

#define LED0_PIN 25
#define LED1_PIN 24

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_RX_PIN 17
#define UART_TX_PIN 16

#define FLASH_INSIDE_PROGRAM_SIZE (16 * 1024 * 1024)
#define FLASH_ADD_OFFSET (FLASH_INSIDE_PROGRAM_SIZE - FLASH_SECTOR_SIZE) // 基地址的偏移量 表示从此偏移量后开始储存数据

void ReadFlashData(uint8_t *data, uint8_t size);
void __no_inline_not_in_flash_func(WriteFlashData)(uint8_t *data, size_t size);

void radar_gpio_init();
void radar_uart_init();
