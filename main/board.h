#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "spi_slave.pio.h"
#include "pio_spi.h"

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

// SPI + DMA
void spi_DMA_init(void);                         // SPI接收DMA通道初始化
void spi_DMA_read(uint8_t *rx_data, size_t len); // 使用DMA通道从SPI接收数据
void spi_DMA_read_blocking(uint8_t *rx_data, size_t len);
void spi_DMA_write(uint8_t *tx_data, size_t len);
void spi_DMA_write_blocking(uint8_t *tx_data, size_t len);
void dma_irq_handler(); // SPI_DMA 中断处理函数

// UART + DMA
void uart_tx_DMA_init(void);                            // UART发送DMA通道初始化
void uartDma_send(uint8_t *data, size_t len);           // 使用DMA通道向UART发送数据
void uart_dma_send_blocking(uint8_t *data, size_t len); // 使用DMA通道向UART发送数据（阻塞，不会因为间隔过短造成数据丢失）

// PIO
void spi_slave_PIO_init(uint pin_mosi, uint pin_sck, uint clk_div);
void PioDMAInit(void);
void pio_spi_dma_send_blocking(uint8_t *data, size_t len);