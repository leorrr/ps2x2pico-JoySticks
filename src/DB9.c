//Este codigo inilicializa el joystick db 9 y ejecuta la ruitna de lectura de Joystick y mapea su correspondiente codigo
//generado por chatgpt
// Joy1
#define joy1Up 2 
#define joy1Down 3
#define joy1Left 4 
#define joy1Right 5 
#define joy1Fire 6 
#define joy1Select 7 // Para joyStick SEGA
#define joy1Start 8 // Para joyStick SEGA

//Joy2
#define joy2Up 29 
#define joy2Down 28
#define joy2Left 27
#define joy2Right 26 
#define joy2Fire 10  
#define joy2Select 9  // Para joyStick SEGA
#define joy2Start 1  // Para joyStick SEGA

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "scancodesets.h"
#include "espectrumCodeSet.h"
#include "neopixel.h"
//#include "espec_to_ps2.c"
/*
#define JOYSTICK_UP_PIN 2
#define JOYSTICK_DOWN_PIN 3
#define JOYSTICK_LEFT_PIN 4
#define JOYSTICK_RIGHT_PIN 5
#define JOYSTICK_FIRE_PIN 6 */

#define DEBOUNCE_DELAY_MS 50  // Retardo para anti-rebote en milisegundos

// Estructura para manejar el estado del joystick
typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
    bool fire;
    bool lastUp;
    bool lastDown;
    bool lastLeft;
    bool lastRight;
    bool lastFire;
} JoystickState;


//JoystickState joystick_state = {1};
JoystickState joy1={1,1,1,1,1,0,0,0,0,0}; //inicializamos la estrucutura con el valores por defecto



//funcion para inicializar gpio's
void setGpio (uint8_t gpio) {  gpio_init(gpio); gpio_set_dir(gpio, GPIO_IN);gpio_pull_up(gpio);}

//funcion para inicializar gpio's
void db9Init()
{
    setGpio(joy1Up);setGpio(joy1Down);setGpio(joy1Left);setGpio(joy1Right);setGpio(joy1Fire);
}

// Función para leer el estado del joystick con anti-rebote
void read_joystick(JoystickState *state) {
    /*static uint32_t last_read_time = 0;

    // Verificar si ha pasado suficiente tiempo para el anti-rebote
    if (absolute_time_diff_us(get_absolute_time(), last_read_time) < DEBOUNCE_DELAY_MS ) {
        return;
    }

    last_read_time = get_absolute_time();*/

    // Leer el estado de cada botón 1 si no esta pulsado 0 si lo pulsamos
    state->up = gpio_get(joy1Up);
    state->down = gpio_get(joy1Down);
    state->left = gpio_get(joy1Left);
    state->right = gpio_get(joy1Right);
    state->fire = gpio_get(joy1Fire);
}
void db9Report()
{
    read_joystick(&joy1);
    //printf("Joystick: UP=%d, DOWN=%d, LEFT=%d, RIGHT=%d, FIRE=%d\n",  joy1.up, joy1.down, joy1.left,joy1.right, joy1.fire);}
    //if ((joy1.fire==0) || (joy1.up==0) || (joy1.down==0)||(joy1.lastDown==0)||(joy1.right==0)){verde;}

//disparo boton B
if (joy1.fire==0 && joy1.lastFire==1){/*kb_send_key(0x2b, 1, 0);*/Esp_code_send(ESP_JOY2B,1);}   //si ya  esta pulsado       
else if (joy1.fire==1 && joy1.lastFire==1){/*kb_send_key(0x2b, 0, 0);*/Esp_code_send(ESP_JOY2B,0);joy1.lastFire=0;}// lo hemos soltado hacem0s el release
if (joy1.fire==0 && joy1.lastFire==0){/*kb_send_key(0x2b, 1, 0);*/Esp_code_send(ESP_JOY2B,1);joy1.lastFire=1;}//lo acabamos de pulsar

//izquierda
if (joy1.left==0 && joy1.lastLeft==1){/*kb_send_key(0x50, 1, 0);*/Esp_code_send(ESP_JOY2LEFT,1);} 
else if (joy1.left==1 && joy1.lastLeft==1){/*kb_send_key(0x50, 0, 0);*/Esp_code_send(ESP_JOY2LEFT,0);joy1.lastLeft=0;}
if (joy1.left==0 && joy1.lastLeft==0){/*kb_send_key(0x50, 1, 0);*/Esp_code_send(ESP_JOY2LEFT,1);joy1.lastLeft=1;}  

//Derecha
if (joy1.right==0 && joy1.lastRight==1){/*kb_send_key(0x4f, 1, 0);*/Esp_code_send(ESP_JOY2RIGHT,1);} 
else if (joy1.right==1 && joy1.lastRight==1){/*kb_send_key(0x4f, 0, 0);*/Esp_code_send(ESP_JOY2RIGHT,0);joy1.lastRight=0;}
if (joy1.right==0 && joy1.lastRight==0){/*kb_send_key(0x4f, 1, 0);*/Esp_code_send(ESP_JOY2RIGHT,1);joy1.lastRight=1;}  


//arriba
if (joy1.up==0 && joy1.lastUp==1){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY2UP,1);} 
else if (joy1.up==1 && joy1.lastUp==1){/*kb_send_key(0x52, 0, 0);*/Esp_code_send(ESP_JOY2UP,0);joy1.lastUp=0;}
if (joy1.up==0 && joy1.lastUp==0){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY2UP,1);joy1.lastUp=1;}  
 

//abajo
if (joy1.down==0 && joy1.lastDown==1){/*kb_send_key(0x51, 1, 0);*/Esp_code_send(ESP_JOY2DOWN,1);} 
else if (joy1.down==1 && joy1.lastDown==1){/*kb_send_key(0x51, 0, 0);*/Esp_code_send(ESP_JOY2DOWN,0);joy1.lastDown=0;}
if (joy1.down==0 && joy1.lastDown==0){/*kb_send_key(0x51, 1, 0);*/Esp_code_send(ESP_JOY2DOWN,1);joy1.lastDown=1;}  
//sleep_ms(80);

}

/*
int main() {
    // Inicializar la comunicación serie
    stdio_init_all();
    gpioInit();
    /*
    // Inicializar los pines GPIO
    gpio_init(JOYSTICK_UP_PIN);
    gpio_set_dir(JOYSTICK_UP_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_UP_PIN);

    gpio_init(JOYSTICK_DOWN_PIN);
    gpio_set_dir(JOYSTICK_DOWN_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_DOWN_PIN);

    gpio_init(JOYSTICK_LEFT_PIN);
    gpio_set_dir(JOYSTICK_LEFT_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_LEFT_PIN);

    gpio_init(JOYSTICK_RIGHT_PIN);
    gpio_set_dir(JOYSTICK_RIGHT_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_RIGHT_PIN);

    gpio_init(JOYSTICK_FIRE_PIN);
    gpio_set_dir(JOYSTICK_FIRE_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_FIRE_PIN);
*/
   /*
while (true) {
        // Leer el estado del joystick
        read_joystick(&joy1);

        // Imprimir el estado por pantalla
        printf("Joystick: UP=%d, DOWN=%d, LEFT=%d, RIGHT=%d, FIRE=%d\n",
               joystick_state.up, joystick_state.down, joystick_state.left,
               joystick_state.right, joystick_state.fire);

        sleep_ms(100); // Pequeña pausa para evitar sobrecargar la salida serie
    }

    return 0;
} */