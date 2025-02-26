#include "espectrumCodeSet.h"
#include "ps2x2pico.h"
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
//
void Esp_code_send(u8 pscode, bool is_key_pressed){
    rojo;
    if (is_key_pressed){kb_send(ESP_prefix);kb_send(pscode);}
    else {kb_send(ESP_prefix); kb_send(ESP_prefix_release); kb_send(pscode);}
}