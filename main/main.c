#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "board.h"
#include "tusb.h"
#include "pico/time.h"
#include "bsp/board.h"
#include "rp2040_clock.h"
#include "usb_descriptors.h"
#include "pico/multicore.h"
#include "pio_spi.h"

#define APP_TX_DATA_SIZE 2048
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];
/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum
{
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,

    BLINK_ALWAYS_OFF = UINT32_MAX,
    BLINK_ALWAYS_ON = 0
};

static uint32_t led0_blink_interval_ms = BLINK_NOT_MOUNTED;
static uint32_t led1_blink_interval_ms = BLINK_NOT_MOUNTED;
#define URL "candas1.github.io/Hoverboard-Web-Serial-Control"

/******************************************************/
#define APPLY_DATA 256
uint8_t databuf[256];
/**************************************************************/

const tusb_desc_webusb_url_t desc_url =
    {
        .bLength = 3 + sizeof(URL) - 1,
        .bDescriptorType = 3, // WEBUSB URL type
        .bScheme = 1,         // 0: http, 1: https
        .url = URL};

typedef struct
{
    uint32_t alarm_id;
    bool time_to_fire;
} my_repeating_timer_t;

static volatile bool web_serial_connected = false;
static volatile bool usb_connected = false;
static volatile bool usb_send = false;

volatile uint64_t time_start1 = 0;
volatile uint64_t time_1 = 0;

void led0_blinking_task(void);
void led1_blinking_task(void);
void web_printf(const char *format, ...);
void usb_printf(const char *format, ...);
void uart_printf(const char *format, ...);
void generate_data(void);

/************************************************/
uint8_t rx_buf[256];
uint8_t tx_byte = 0x01;
extern pio_spi_inst_t spi;
extern bool cs_irq_flag;
extern int pio_rx_chan;
/*************************************************/
int main()
{
    board_init();
    rp2040_clock_133Mhz();
    radar_gpio_init();
    radar_uart_init();
    // radar_spi_init();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);
    generate_data();
    spi_slave_PIO_init(SPI_RX_PIN, SPI_SCK_PIN, 1.0f);
    PioDMAInit();
    uart_tx_DMA_init();

    while (1)
    {
        tud_task(); // tinyusb device task
        led0_blinking_task();
        // while (!cs_irq_flag)
        //     ;
        // cs_irq_flag = false;
        // pio_spi_dma_send_blocking(rx_buf, sizeof(rx_buf));
        pio_spi_read8_blocking(&spi, rx_buf, sizeof(rx_buf));
        for (int i = 0; i < APPLY_DATA; i++) // LEN
        {
            uart_printf("%x ", rx_buf[i]);
        }
        uart_printf("\r\n");
    }
}

// 写一个函数生成00到FF存储在databuf中,使用静态数组存储
void generate_data(void)
{
    for (int i = 0; i < APPLY_DATA; i++)
    {
        databuf[i] = i;
    }
}

void led0_blinking_task(void)
{
    static uint32_t led0_start_ms = 0;
    static bool led0_state = false;

    // Blink every interval ms
    if (board_millis() - led0_start_ms < led0_blink_interval_ms)
        return; // not enough time
    led0_start_ms += led0_blink_interval_ms;

    gpio_put(LED0_PIN, led0_state);
    led0_state = 1 - led0_state; // toggle
}

void led1_blinking_task(void)
{
    static uint32_t led1_start_ms = 0;
    static bool led1_state = false;

    // Blink every interval ms
    if (board_millis() - led1_start_ms < led1_blink_interval_ms)
        return; // not enough time
    led1_start_ms += led1_blink_interval_ms;

    gpio_put(LED1_PIN, led1_state);
    led1_state = 1 - led1_state; // toggle
}

void web_printf(const char *format, ...)
{
    if (web_serial_connected)
    {
        va_list args;
        uint32_t length;

        va_start(args, format);
        length = vsnprintf((char *)UserTxBufferFS, APP_TX_DATA_SIZE, (char *)format, args);
        va_end(args);

        tud_vendor_write(UserTxBufferFS, length);
        tud_vendor_flush();
    }
}

void usb_printf(const char *format, ...)
{
    va_list args;
    uint32_t length;

    va_start(args, format);
    length = vsnprintf((char *)UserTxBufferFS, APP_TX_DATA_SIZE, (char *)format, args);
    va_end(args);
    tud_cdc_write(UserTxBufferFS, length);
}

void uart_printf(const char *format, ...)
{
    va_list args;
    uint32_t length;

    va_start(args, format);
    length = vsnprintf((char *)UserTxBufferFS, APP_TX_DATA_SIZE, (char *)format, args);
    va_end(args);
    uart_write_blocking(UART_ID, UserTxBufferFS, length);
}

// Invoked when a control transfer occurred on an interface of this class
// Driver response accordingly to the request and the transfer stage (setup/data/ack)
// return false to stall control endpoint (e.g unsupported request)
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request)
{
    // nothing to with DATA & ACK stage
    if (stage != CONTROL_STAGE_SETUP)
        return true;

    switch (request->bmRequestType_bit.type)
    {
    case TUSB_REQ_TYPE_VENDOR:
        switch (request->bRequest)
        {
        case VENDOR_REQUEST_WEBUSB:
            // match vendor request in BOS descriptor
            // Get landing page url
            return tud_control_xfer(rhport, request, (void *)(uintptr_t)&desc_url, desc_url.bLength);

        case VENDOR_REQUEST_MICROSOFT:
            if (request->wIndex == 7)
            {
                // Get Microsoft OS 2.0 compatible descriptor
                uint16_t total_len;
                memcpy(&total_len, desc_ms_os_20 + 8, 2);

                return tud_control_xfer(rhport, request, (void *)(uintptr_t)desc_ms_os_20, total_len);
            }
            else
            {
                return false;
            }

        default:
            break;
        }
        break;

    case TUSB_REQ_TYPE_CLASS:
        if (request->bRequest == 0x22)
        {
            // Webserial simulate the CDC_REQUEST_SET_CONTROL_LINE_STATE (0x22) to connect and disconnect.
            web_serial_connected = (request->wValue != 0);

            // Always lit LED if connected
            if (web_serial_connected)
            {
                // board_led_write(true);
                led0_blink_interval_ms = BLINK_ALWAYS_ON;

                tud_vendor_write_str("\r\nWebUSB interface connected\r\n");
                tud_vendor_flush();
            }
            else
            {
                led0_blink_interval_ms = BLINK_MOUNTED;
            }

            // response with status OK
            return tud_control_status(rhport, request);
        }
        break;

    default:
        break;
    }

    // stall unknown request
    return false;
}

// Invoked when device is mounted
void tud_mount_cb(void)
{
    led1_blink_interval_ms = BLINK_MOUNTED;
    usb_connected = true;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    led1_blink_interval_ms = BLINK_NOT_MOUNTED;
    usb_connected = false;
}

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
    (void)itf;

    // connected
    if (dtr && rts)
    {
        led1_blink_interval_ms = BLINK_SUSPENDED;
        // print initial message when connected
        // tud_cdc_write_str("\r\nTinyUSB WebUSB device example\r\n");
    }
}
