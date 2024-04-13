#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "hardware/irq.h"
#include "hardware/pio.h"

#define TIMER_INDEX 0
#define TIMER_INTERVAL_MS 20000 // 10s
#define IRQ_PIN 5

#define LED0_PIN 25
#define LED1_PIN 24

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_RX_PIN 17
#define UART_TX_PIN 16

#define SPI_PORT spi0
#define SPI_BAUD_RATE 25000000
#define SPI_RX_PIN 0
#define SPI_CSN_PIN 1
#define SPI_SCK_PIN 2
#define SPI_TX_PIN 3


void radar_gpio_init();
void radar_uart_init();
void radar_spi_init();