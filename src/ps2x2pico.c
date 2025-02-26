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
#include "button.h"

//neopixel
//defimos unos cuantos colores
#define rojo put_rgb(0xff, 0x00, 0x00)
#define verde put_rgb(0xff, 0xff, 0x00)
#define azul put_rgb(0x00, 0x00, 0xff)
#define violeta put_rgb(0xda, 0x05, 0xf8)
#define negro put_rgb(0x00, 0x00, 0x00)
#define coral put_rgb(0x00,0xff,0xa2)
#define naranja put_rgb(0xfa,0xad,0x08)
#define azulito put_rgb(0x03,0xe8,0xfa)
#define amarillo put_rgb(0xf6,0xfa,0x03)


#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
int sm;
PIO pio;
#define neopixel 16
void put_pixel(uint32_t pixel_grb)
{
    //pio_sm_put_blocking(pio0, sm, pixel_grb << 8u);
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}
void put_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t mask = (green << 16) | (red << 8) | (blue << 0);
    put_pixel(mask);
}
//neopixel

//
#define joy1Up 2 
#define joy1Down 3
#define joy1Left 4 
#define joy1Right 5 
#define joy1Fire 6 
#define joy1Select 7 // Para joyStick SEGA
#define joy1Start 8 // Para joyStick SEGA

//
#define joy2Up 29 
#define joy2Down 28
#define joy2Left 27
#define joy2Right 26 
#define joy2Fire 10  
#define joy2Select 9  // Para joyStick SEGA
#define joy2Start 1  // Para joyStick SEGA






// #define myMillis to_ms_since_boot(get_absolute_time()) //leo esto tampoco se usa para nada
static void print_utf16(uint16_t *temp_buf, size_t buf_len);
void print_device_descriptor(tuh_xfer_t* xfer);


unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0; //leo

u8 kb_addr = 0;
u8 kb_inst = 0;
u8 kb_leds = 0;
char device_str[50];
char manufacturer_str[50];
/*Codigos teclas mas comunes, u8 keycode - from "tinyusb/src/class/hid/hid.h"  HID_KEY_ definition   
0x1e (1),0x1f (2),0x20 (3), 0x21 (4), 0x22 (5), 0x23 (6), 0x24 (7), 0x25 (8), 0x26 (9), 0x27 (0)
0x14 (q), 0x04 (a), 0x12 (o), 0x13 (p), 0x28 (Enter), 0x29 (ESC), 0x2c (SPACE)
0x52 (cursor arriba), 0x51 (cursor abajo), 0x50 (cursor izquierda), 0x4f (cursor derecha), 0x2b (TAB)

*/
// Rutina control jopystick's
void onchange(button_t *button_p) {
  button_t *button = (button_t*)button_p;
  //printf("Button on pin %d changed its state to %d\n", button->pin, button->state);

//de momento los 2 puertos de joystcik actuan igual, queda pendiente reasignar las teclas correspondientes a cada uno

//Cuando soltamos un boton
  if(button->state) {printf ("el state del pin %d es %d ¿Hemos SOLTADO el boton?\n",button->pin,button->state);
  switch(button->pin){

  //Comprobamos el joy1 si hemos soltado un boton 
    case joy1Up:
        kb_send_key(0x52, 0, 0); // 0x52 cursor arriba
        printf("Joy1Up release\n");
    break;
    case joy1Down:
        kb_send_key(0x51, 0, 0); // 0x51 cursor abajo
        printf("Joy1Down release\n");
    break;
    case joy1Left:
        kb_send_key(0x50, 0, 0); //0x50 cursor izquierda
        printf("Joy1Left release\n");
    break;
    case joy1Right:
        kb_send_key(0x4f, 0, 0); //0x4f cursor derecha
        printf("Joy1Right release\n");
    break;
    case joy1Fire: 
        kb_send_key(0x2b, 0, 0); // tabulador
        printf("Joy1Fire release \n");
    break;
//comprobamos el joy2 si hemos soltado un boton
// Mapeamos la norma sinclaer para el joystick 1 que corresponden 
// a las teclas 6,7,8,9,0
   case joy2Up:
        kb_send_key(0x26, 0, 0); //0x26 tecla 9
        printf("Joy2Up release\n");
    break;
    case joy2Down:
        kb_send_key(0x25, 0, 0); //0x25 tecla 8
        printf("Joy2Down release\n");
    break;
    case joy2Left:
        kb_send_key(0x23, 0, 0); // 0x23 tecla 6
        printf("Joy2Left release\n");
    break;
    case joy2Right:
        kb_send_key(0x24, 0, 0); // 0x24 tecla 7
        printf("Joy2Right release\n");
    break;
    case joy2Fire:
        kb_send_key(0x27, 0, 0); //0x27 tecla 0
        printf("Joy2Fire release \n");
    break; 
  }
  
  //kb_send_key(0x52, 0, 0);
   return;}
   
   //cuando pulsamos un boton
   if(!button->state) {printf ("el state del pin %d es %d ¿Hemos pulsado el boton?\n",button->pin,button->state);
   switch(button->pin){

   //comprobamos el joy1 si hemos pulsado un boton 
    case joy1Up:
        kb_send_key(0x52, 1, 0); // 0x52 cursor arriba 
        printf("Joy1Up \n");
    break;
    case joy1Down:
        kb_send_key(0x51, 1, 0); // 0x51 cursor abajo 
        printf("Joy1Down\n");
    break;
    case joy1Left:
        kb_send_key(0x50, 1, 0); // 0x50 cursor izquierda
        printf("Joy1Left\n");
    break;
    case joy1Right:
        kb_send_key(0x4f, 1, 0);// 0x4f cursor derecha
        printf("Joy1Right\n");
    break;
    case joy1Fire:
        kb_send_key(0x2b, 1, 0);// 0x2b tabulador
        printf("Joy1Fire\n");
    break;
//comprobamos el joy2 si hemos pulsado un boton 
// Mapeamos la norma sinclaer para el joystick 1 que corresponden 
// a las teclas 6,7,8,9,0
  case joy2Up:
        kb_send_key(0x26, 1, 0); // //0x26 tecla 9
        printf("Joy2Up \n");
    break;
    case joy2Down:
        kb_send_key(0x25, 1, 0); //0x25 tecla 8
        printf("Joy2Down\n");
    break;
    case joy2Left:
        kb_send_key(0x23, 1, 0);// 0x23 tecla 6
        printf("Joy2Left\n");
    break;
    case joy2Right:
        kb_send_key(0x24, 1, 0);// 0x24 tecla 7
        printf("Joy2Right\n");
    break;
    case joy2Fire:
        kb_send_key(0x27, 1, 0);//0x27 tecla 0
        printf("Joy2Fire\n");
    break;
  }
   
   
   //kb_send_key(0x52, 1, 0);
   return; }
/*
  switch(button->pin){
    case joy1Up:
        printf("Joy1Up\n");
    break;
    case joy2Up:
        printf("Joy2Up\n");
    break;
  }*/
}
//


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
      //board_led_write(1); //desabilitamos el led, porque la rp2040-zero, no tiene led y es tonteria
       //put_rgb(0x00, 0x00, 0xff);
      azul;
    }
  }
}

