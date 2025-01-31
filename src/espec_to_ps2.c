#include "espectrumCodeSet.h"
#include "ps2x2pico.h"

void Esp_code_send(u8 pscode, bool is_key_pressed){
    if (is_key_pressed){kb_send(ESP_prefix);kb_send(pscode);}
    else {kb_send(ESP_prefix); kb_send(ESP_prefix_release); kb_send(pscode);}
}