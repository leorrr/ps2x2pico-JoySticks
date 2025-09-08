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
#include "ps2gamepad.h"

#include "DB9.h"
#include "neopixel.h"
#include "xinput_host.h"
#include "ps2x360.h"

//--------------------------------------------------------------------+
// Para generar la fecha de compilacion by CHATGPT
//--------------------------------------------------------------------+

const char *fecha_hora_build() {
    static char out[13];
    char m[4]; int y, d, mo, h, mi, s;

    // Leer fecha y hora
    sscanf(__DATE__, "%s %d %d", m, &d, &y);
    sscanf(__TIME__, "%d:%d:%d", &h, &mi, &s);

    // Convertir mes a número
    switch(m[0]) {
        case 'J': mo = (m[1]=='a')?1: (m[2]=='n')?6:7; break; // Jan, Jun, Jul
        case 'F': mo = 2; break;
        case 'M': mo = (m[2]=='r')?3:5; break;                // Mar, May
        case 'A': mo = (m[1]=='p')?4:8; break;                // Apr, Aug
        case 'S': mo = 9; break;
        case 'O': mo = 10; break;
        case 'N': mo = 11; break;
        case 'D': mo = 12; break;
    }

    // Formato final AAAAMMDDHHMM
    sprintf(out, "%04d%02d%02d%02d%02d", y, mo, d, h, mi);
    return out;
}

#ifdef COMPATIBLE_PCB
#define LVOUT 13
#endif

#define myMillis to_ms_since_boot(get_absolute_time()) //nos da el tiempo en milisegundo desde que hemos arrancado la placa
#define db9_periodo 50 //100 ms serian 10 veces por segundo
#define gamePad_periodo 50
#define xinput_Periodo 50

static void print_utf16(uint16_t *temp_buf, size_t buf_len);
//void print_device_descriptor(tuh_xfer_t* xfer); // como no se usa en ninguna parte  lo desactivamos


//unsigned long currentMillis; //almacena el tiempo actual
unsigned long last_millisDB9=0; // almacena el ultimo tiempo leido para db9
unsigned long last_millisGamePad=0; // almacena el ultimo tiempo leido para gamepad
unsigned long last_millisXinput=0; // almacena el ultimo tiempo leido para xinput mando 360

u8 kb_addr = 0;
u8 kb_inst = 0;
u8 kb_leds = 0;
//Leo
u8 gamepadADDR1= 0;
u8 gamepadINST1=0;
u8 gamepadADDR2= 0;
u8 gamepadINST2=0;
int reportSum=269;

//Esta funciona establiza las lecturas analogicas en los gamepad viejos, que ya no son  tan precisos, como cuando eran nuevos
#define ruido 8000
int16_t removeNoise (int16_t valor) {
  if (valor > -ruido && valor < ruido) {return 0; }// dentro de la zona muerta → 0  
  return valor; // fuera de la zona muerta → mantener valor 
}



int16_t gamepad360_report[7] = {0}; //aqui guardaremos el report del gamepad
u8 gameMouseStick_report[3] = {0}; //aqui guardaremos el report del mouseStick tal vez tenga que ser int8_t pero asi funciona
int mousebr=0;
int mousebl=0;


#define DEADZONESTICK 8000
#define SCALE    32768.0f

void process_stick_as_mouse(int16_t lx, int16_t ly)
{
    int8_t dx = 0;
    int8_t dy = 0;

    if (abs(lx) > DEADZONESTICK) {
        dx = (int8_t)((lx / SCALE) *8); // escala a ±10
    }
    if (abs(ly) > DEADZONESTICK) {
        dy = (int8_t)((-ly / SCALE) * 8); // invertir Y (arriba negativo → arriba en pantalla)
    }

    if (dx || dy) {
       // mouse_move(dx, dy);
       gameMouseStick_report[1]=dx;
       gameMouseStick_report[2]=dy;
       //printf("dx: %d, dy: %d\n", dx, dy);
    }
}


