#include "board.h"
#include <string.h>

// SPI + DMA
int dma_spi_rx_chan = 0; // SPI接收DMA通道
int dma_spi_tx_chan = 0;
dma_channel_config spi_rx_dma_config; // SPI接收DMA通道配置
dma_channel_config spi_tx_dma_config;

// UART + DMA
int dma_tx_chan = 0;                   // UART发送DMA通道
dma_channel_config uart_tx_dma_config; // UART发送DMA通道配置
int pio_rx_chan = 0;
bool cs_irq_flag = false;

extern pio_spi_inst_t spi;
pio_spi_inst_t spi = {
    .pio = pio0,
    .sm = 0,
    .clk_div = 1,
    .cs_pin = SPI_CSN_PIN};

void radar_gpio_init()
{
    gpio_init(LED0_PIN);
    gpio_set_dir(LED0_PIN, GPIO_OUT);
    gpio_put(LED0_PIN, 1);

    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_put(LED1_PIN, 0);

    gpio_init(SPI_CSN_PIN);
    gpio_set_dir(SPI_CSN_PIN, GPIO_IN);
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
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_CSN_PIN, GPIO_FUNC_SPI);
}

/* SPI + DMA */
/**
 * @brief 初始化SPI DMA通道
 */
void spi_DMA_init(void)
{
    // 获取一个空闲的DMA通道
    dma_spi_tx_chan = dma_claim_unused_channel(true);
    dma_spi_rx_chan = dma_claim_unused_channel(true);

    dma_channel_config spi_tx_dma_config = dma_channel_get_default_config(dma_spi_tx_chan);
    channel_config_set_transfer_data_size(&spi_tx_dma_config, DMA_SIZE_8);
    channel_config_set_read_increment(&spi_tx_dma_config, true);
    channel_config_set_write_increment(&spi_tx_dma_config, false);
    channel_config_set_dreq(&spi_tx_dma_config, spi_get_dreq(SPI_PORT, true));
    dma_channel_configure(
        dma_spi_tx_chan, &spi_tx_dma_config,
        &spi_get_hw(SPI_PORT)->dr, // 目的地指针
        NULL,                      // 源地址（SPI数据寄存器的地址）
        0,                         // 传输计数
        false                      // 立即启动
    );

    // 配置DMA从SPI接收数据到指定的缓冲区
    dma_channel_config spi_rx_dma_config = dma_channel_get_default_config(dma_spi_rx_chan);
    channel_config_set_transfer_data_size(&spi_rx_dma_config, DMA_SIZE_8);      // 设置数据大小为8位
    channel_config_set_read_increment(&spi_rx_dma_config, false);               // SPI读取地址不增加
    channel_config_set_write_increment(&spi_rx_dma_config, true);               // 内存写入地址递增
    channel_config_set_dreq(&spi_rx_dma_config, spi_get_dreq(SPI_PORT, false)); // 设置DMA请求源为SPI RX
    dma_channel_configure(
        dma_spi_rx_chan, &spi_rx_dma_config,
        NULL,                      // 目的地指针
        &spi_get_hw(SPI_PORT)->dr, // 源地址（SPI数据寄存器的地址）
        0,                         // 传输计数
        false                      // 立即启动
    );

    // 设置DMA传输完成中断回调函数(设置了中断就要在中断处理函数中清除中断，不然只能运行一次)
    dma_channel_set_irq0_enabled(dma_spi_rx_chan, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_irq_handler);
    irq_set_enabled(DMA_IRQ_0, true);

    dma_channel_set_irq0_enabled(dma_spi_tx_chan, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_irq_handler);
    irq_set_enabled(DMA_IRQ_0, true);
}

/**
 * @brief 使用 DMA 通道从 SPI 接收数据，非阻塞式
 *
 * @param rx_data    接收数据存放地址
 * @param len        数据长度
 */
void spi_DMA_read(uint8_t *rx_data, size_t len)
{
    dma_channel_transfer_to_buffer_now(dma_spi_rx_chan, rx_data, len);
}

/**
 * @brief 使用 DMA 通道从 SPI 接收数据，阻塞式
 *
 * @param rx_data    接收数据存放地址
 * @param len        数据长度
 */
void spi_DMA_read_blocking(uint8_t *rx_data, size_t len)
{
    dma_channel_transfer_to_buffer_now(dma_spi_rx_chan, rx_data, len);
    dma_channel_wait_for_finish_blocking(dma_spi_rx_chan);
}

/**
 * @brief 使用 DMA 通道从 SPI 发送数据，非阻塞式
 *
 * @param tx_data    发送的数据
 * @param len        数据长度
 */
void spi_DMA_write(uint8_t *tx_data, size_t len)
{
    dma_channel_transfer_from_buffer_now(dma_spi_tx_chan, tx_data, len);
}

/**
 * @brief 使用 DMA 通道从 SPI 发送数据，阻塞式
 *
 * @param tx_data    发送的数据
 * @param len        数据长度
 */
