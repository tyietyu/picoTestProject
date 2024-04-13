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

void spi_slave_init(PIO pio, uint sm, uint pin_mosi, uint pin_sck, uint pin_cs) {
    // 加载编译好的PIO程序到指定的PIO实例中
    uint offset = pio_add_program(pio, &spi_slave_program);

    // 根据实际的引脚配置，初始化PIO状态机
    pio_sm_config c = spi_slave_program_get_default_config(offset);

    // 设置用于数据输入的引脚（MOSI）
    sm_config_set_in_pins(&c, pin_mosi);

    // 将SCK引脚设置为时钟输入
    sm_config_set_sclk_pin(&c, pin_sck);

    // 将CS引脚设置为片选信号
    sm_config_set_in_pins(&c, pin_cs);
    pio_gpio_init(pio, pin_cs);
    pio_sm_set_consecutive_pindirs(pio, sm, pin_cs, 1, false);

    // 若需要，设置侧面设置（side-set）以驱动MISO引脚

    // 初始化状态机的时钟频率，这里没有设置分频因为需要跟踪主机的时钟
    sm_config_set_clkdiv(&c, 1);

    // 设置状态机的FIFO接收条件
    sm_config_set_in_shift(&c, true, true, 8);

    // 将配置应用于PIO状态机并启动
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

uint8_t spi_slave_receive_byte(PIO pio, uint sm) {
    // 等待，直到接收到新数据
    while (pio_sm_is_rx_fifo_empty(pio, sm)) {
        tight_loop_contents();
    }
    // 从RX FIFO中读取数据
    return pio_sm_get(pio, sm);
}

