//Rutina para controlar led neopixel tarjeta rp2040-zero



#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "neopixel.h"


int sm;
PIO pio=pio1;
#define neopixel 16

void neopixel_init()
{      
  int sm= pio_claim_unused_sm(pio, true); //proporciona una stateMachine libre
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, neopixel, 800000, true);
}


void put_pixel(uint32_t pixel_grb){ pio_sm_put_blocking(pio, sm, pixel_grb << 8u); }

void put_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t mask = (green << 16) | (red << 8) | (blue << 0);
    put_pixel(mask);
}


//neopixel