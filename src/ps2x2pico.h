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

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

//----------------
// ***********************************************************************************************************************
// Especificamos que version es la que estamos compilando, la 1 que seria la normal                                      *
// compatible con el proyecto de noOne https://github.com/No0ne/ps2x2pico y el de                                        *
// destroyer https://github.com/destroyer-dcf/usb2ps2                                                                    *
// o la version 2 que es para usar con mi pcb personalizada.                                                             *
// La diferencia entre ambas pcb reside en la version 1, obtine los 3 voltios para usar el level shifter de GPIO 13,     *
// mientras que en la version 2 obtenemos esos 3 voltios directamente del pin de 3 voltios de la Pico                    *
// asi de este modo podemos usar el GPIO 13 como pin 9 del Joy 2 y liberamos el GPIO 0 que es usado como RX en la uart   *
//                                                                                                                       *
//********************************************************************************************************************** *

//#define debugLeo  // Descomentar para activar mensajes de debug por el puerto serie
                        // Comentar para desactivar los mensajes de debug

//Descomenntar lo que proceda y comomentar lo que no, una cosa o la otra

#define COMPATIBLE_PCB //  pcb compatible, 3v from GPIO 13, pin 9 joy2 GPIO 1

//#define CROMS_PCB // pcb personalizada, 3v from pin 3v Pico, pin 9 joy2 GPIO 13

//------------------

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void tuh_kb_set_leds(u8 leds);
void kb_usb_receive(u8 const* report, u16 len);
bool kb_task();
void kb_init(u8 gpio_out, u8 gpio_in);
void kb_set_defaults();

void ms_init(u8 gpio_out, u8 gpio_in);
void ms_usb_receive(u8 const* report);
bool ms_task();
