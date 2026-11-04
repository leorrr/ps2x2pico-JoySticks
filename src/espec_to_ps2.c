#include "espectrumCodeSet.h"
#include "ps2x2pico.h"
#include "neopixel.h"
void kb_send(u8 byte); //declaramos la funcion que se encuentra en ps2kb.c para evitar el warning del compilador
void Esp_code_send(u8 pscode, bool is_key_pressed)
{
    rojo; 
    if (is_key_pressed){kb_send(ESP_prefix);kb_send(pscode);}
    else {kb_send(ESP_prefix); kb_send(ESP_prefix_release); kb_send(pscode);}
}