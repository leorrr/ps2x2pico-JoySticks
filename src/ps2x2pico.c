/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 No0ne (https://github.com/No0ne)
 *           (c) 2023 Dustin Hoffman
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hardware/watchdog.h"
#include "hardware/gpio.h"
#include "bsp/board_api.h"
#include "tusb.h"
#include "ps2x2pico.h"
#include "ps2gamepad.c"
#include "DB9.c"
#include "neopixel.c"


#define myMillis to_ms_since_boot(get_absolute_time()) //nos da el tiempo en milisegundo desde que hemos arrancado la placa
#define db9_periodo 100 //100 ms serian 10 veces por segundo
#define gamePad_periodo 100
static void print_utf16(uint16_t *temp_buf, size_t buf_len);
void print_device_descriptor(tuh_xfer_t* xfer);


//unsigned long currentMillis; //almacena el tiempo actual
unsigned long last_millisDB9=0; // almacena el ultimo tiempo leido para db9
unsigned long last_millisGamePad=0; // almacena el ultimo tiempo leido para gamepad

u8 kb_addr = 0;
u8 kb_inst = 0;
u8 kb_leds = 0;
//Leo
u8 gamepadADDR1= 0;
u8 gamepadINST1=0;
u8 gamepadADDR2= 0;
u8 gamepadINST2=0;
int reportSum=269;
//leo

char device_str[50];
char manufacturer_str[50];


void tuh_kb_set_leds(u8 leds) {
  if(kb_addr) {
    kb_leds = leds;
    printf("HID(%d,%d): LEDs = %d\n", kb_addr, kb_inst, kb_leds);
    tuh_hid_set_report(kb_addr, kb_inst, 0, HID_REPORT_TYPE_OUTPUT, &kb_leds, sizeof(kb_leds));
  }
}

#define LANGUAGE_ID 0x0409 // English

void tuh_hid_mount_cb(u8 dev_addr, u8 instance, u8 const* desc_report, u16 desc_len) {
  // This happens if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE.
  // Consider increasing #define CFG_TUH_ENUMERATION_BUFSIZE 256 in tusb_config.h
  if (desc_report == NULL && desc_len == 0) {
    printf("WARNING: HID(%d,%d) skipped!\n",dev_addr, instance);
    return;
  }

  hid_interface_protocol_enum_t hid_if_proto = tuh_hid_interface_protocol(dev_addr, instance);
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  char* hidprotostr;
  switch (hid_if_proto) {
    case HID_ITF_PROTOCOL_NONE:
      hidprotostr = "NONE";
      printf ("case none \n");
      break;
    case HID_ITF_PROTOCOL_KEYBOARD:
      hidprotostr = "KEYBOARD";
      printf ("case keyboard \n");
      break;
    case HID_ITF_PROTOCOL_MOUSE:
      hidprotostr = "MOUSE";
      printf ("case Mouse \n");
      //tuh_hid_set_protocol(dev_addr, instance, HID_PROTOCOL_REPORT);
      break;
    default:
      hidprotostr = "UNKNOWN";
      printf ("case desconocido \n");
      break;
  };

  printf("HID(%d,%d,%s) mounted\n", dev_addr, instance, hidprotostr);
  printf(" ID: %04x:%04x\n", vid, pid);
 
  uint16_t temp_buf[128];

  printf(" Manufacturer: ");
  if (XFER_RESULT_SUCCESS == tuh_descriptor_get_manufacturer_string_sync(dev_addr, LANGUAGE_ID, temp_buf, sizeof(temp_buf)) )
  {
    print_utf16(temp_buf, TU_ARRAY_SIZE(temp_buf));
  }
  printf("\n");

  printf(" Product:      ");
  if (XFER_RESULT_SUCCESS == tuh_descriptor_get_product_string_sync(dev_addr, LANGUAGE_ID, temp_buf, sizeof(temp_buf)))
  {
    print_utf16(temp_buf, TU_ARRAY_SIZE(temp_buf));
  }
  printf("\n\n");

  if (hid_if_proto == HID_ITF_PROTOCOL_KEYBOARD || hid_if_proto == HID_ITF_PROTOCOL_MOUSE || hid_if_proto == HID_ITF_PROTOCOL_NONE ) {
    if (!tuh_hid_receive_report(dev_addr, instance)) {
      printf("ERROR: Could not register for HID(%d,%d,%s)!\n", dev_addr, instance, hidprotostr);
    } else {
      printf("HID(%d,%d,%s) registered for reports\n", dev_addr, instance, hidprotostr);
      if (hid_if_proto == HID_ITF_PROTOCOL_KEYBOARD) {
          // TODO: This needs to be addressed if we want to have multiple connected kbds working correctly! 
          // Only relevant for KB LEDS though.
          // Could be a list of all connected kbds, so we could set the LEDs on each.
          kb_addr = dev_addr;
          kb_inst = instance;
      }
      //leo
      if (hid_if_proto == HID_ITF_PROTOCOL_NONE) {
        // Guardamos la direcciones de los gamepad a medida que los conectamos
        // solo se guardan 2 gamepads el resto los ignoramos el spectrum solo tenioa 2 joysticks
        if (gamepadADDR1==0 && gamepadINST1==0){gamepadADDR1 = dev_addr; gamepadINST1= instance;}
        else if (gamepadADDR2==0 && gamepadINST2==0){gamepadADDR2= dev_addr; gamepadINST2= instance;}
    }
    //leo
      azul; //encendemos el led cuando conectamos un dispositivo usb
    }
  }
}

void tuh_hid_umount_cb(u8 dev_addr, u8 instance) {
  printf("HID(%d,%d) unmounted\n", dev_addr, instance);
  negro;//apagamos  el led cuando desconectamos un dispositivo usb
  if(dev_addr == kb_addr && instance == kb_inst) {
    kb_addr = 0;
    kb_inst = 0;
  }
  //tuh_deinit(TUH_OPT_RHPORT);
  //printf("deinit(%d)\n", TUH_OPT_RHPORT);
  //tusb_init();
  //printf("init()\n");
}

void tuh_hid_report_received_cb(u8 dev_addr, u8 instance, u8 const* report, u16 len) {

  switch(tuh_hid_interface_protocol(dev_addr, instance)) {
    case HID_ITF_PROTOCOL_KEYBOARD:
      #ifdef TRACE
      printf("HID_KB(%d,%d): r[2..7]={0x%x,0x%x,0x%x,0x%x,0x%x,0x%x},r[0]=0x%x,l=%d\n",
       dev_addr, instance, 
       report[2], report[3], report[4], report[5], report[6], report[7], 
       report[0], len);
      #else
      #ifdef KB_DEBUG
      printf("HID_KB(%d,%d): r[2]=0x%x,r[0]=0x%x,l=%d\n", dev_addr, instance, report[2], report[0], len);
      #endif
      #endif
      //printf("kb_usb_receive report: %02x  \r",report[2]);
      kb_usb_receive(report, len);
      tuh_hid_receive_report(dev_addr, instance);
    break;
    
    case HID_ITF_PROTOCOL_MOUSE:
      #ifdef TRACE
      printf("HID_MS(%d,%d): r[2..7]={0x%x,0x%x,0x%x,0x%x,0x%x,0x%x},r[0]=0x%x,l=%d\n",
       dev_addr, instance, 
       report[2], report[3], report[4], report[5], report[6], report[7], 
       report[0], len);
      #else
      #ifdef MS_DEBUG
      printf("HID_MS(%d,%d)\n", dev_addr, instance);
      #endif
      #endif

      ms_usb_receive(report);
      tuh_hid_receive_report(dev_addr, instance);
    break;

    case HID_ITF_PROTOCOL_NONE: 

    
    //Todo esto son pruebas y de momento funciona a medias y mal
     #ifdef TRACE
      printf("HID_MS(%d,%d): r[2..7]={0x%x,0x%x,0x%x,0x%x,0x%x,0x%x},r[0]=0x%x,l=%d\n",
       dev_addr, instance, 
       report[2], report[3], report[4], report[5], report[6], report[7], 
       report[0], len);
      #else
      #ifdef MS_DEBUG
      printf("HID_MS(%d,%d)\n", dev_addr, instance);
      #endif
      #endif
//leo
/*
    if(dev_addr==gamepadADDR1 && instance==gamepadINST1) {gamePad1_usb_receive(report);}
    if(dev_addr==gamepadADDR2 && instance==gamepadINST2) {gamePad2_usb_receive(report);}*/
    
/*
//int reportSum = report[3]+report[4]+ report[5]+ report[6];
//printf ("reportSum= %d",reportSum);
if(dev_addr==gamepadADDR1 && instance==gamepadINST1 ) {gamePad1_usb_receive(report);}//reportSum=269;}
//if(dev_addr==gamepadADDR1 && instance==gamepadINST1 && (report[3]!=127 && report[4]!=127 && report[5]!=15 && report[6]!=0)) {printf("report3: %d, report4; %d,report5: %d,report6:%d",report[3],report[4],report[5], report[6]);gamePad1_usb_receive(report);}
if(dev_addr==gamepadADDR2 && instance==gamepadINST2 ) {gamePad2_usb_receive(report);}//reportSum=269;}
*/
//leo
unsigned long currentMillisGMPD=myMillis;
if (currentMillisGMPD-last_millisGamePad>gamePad_periodo)
{
last_millisGamePad=myMillis;
if(dev_addr==gamepadADDR1 && instance==gamepadINST1 ) {Gamepad1Process(report);tuh_hid_receive_report(dev_addr, instance);break;}
if(dev_addr==gamepadADDR2 && instance==gamepadINST2 ) {Gamepad2Process(report);tuh_hid_receive_report(dev_addr, instance);break;}
}
/*
  // procesamos el report cada 100ms, unas 10 veces por segundo, para no sobrecargar el sistema
  //porque los reports del gamepad aunque no este pulsado ningun boton siempre se envian
       unsigned long currentMillisGMPD=myMillis;
       if (currentMillisGMPD-last_millisGamePad>gamePad_periodo){last_millisGamePad=myMillis;gamePad_usb_receive(report);}
 */      
    tuh_hid_receive_report(dev_addr, instance);
    
    break;

  }
}

void main() {
  board_init();
  printf("\n%s-%s\n", PICO_PROGRAM_NAME, PICO_PROGRAM_VERSION_STRING);
  
  gpio_init(LVOUT);
  //gpio_init(LVIN);
  gpio_set_dir(LVOUT, GPIO_OUT);
  //gpio_set_dir(LVIN, GPIO_OUT);
  gpio_put(LVOUT, 1);
  //gpio_put(LVIN, 1);
  


  tusb_init();
  kb_init(KBOUT, KBIN);
  ms_init(MSOUT, MSIN);
  db9Init(); // inicializamos los joysticks db9, norma atari

  neopixel_init(); //inicializamos neopixel

  while(1) {

    // procesamos el report de los db9 cada 100ms, que serian unas 10 veces por segungo
    // para no sobrecargar el sistema
    unsigned long currentMillisDB9=myMillis;
    if (currentMillisDB9-last_millisDB9>db9_periodo){last_millisDB9=myMillis;db9Report();}
   
    tuh_task();
    kb_task();
    ms_task();
  }
}

void reset() {
  printf("\n\n *** PANIC via tinyusb: watchdog reset!\n\n");
  watchdog_enable(100, false);
}

//--------------------------------------------------------------------+
// String Descriptor Helper
//--------------------------------------------------------------------+

static void _convert_utf16le_to_utf8(const uint16_t *utf16, size_t utf16_len, uint8_t *utf8, size_t utf8_len) {
    // TODO: Check for runover.
    (void)utf8_len;
    // Get the UTF-16 length out of the data itself.

    for (size_t i = 0; i < utf16_len; i++) {
        uint16_t chr = utf16[i];
        if (chr < 0x80) {
            *utf8++ = chr & 0xffu;
        } else if (chr < 0x800) {
            *utf8++ = (uint8_t)(0xC0 | (chr >> 6 & 0x1F));
            *utf8++ = (uint8_t)(0x80 | (chr >> 0 & 0x3F));
        } else {
            // TODO: Verify surrogate.
            *utf8++ = (uint8_t)(0xE0 | (chr >> 12 & 0x0F));
            *utf8++ = (uint8_t)(0x80 | (chr >> 6 & 0x3F));
            *utf8++ = (uint8_t)(0x80 | (chr >> 0 & 0x3F));
        }
        // TODO: Handle UTF-16 code points that take two entries.
    }
}

// Count how many bytes a utf-16-le encoded string will take in utf-8.
static int _count_utf8_bytes(const uint16_t *buf, size_t len) {
    size_t total_bytes = 0;
    for (size_t i = 0; i < len; i++) {
        uint16_t chr = buf[i];
        if (chr < 0x80) {
            total_bytes += 1;
        } else if (chr < 0x800) {
            total_bytes += 2;
        } else {
            total_bytes += 3;
        }
        // TODO: Handle UTF-16 code points that take two entries.
    }
    return (int) total_bytes;
}
static void print_utf16(uint16_t *temp_buf, size_t buf_len) {
    if ((temp_buf[0] & 0xff) == 0) return;  // empty
    size_t utf16_len = ((temp_buf[0] & 0xff) - 2) / sizeof(uint16_t);
    size_t utf8_len = (size_t) _count_utf8_bytes(temp_buf + 1, utf16_len);
    _convert_utf16le_to_utf8(temp_buf + 1, utf16_len, (uint8_t *) temp_buf, sizeof(uint16_t) * buf_len);
    ((uint8_t*) temp_buf)[utf8_len] = '\0';

    printf("%s", (char*)temp_buf);
}
