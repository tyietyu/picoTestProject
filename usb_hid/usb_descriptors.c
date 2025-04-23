/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

 #include "bsp/board.h"
 #include "tusb.h"
 #include "usb_descriptors.h"
 
 /* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
  * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
  *
  * Auto ProductID layout's Bitmap:
  *   [MSB]         HID | MSC | CDC          [LSB]
  */
 #define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
 #define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                            _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )
 
 #define USB_VID   0xCafe
 #define USB_BCD   0x0200

 #define MS_OS_20_DESC_LEN  0xB2
 uint8_t const desc_ms_os_20[] =
{
  // Set header: length, type, windows version, total length
  U16_TO_U8S_LE(0x000A), U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR), U32_TO_U8S_LE(0x06030000), U16_TO_U8S_LE(MS_OS_20_DESC_LEN),

  // Configuration subset header: length, type, configuration index, reserved, configuration total length
  U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION), 0, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN-0x0A),

  // Function Subset header: length, type, first interface, reserved, subset length
  U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION), ITF_NUM_VENDOR, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN-0x0A-0x08),

  // MS OS 2.0 Compatible ID descriptor: length, type, compatible ID, sub compatible ID
  U16_TO_U8S_LE(0x0014), U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID), 'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // sub-compatible

  // MS OS 2.0 Registry property descriptor: length, type
  U16_TO_U8S_LE(MS_OS_20_DESC_LEN-0x0A-0x08-0x08-0x14), U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
  U16_TO_U8S_LE(0x0007), U16_TO_U8S_LE(0x002A), // wPropertyDataType, wPropertyNameLength and PropertyName "DeviceInterfaceGUIDs\0" in UTF-16
  'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00,
  'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00, 0x00,
  U16_TO_U8S_LE(0x0050), // wPropertyDataLength
	//bPropertyData: “{975F44D9-0D08-43FD-8B3E-127CA8AFFF9D}”.
  '{', 0x00, '9', 0x00, '7', 0x00, '5', 0x00, 'F', 0x00, '4', 0x00, '4', 0x00, 'D', 0x00, '9', 0x00, '-', 0x00,
  '0', 0x00, 'D', 0x00, '0', 0x00, '8', 0x00, '-', 0x00, '4', 0x00, '3', 0x00, 'F', 0x00, 'D', 0x00, '-', 0x00,
  '8', 0x00, 'B', 0x00, '3', 0x00, 'E', 0x00, '-', 0x00, '1', 0x00, '2', 0x00, '7', 0x00, 'C', 0x00, 'A', 0x00,
  '8', 0x00, 'A', 0x00, 'F', 0x00, 'F', 0x00, 'F', 0x00, '9', 0x00, 'D', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00
};
 //--------------------------------------------------------------------+
 // Device Descriptors
 //--------------------------------------------------------------------+
 tusb_desc_device_t const desc_device =
 {
     .bLength            = sizeof(tusb_desc_device_t),
     .bDescriptorType    = TUSB_DESC_DEVICE,
     .bcdUSB             = USB_BCD,
     .bDeviceClass       = 0x00,
     .bDeviceSubClass    = 0x00,
     .bDeviceProtocol    = 0x00,
     .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
 
     .idVendor           = USB_VID,
     .idProduct          = USB_PID,
     .bcdDevice          = 0x0100,
 
     .iManufacturer      = 0x01,
     .iProduct           = 0x02,
     .iSerialNumber      = 0x03,
 
     .bNumConfigurations = 0x01
 };
 
 // Invoked when received GET DEVICE DESCRIPTOR
 // Application return pointer to descriptor
 uint8_t const * tud_descriptor_device_cb(void)
 {
   return (uint8_t const *) &desc_device;
 }
 
 //--------------------------------------------------------------------+
 // HID Report Descriptor
 //--------------------------------------------------------------------+
 
 uint8_t const desc_hid_report[] =
 {
   TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD         )),
   TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(REPORT_ID_MOUSE            )),
   TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL )),
   TUD_HID_REPORT_DESC_GAMEPAD ( HID_REPORT_ID(REPORT_ID_GAMEPAD          ))
 };
 
 // Invoked when received GET HID REPORT DESCRIPTOR
 // Application return pointer to descriptor
 // Descriptor contents must exist long enough for transfer to complete
 uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
 {
   (void) instance;
   return desc_hid_report;
 }
 
 //--------------------------------------------------------------------+
 // Configuration Descriptor
 //--------------------------------------------------------------------+
 
 enum
 {
   ITF_NUM_HID,
   ITF_NUM_TOTAL
 };
 
 #define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
 
 #define EPNUM_HID   0x81
 
 uint8_t const desc_configuration[] =
 {
   // Config number, interface count, string index, total length, attribute, power in mA
   TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
 
   // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
   TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)
 };
 
 #if TUD_OPT_HIGH_SPEED
 // Per USB specs: high speed capable device must report device_qualifier and other_speed_configuration
 
 // other speed configuration
 uint8_t desc_other_speed_config[CONFIG_TOTAL_LEN];
 
 // device qualifier is mostly similar to device descriptor since we don't change configuration based on speed
 tusb_desc_device_qualifier_t const desc_device_qualifier =
 {
   .bLength            = sizeof(tusb_desc_device_qualifier_t),
   .bDescriptorType    = TUSB_DESC_DEVICE_QUALIFIER,
   .bcdUSB             = USB_BCD,
 
   .bDeviceClass       = 0x00,
   .bDeviceSubClass    = 0x00,
   .bDeviceProtocol    = 0x00,
 
   .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
   .bNumConfigurations = 0x01,
   .bReserved          = 0x00
 };
 
 // Invoked when received GET DEVICE QUALIFIER DESCRIPTOR request
 // Application return pointer to descriptor, whose contents must exist long enough for transfer to complete.
 // device_qualifier descriptor describes information about a high-speed capable device that would
 // change if the device were operating at the other speed. If not highspeed capable stall this request.
 uint8_t const* tud_descriptor_device_qualifier_cb(void)
 {
   return (uint8_t const*) &desc_device_qualifier;
 }
 
 // Invoked when received GET OTHER SEED CONFIGURATION DESCRIPTOR request
 // Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
 // Configuration descriptor in the other speed e.g if high speed then this is for full speed and vice versa
 uint8_t const* tud_descriptor_other_speed_configuration_cb(uint8_t index)
 {
   (void) index; // for multiple configurations
 
   // other speed config is basically configuration with type = OHER_SPEED_CONFIG
   memcpy(desc_other_speed_config, desc_configuration, CONFIG_TOTAL_LEN);
   desc_other_speed_config[1] = TUSB_DESC_OTHER_SPEED_CONFIG;
 
   // this example use the same configuration for both high and full speed mode
   return desc_other_speed_config;
 }
 
 #endif // highspeed
 
 // Invoked when received GET CONFIGURATION DESCRIPTOR
 // Application return pointer to descriptor
 // Descriptor contents must exist long enough for transfer to complete
 uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
 {
   (void) index; // for multiple configurations
 
   // This example use the same configuration for both high and full speed mode
   return desc_configuration;
 }
 
 //--------------------------------------------------------------------+
 // String Descriptors
 //--------------------------------------------------------------------+
 
 // String Descriptor Index
 enum {
   STRID_LANGID = 0,
   STRID_MANUFACTURER,
   STRID_PRODUCT,
   STRID_SERIAL,
 };
 
 // array of pointer to string descriptors
 char const *string_desc_arr[] =
 {
   (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
   "TinyUSB",                     // 1: Manufacturer
   "TinyUSB Device",              // 2: Product
   NULL,                          // 3: Serials will use unique ID if possible
 };
 
 static uint16_t _desc_str[32 + 1];
 
 // Invoked when received GET STRING DESCRIPTOR request
 // Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
 uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
   (void) langid;
   size_t chr_count;
 
   switch ( index ) {
     case STRID_LANGID:
       memcpy(&_desc_str[1], string_desc_arr[0], 2);
       chr_count = 1;
       break;
 
     case STRID_SERIAL:
      //  chr_count = board_usb_get_serial(_desc_str + 1, 32);
       break;
 
     default:
       // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
       // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
 
       if ( !(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) ) return NULL;
 
       const char *str = string_desc_arr[index];
 
       // Cap at max char
       chr_count = strlen(str);
       size_t const max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) - 1; // -1 for string type
       if ( chr_count > max_count ) chr_count = max_count;
 
       // Convert ASCII string into UTF-16
       for ( size_t i = 0; i < chr_count; i++ ) {
         _desc_str[1 + i] = str[i];
       }
       break;
   }
 
   // first byte is length (including header), second byte is string type
   _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));
 
   return _desc_str;
 }
 