//xinput_host____________________________________

//Since https://github.com/hathach/tinyusb/pull/2222, we can add in custom vendor drivers easily
usbh_class_driver_t const* usbh_app_driver_get_cb(uint8_t* driver_count){
    *driver_count = 1;
    return &usbh_xinput_driver;
}

void tuh_xinput_report_received_cb(uint8_t dev_addr, uint8_t instance, xinputh_interface_t const* xid_itf, uint16_t len)
{
    const xinput_gamepad_t *p = &xid_itf->pad;
    //uint32_t report360[]={p->wButtons,p->bLeftTrigger,p->bRightTrigger,p->sThumbLX,p->sThumbLY,p->sThumbRX,p->sThumbRY};

    const char* type_str;
//unsigned long currentMillisXinput=myMillis;

      
        switch (xid_itf->type)
        {
            case 1: type_str = "Xbox One";          break;
            case 2: type_str = "Xbox 360 Wireless"; break;
            case 3: type_str = "Xbox 360 Wired";    break;
            case 4: type_str = "Xbox OG";           break;
            default: type_str = "Unknown";
        }

        if (xid_itf->connected && xid_itf->new_pad_data)
        {
         
// cargamos el report usb en el array gamepad360_report
        gamepad360_report[0] = p->wButtons;
        gamepad360_report[1] = p->bLeftTrigger;
        gamepad360_report[2] = p->bRightTrigger;
        gamepad360_report[3] = removeNoise(p->sThumbLX);//Stick izquierdo horizontal
        gamepad360_report[4] = removeNoise(p->sThumbLY);//Stick izquierdo vertical
        gamepad360_report[5] = p->sThumbRX;//Stick derecho horizontal
        gamepad360_report[6] = p->sThumbRY;//Stick derecho vertical


        // procesamos los sticks como mouse para el movimiento
        process_stick_as_mouse(gamepad360_report[5], gamepad360_report[6]);
        if (gamepad360_report[1]>10){mousebl=1;}else{mousebl=0;}
        if (gamepad360_report[2]>10){mousebr=2;}else{mousebr=0;}
        //procesamos los botones del mouse
        gameMouseStick_report[0]=(mousebl+mousebr);

        //printf("gameMouseStick_report: %d %d %d\n", gameMouseStick_report[0], gameMouseStick_report[1], gameMouseStick_report[2]);
        ms_usb_receive(gameMouseStick_report); //se lo enviamos al ps2 a traves de esta funcion
        gameMouseStick_report[0]=0; gameMouseStick_report[1]=0;gameMouseStick_report[2]=0; //reseteamos report del mouse a 0


// procesamos el resto de valores del report, solo si ha pasado el periodo establecido
unsigned long currentMillisXinput=myMillis; // Guardamos el tiempo actual

if (currentMillisXinput-last_millisXinput>xinput_Periodo)
{
last_millisXinput=myMillis;
   if (xid_itf->last_xfer_result == XFER_RESULT_SUCCESS){ x360Process1(gamepad360_report);}
    }
  
    // le indicamos al sistema que ya estamos preparados para recibir otro paquete
    //tuh_xinput_receive_report(dev_addr, instance);
}
tuh_xinput_receive_report(dev_addr, instance);
}
void tuh_xinput_mount_cb(uint8_t dev_addr, uint8_t instance, const xinputh_interface_t *xinput_itf)
{
    printf("XINPUT MOUNTED %02x %d\n", dev_addr, instance);
    // If this is a Xbox 360 Wireless controller we need to wait for a connection packet
    // on the in pipe before setting LEDs etc. So just start getting data until a controller is connected.
    if (xinput_itf->type == XBOX360_WIRELESS && xinput_itf->connected == false)
    {
        tuh_xinput_receive_report(dev_addr, instance);
        return;
    }
    tuh_xinput_set_led(dev_addr, instance, 0, true);
    tuh_xinput_set_led(dev_addr, instance, 1, true);
    tuh_xinput_set_rumble(dev_addr, instance, 0, 0, true);
    tuh_xinput_receive_report(dev_addr, instance);
}

