// Lectura de los joysticks DB9 (Atari, Sega 3 botones y Sega 6 botones) y envio de los codigos ESPectrum.
//
// Como se distingue cada tipo (la Pico controla la linea SELECT, pin 7, y lee el resto):
//   Sega:  con SELECT=0 las lineas Izq. y Der. bajan a 0 solas (su "firma"). El de 6 botones
//          ademas pone a 0 las cuatro direcciones en el 3er pulso de SELECT.
//   Atari: son solo interruptores, no responden a SELECT.
//   Un Atari en reposo es igual a "nada conectado": solo se reconoce al pulsar algo.
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "espectrumCodeSet.h"
#include "DB9.h"

// Pines de cada puerto: ARRIBA, ABAJO, IZQ., DER., pin 6, SELECT (pin 7), pin 9
static const uint8_t PIN[2][7] = {
    { joy1Up, joy1Down, joy1Left, joy1Right, joy1Fire, joy1Select, joy1Start },
    { joy2Up, joy2Down, joy2Left, joy2Right, joy2Fire, joy2Select, joy2Start }
};

// Codigo ESPectrum de cada boton, en el orden de los bits del mapa de botones:
// ARRIBA ABAJO IZQ. DER. B C A START Z Y X MODE
static const uint8_t ESP[2][12] = {
    { ESP_JOY1UP, ESP_JOY1DOWN, ESP_JOY1LEFT, ESP_JOY1RIGHT, ESP_JOY1B, ESP_JOY1C,
      ESP_JOY1A, ESP_JOY1START, ESP_JOY1Z, ESP_JOY1Y, ESP_JOY1X, ESP_JOY1MODE },
    { ESP_JOY2UP, ESP_JOY2DOWN, ESP_JOY2LEFT, ESP_JOY2RIGHT, ESP_JOY2B, ESP_JOY2C,
      ESP_JOY2A, ESP_JOY2START, ESP_JOY2Z, ESP_JOY2Y, ESP_JOY2X, ESP_JOY2MODE }
};

static int r[2][8];        // ultima lectura de cada puerto (SELECT alternando 1,0,1,0,...)
static int tipo[2];        // tipo detectado en cada puerto: 6, 3, 1 o 0
static unsigned antes[2];  // botones pulsados en la lectura anterior

void db9Init()
{
    for (int p = 0; p < 2; p++) {
        for (int i = 0; i < 7; i++) {
            if (i == 5) continue;                                  // SELECT se configura aparte
            gpio_init(PIN[p][i]);
            gpio_set_dir(PIN[p][i], GPIO_IN);
            gpio_pull_up(PIN[p][i]);
        }
        gpio_init(PIN[p][5]);
        gpio_put(PIN[p][5], 1);                                    // SELECT en reposo (alto)
        gpio_set_dir(PIN[p][5], GPIO_OUT);
    }
}

// Cambia SELECT, espera y devuelve las 6 lineas de datos (bit a 1 = linea a 0)
static int lineas(int p, bool sel)
{
    gpio_put(PIN[p][5], sel);
    sleep_us(20);
    uint32_t g = ~gpio_get_all();
    return ((g >> PIN[p][0]) & 1)      | ((g >> PIN[p][1]) & 1) << 1 | ((g >> PIN[p][2]) & 1) << 2 |
           ((g >> PIN[p][3]) & 1) << 3 | ((g >> PIN[p][4]) & 1) << 4 | ((g >> PIN[p][6]) & 1) << 5;
}

// Lee el puerto p (0 o 1) y devuelve el tipo: 6 = Sega 6 botones, 3 = Sega 3 botones, 1 = Atari, 0 = nada
static int detectar(int p)
{
    for (int i = 0; i < 8; i++) r[p][i] = lineas(p, i % 2 == 0);
    gpio_put(PIN[p][5], 1);
    if ((r[p][1] & 12) == 12 && (r[p][0] & 12) != 12) return (r[p][5] & 15) == 15 ? 6 : 3;
    return (r[p][0] | r[p][1]) & 63 ? 1 : 0;
}

int db9Tipo(int puerto)
{
    return tipo[puerto - 1];
}

// Lee los dos joysticks y envia a la placa ESP32 los codigos de los botones que han cambiado
void db9Process()
{
    for (int p = 0; p < 2; p++) {
        int t;
        if (tipo[p] == 1 && (r[p][0] = lineas(p, 1)) != 0) {
            r[p][1] = r[p][0];                 // Atari con algo pulsado: solo se lee, sin tocar SELECT
            t = 1;                             // (igual que la version anterior). El Atari no responde a SELECT
        } else {
            t = detectar(p);                   // en reposo se prueba la secuencia completa por si cambian el mando
        }
        if (t == 0 && tipo[p] == 1) t = 1;     // un Atari en reposo no se distingue de nada: lo conservamos
        tipo[p] = t;

        // Mapa de botones, un bit por boton: ARRIBA ABAJO IZQ. DER. B C A START Z Y X MODE
        unsigned m = 0;
        if (t == 1)                            // Atari: fuego -> A, segundo boton -> B
            m = (r[p][0] & 15) | ((r[p][0] & 0x10) << 2) | ((r[p][0] & 0x20) >> 1);
        else if (t >= 3)                       // Sega: direcciones, B, C, A y Start
            m = (r[p][0] & 0x3F) | (((r[p][1] >> 4) & 3) << 6);
        if (t == 6)                            // Sega 6 botones: Z, Y, X y Mode
            m |= (r[p][6] & 15) << 8;

        for (int i = 0; i < 12; i++) {
            bool pulsado = (m >> i) & 1;
            bool cambio  = pulsado != ((antes[p] >> i) & 1);
            if (cambio || (DB9_REPETIR_PULSADO && pulsado))
                Esp_code_send(ESP[p][i], pulsado);
        }
#ifdef DB9_DEBUG
        if (m != antes[p]) printf("DB9 joy%d tipo=%d r0=%02x r1=%02x botones=%03x\n", p + 1, t, r[p][0], r[p][1], m);
#endif
        antes[p] = m;
    }
}
