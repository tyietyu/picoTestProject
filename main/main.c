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
#include "flash.h"

// typedef define freeRTOS
#define USE_FREERTOS 0
#if USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"

#endif
// define for USB
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
const tusb_desc_webusb_url_t desc_url =
    {
        .bLength = 3 + sizeof(URL) - 1,
        .bDescriptorType = 3, // WEBUSB URL type
        .bScheme = 1,         // 0: http, 1: https
        .url = URL};

// time test
typedef struct
{
    uint32_t alarm_id;
    bool time_to_fire;
} my_repeating_timer_t;
volatile uint64_t time_start1 = 0;
volatile uint64_t time_1 = 0;

// led-usb-uart打印函数声明
static volatile bool web_serial_connected = false;
static volatile bool usb_connected = false;
static volatile bool usb_send = false;
void led0_blinking_task(void);
void led1_blinking_task(void);
void web_printf(const char *format, ...);
void usb_printf(const char *format, ...);
void uart_printf(const char *format, ...);

// gererate_test_data
#define APPLY_DATA 256
uint8_t databuf[256]= {0};
uint8_t Flash_Buf[256]={0}; //flash数据
void generate_data(void);
void flash_test();

// PIO SPI
uint8_t rx_buf[256];
uint8_t tx_byte = 0x01;
extern pio_spi_inst_t spi;
extern bool cs_irq_flag;
extern int pio_rx_chan;

// uart_recvice_data
#define ENABLE_UART_RX 1
#define UART_REC_BUFF_SIZE 64
volatile bool rx_flag = false;
typedef enum
{
    AT_IDLE_STATE = 0x00U,
    AT_QUERY_THRESHOLD = 0x01U,
    AT_MODIFY_THRESHOLD = 0x02U,
} ReceiveMode;
ReceiveMode receiveMode = AT_IDLE_STATE;
// void uart_rx_polling(void);

// use freertos
#if USE_FREERTOS
void led_task(void *pvParameters);
void uart_task(void *pvParameters);
#endif

int main()
{
    board_init();
    rp2040_clock_133Mhz();
    radar_gpio_init();
    radar_uart_init();

    tud_init(BOARD_TUD_RHPORT); // init device stack on configured roothub port
    // gpio_set_irq_enabled_with_callback(SPI_CSN_PIN, GPIO_IRQ_EDGE_FALL, true, &cs_irq_handler);
    generate_data();
#if ENABLE_UART_RX
    // irq_set_exclusive_handler(UART0_IRQ, uart_rx_polling);
    // uart_set_irq_enables(UART_ID, true, false);
    // irq_set_enabled(UART0_IRQ, true);
#endif

#if USE_FREERTOS
    gpio_init(LED0_PIN);
    gpio_set_dir(LED0_PIN, GPIO_OUT);
    xTaskCreate(led_task, "LED Task", 128, NULL, 1, NULL);
    xTaskCreate(uart_task, "UART Task", 128, NULL, 2, NULL);

    vTaskStartScheduler();
#endif
flash_test();
    while (1)
    {
        for(int i = 0; i < 256; i++)
        {
             uart_printf("%02x ", Flash_Buf[i]);
        }
        sleep_ms(1000);
    }
}

void flash_test()
{
    flashErase(FLASH_PROGRAM_ADDRESS,FLASH_PAGE_SIZE);
    flashProgram(FLASH_PROGRAM_ADDRESS,databuf, sizeof(databuf));
    getFlashData(FLASH_PROGRAM_ADDRESS, Flash_Buf, sizeof(Flash_Buf));

}


#if USE_FREERTOS
/** led_task */
void led_task(void *pvParameters)
{

    while (1)
    {
        gpio_put(LED0_PIN, 1);
        uart_printf("LED0 blink task started from RTOS!!\r\n");
        vTaskDelay(100);
        gpio_put(LED0_PIN, 0);
        vTaskDelay(500);
    }
}

void uart_task(void *pvParameters)
{
    while (1)
    {
        uart_printf("uartprintf task started from RTOS!!\r\n");
        vTaskDelay(1000);
    }
}
#endif
/*
// uart中断接收函数
void uart_rx_polling(void)
{
    static int i = 0;
    static int count = 0;
    static uint8_t RxData;
    static uint8_t RX_buff[UART_REC_BUFF_SIZE];
    while (uart_is_readable(UART_ID))
    {
        RxData = uart_getc(UART_ID);
        // 根据当前接收模式进行处理
        switch (receiveMode)
        {
        case AT_IDLE_STATE:
            if (i< UART_REC_BUFF_SIZE-1) // 判断最开始接收到的数据是不是AT
            {
                RX_buff[i++] = RxData;
                if (i>=2 && RX_buff[i-2] == '\r' && RxData == '\n') 
                {
                    RX_buff[i-2] = '\0';
                    if (strcmp((const char *)RX_buff, "AT") == 0)
                    {
                        uart_printf("OK\r\n");
                        receiveMode = AT_QUERY_THRESHOLD;
                    }
                    else
                    {
                        uart_printf("ERROR\r\n");
                    }
                     i = 0;
                }
            }
            break;
        case AT_QUERY_THRESHOLD:

            break;
        case AT_MODIFY_THRESHOLD:

        
            break;

        }
    }
}*/
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