void tuh_xinput_umount_cb(uint8_t dev_addr, uint8_t instance)
{
    printf("XINPUT UNMOUNTED %02x %d\n", dev_addr, instance);
}

//ximput_host__________________________________________




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
  printf("decs_Report: %02x, len: %04x ",desc_report,desc_len);
  // This happens if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE.
  // Consider increasing #define CFG_TUH_ENUMERATION_BUFSIZE 256 in tusb_config.h
  if (desc_report == NULL && desc_len == 0) {
    printf("WARNING: HID(%d,%d) skipped!\n",dev_addr, instance);
    return;
  }

#ifdef debugLeo
  printf("HID(%d,%d) mounted\n", dev_addr, instance);
  printf("gamepad_descriptor = [\n");

  for (int   xx=0;xx<desc_len;xx=xx+2) {
    printf ("%02x, %02x, \n",desc_report[xx],desc_report[xx+1]);}
  printf("]\n");
  for (int   xx=0;xx<desc_len;xx++) {
    printf ("0x%02x, ",desc_report[xx]);}
  printf("\n");
#endif

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
      /*
      printf ("addr %04x instance %04x report len %d \n",dev_addr,instance,len);
      for (int xx=0;xx<len;xx++) {
          printf ("report mouse[%d]: %04x \n",xx,report[xx]);
      }*/
      ms_usb_receive(report);

      tuh_hid_receive_report(dev_addr, instance);
    break;

    case HID_ITF_PROTOCOL_NONE: 
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

printf ("Iniciando sistema...Waiting for USB devices...\n");

sleep_ms(2000); // esperamos dos segundos a que se estabilice la alimentacion

#ifdef COMPATIBLE_PCB
char *pcbversion="Compatible_PCB para usar con el proyecto de NoOne y destroyer, y la PCB1 de leorrr";
#endif
#ifdef CROMS_PCB
char *pcbversion="Croms_PCB para usar con la placa PCB2 de leorrr ps2x2pico-JoySticks";
#endif

  printf("\n\n%s - %s %s\n\n", PICO_PROGRAM_NAME, PICO_PROGRAM_VERSION_STRING,fecha_hora_build());
  printf ("PCB: %s\n\n", pcbversion);
  printf("- USB Keyboard to ps2\n- Mouse to ps2\n- 2 joysticks DB9 to PS2 Espectrum Especial Codes.\n- USB GamePaD to ps2 ESPectrum Especial Codes\n- USB GamePadX360 to Ps2 ESPectrum Especial codes control direccional en cruceta y stick analogico izq\n  emulacion de Mouse en Stick Derecho y botones en gatillos analogicos\n\n");
  printf("By leocroms@2025 https://github.com/leorrr/ps2x2pico-JoySticks\n\n");

  //---------------
  #ifdef COMPATIBLE_PCB
  gpio_init(LVOUT); // LVOUT  servia para dar los 3 voltios al levelshifter, seguramente para facilitar el montaje original
  //gpio_init(LVIN);
  gpio_set_dir(LVOUT, GPIO_OUT);
  //gpio_set_dir(LVIN, GPIO_OUT);
  gpio_put(LVOUT, 1); //ahora se los damos desde el  pin de 3v del rp2040
  //gpio_put(LVIN, 1);
  #endif
 //---------------------


  tusb_init();
  kb_init(KBOUT, KBIN);
  ms_init(MSOUT, MSIN);
  db9Init(); // inicializamos los joysticks db9, norma atari

  neopixel_init(); //inicializamos neopixel

  while(1) {

    // procesamos el report de los db9 cada 100ms, que serian unas 10 veces por segungo
    // para no sobrecargar el sistema
    unsigned long currentMillisDB9=myMillis;
    if (currentMillisDB9-last_millisDB9>db9_periodo){last_millisDB9=myMillis;db9Process();}
   
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
