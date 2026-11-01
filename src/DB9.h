// Joysticks DB9 de los puertos joy1 y joy2: Atari (1 boton), Sega 3 botones y Sega 6 botones.
// El tipo de joystick se detecta solo en cada lectura (ver db9Tipo()).
#include "ps2x2pico.h"

// Pines de cada puerto (pin del DB9 entre parentesis)
// Joy1
#define joy1Up 2        // pin 1
#define joy1Down 3      // pin 2
#define joy1Left 4      // pin 3
#define joy1Right 5     // pin 4
#define joy1Fire 6      // pin 6  (Atari: fuego / Sega: B o A)
#define joy1Select 7   // pin 7  (SALIDA: linea SELECT de los mandos Sega)
#define joy1Start 8    // pin 9  (Atari: 2o boton / Sega: C o Start)

// Joy2
#define joy2Up 29
#define joy2Down 28
#define joy2Left 27
#define joy2Right 26
#define joy2Fire 10
#define joy2Select 9
//--------------------------
#ifdef COMPATIBLE_PCB // PCB estandar
#define joy2Start 1     // pin 9
#endif

#ifdef CROMS_PCB // PCB personalizada
#define joy2Start 13    // pin 9
#endif
//---------------------------

//#define DB9_DEBUG   // descomentar para imprimir por UART el tipo y los valores crudos cada vez que cambian los botones

// 1 = mientras un boton esta pulsado se reenvia su codigo en cada lectura (como hacia la version anterior)
// 0 = solo se envia al pulsar y al soltar
#ifndef DB9_REPETIR_PULSADO
#define DB9_REPETIR_PULSADO 1
#endif

void db9Init();      // inicializa los GPIO de los dos puertos
void db9Process();   // lee los dos puertos y envia los codigos a la placa ESP32 (llamar cada ~50 ms)
int db9Tipo(int puerto);   // puerto 1 o 2: devuelve 6 = Sega 6 botones, 3 = Sega 3 botones, 1 = Atari, 0 = nada
