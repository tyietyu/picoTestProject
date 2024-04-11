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

void radar_spi_init()
{
    spi_init(SPI_PORT, SPI_BAUD_RATE);
    /**********************手动修改spi的模式************************************/
    // Clear the current FRF field value
    hw_clear_bits(&spi_get_hw(SPI_PORT)->cr0, SPI_SSPCR0_FRF_BITS);
    // Set the new FRF field value to National Microwire format (10)
    hw_set_bits(&spi_get_hw(SPI_PORT)->cr0, (2 << SPI_SSPCR0_FRF_LSB));
    /**********************************************************/
    spi_set_slave(SPI_PORT, true);
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_CSN_PIN, GPIO_FUNC_SPI);
}