void spi_DMA_write_blocking(uint8_t *tx_data, size_t len)
{
    dma_channel_transfer_from_buffer_now(dma_spi_tx_chan, tx_data, len);
    dma_channel_wait_for_finish_blocking(dma_spi_tx_chan);
}

void dma_irq_handler() // SPI_DMA 中断处理函数
{
    // 检查DMA通道是否触发了中断
    if (dma_hw->ints0 & (1u << dma_spi_rx_chan))
    {                                                  // RX 完成 DMA 中断处理
        dma_channel_acknowledge_irq0(dma_spi_rx_chan); // 清除DMA中断请求
    }
    else if (dma_hw->ints0 & (1u << dma_spi_tx_chan))
    {                                                  // TX 完成 DMA 中断处理
        dma_channel_acknowledge_irq0(dma_spi_tx_chan); // 清除DMA中断请求
    }
}
/* SPI + DMA */

/* UART + DMA */
/**
 * @brief UART发送DMA通道初始化
 */
void uart_tx_DMA_init(void)
{
    dma_tx_chan = dma_claim_unused_channel(true);
    // 配置DMA通道用于UART发送
    uart_tx_dma_config = dma_channel_get_default_config(dma_tx_chan);
    channel_config_set_transfer_data_size(&uart_tx_dma_config, DMA_SIZE_8);  //8位数据
    channel_config_set_read_increment(&uart_tx_dma_config, true);
    channel_config_set_write_increment(&uart_tx_dma_config, false);
    channel_config_set_dreq(&uart_tx_dma_config, uart_get_dreq(UART_ID, true));
    // 设置DMA传输的源和目的
    dma_channel_configure(
        dma_tx_chan,
        &uart_tx_dma_config,
        &uart_get_hw(UART_ID)->dr, // UART数据寄存器为DMA目的地址
        NULL,                      // 数据缓冲区为DMA源地址
        0,                         // 传输数据的长度
        false                      // 立即启动
    );
}

/**
 * @brief uart DMA 发送数据，非阻塞式
 *
 * @param data 发送数据
 * @param len  发送长度
 */
void uartDma_send(uint8_t *data, size_t len)
{
    dma_channel_transfer_from_buffer_now(dma_tx_chan, data, len);
}

/**
 * @brief uart DMA 发送数据，阻塞式
 *
 * @param data 发送数据
 * @param len  发送长度
 */
void uart_dma_send_blocking(uint8_t *data, size_t len)
{
    dma_channel_transfer_from_buffer_now(dma_tx_chan, data, len);
    dma_channel_wait_for_finish_blocking(dma_tx_chan);
}
/* UART + DMA */

/*PIO 初始化&dma发送*/
void spi_slave_PIO_init(uint pin_mosi, uint pin_sck,uint pin_cs, uint clk_div)
{
    // 加载编译好的PIO程序到指定的PIO实例中
    uint offset = pio_add_program(spi.pio, &spi_slave_program);
    // 获取空闲的状态机
    spi.sm = pio_claim_unused_sm(spi.pio, true);
    // 将GPIO管脚初始化为pio功能
    pio_gpio_init(spi.pio, pin_mosi);
    pio_gpio_init(spi.pio, pin_sck);
    pio_gpio_init(spi.pio, pin_cs);
    pio_sm_set_consecutive_pindirs(spi.pio, spi.sm, pin_mosi, 1, false);
    pio_sm_set_consecutive_pindirs(spi.pio, spi.sm, pin_sck, 1, false);
    pio_sm_set_consecutive_pindirs(spi.pio, spi.sm, pin_cs, 1, false);

    // 根据实际的引脚配置，初始化PIO状态机
    pio_sm_config c = spi_slave_program_get_default_config(offset);

    // 设置附加置位的GPIO管脚
    sm_config_set_in_pins(&c, pin_mosi);

    // 设置合并FIFO
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);

    // 初始化状态机的时钟频率，这里没有设置分频因为需要跟踪主机的时钟
    sm_config_set_clkdiv(&c, clk_div);

    // 设置输入移位寄存器的方向、自动拉取和位数
    sm_config_set_in_shift(&c, false, true, 8); // 8位数据

    // 将配置应用于PIO状态机并启动
    pio_sm_init(spi.pio, spi.sm, offset, &c);
    pio_sm_set_enabled(spi.pio, spi.sm, true);
}

void PioDMAInit(void)
{
    pio_rx_chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(pio_rx_chan);

    channel_config_set_dreq(&c, pio_get_dreq(spi.pio, spi.sm, false));
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);

    dma_channel_configure(pio_rx_chan, &c, NULL, (uint8_t *)&spi.pio->rxf[spi.sm], 0, false);
}

void pio_spi_dma_send_blocking(uint8_t *data, size_t len)
{
    dma_channel_transfer_to_buffer_now(pio_rx_chan, data, len);
    dma_channel_wait_for_finish_blocking(pio_rx_chan);
}