void tuh_hid_umount_cb(u8 dev_addr, u8 instance) {
  printf("HID(%d,%d) unmounted\n", dev_addr, instance);
  //board_led_write(0); //desabilitamos el led, porque la rp2040-zero, no tiene led y es tonteria
  //put_rgb(0x00, 0x00, 0x00);
  negro;
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

      
       //u8 report2[]={0,0,0x2b,0,0,0,0};
       //u8 report3[]={0,0,0,0,0,0,0,0};
       //uint32_t tiempo1=myMillis;

    gamePad_usb_receive(report); 
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
  
//neopixel
  pio = pio1;
  int sm= pio_claim_unused_sm(pio, true); //proporciona una stateMachine libre
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, neopixel, 800000, true);
//neopixel

  //Joystick 1
  button_t *Joy1Up = create_button(joy1Up, onchange);
  button_t *Joy1Down = create_button(joy1Down, onchange);
  button_t *Joy1Left = create_button(joy1Left, onchange);
  button_t *Joy1Right = create_button(joy1Right, onchange);
  button_t *Joy1Fire = create_button(joy1Fire, onchange);

  //Se usa en el mando sega para comprobrar botones extra, hay que ponerla a 0 o 1 de modo manual y hacer a continuacion la lectura
  //de momento la desactivamos
  //button_t *Joy1Select = create_button(joy1Select, onchange); 

  button_t *Joy1Start = create_button(joy1Start, onchange); // o tambien conocido como fire2

// Joystick 2
  button_t *Joy2Up = create_button(joy2Up, onchange);
  button_t *Joy2Down = create_button(joy2Down, onchange);
  button_t *Joy2Left = create_button(joy2Left, onchange);
  button_t *Joy2Right = create_button(joy2Right, onchange);
  button_t *Joy2Fire = create_button(joy2Fire, onchange);

  //Se usa en el mando sega para comprobrar botones extra, hay que ponerla a 0 o 1 de modo manual y hacer a continuacion la lectura
  //de momento la desactivamos
  //button_t *Joy2Select = create_button(joy2Select, onchange); 
  
  button_t *Joy2Start = create_button(joy2Start, onchange); // o tambien conocido como fire2

  while(1) {
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
