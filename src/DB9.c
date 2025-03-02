//Este codigo inicializa el joystick db 9 y ejecuta la ruitna de lectura de Joystick y mapea su correspondiente codigo
//Codigo inspirado por  por chatgpt
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


//inicializa los valores por defecto de la estructura, como el pull up de los gpio estan activado
//utilizamos logica inversa 1 desactivados , 0 para activados, para guardar el estado anterior
// lo hacemos normal 0 para desactivado y 1 para activado


JoystickState joy1={1,1,1,1,1,0,0,0,0,0}; //inicializamos la estrucutura con el valores por defecto
JoystickState joy2={1,1,1,1,1,0,0,0,0,0}; //inicializamos la estrucutura con el valores por defecto



//funcion para inicializar gpio's
void setGpio (uint8_t gpio) {  gpio_init(gpio); gpio_set_dir(gpio, GPIO_IN);gpio_pull_up(gpio);}

//funcion para inicializar gpio's
void db9Init()
{
    setGpio(joy1Up);setGpio(joy1Down);setGpio(joy1Left);setGpio(joy1Right);setGpio(joy1Fire);//inicializa puerto joy 1
    setGpio(joy2Up);setGpio(joy2Down);setGpio(joy2Left);setGpio(joy2Right);setGpio(joy2Fire);//inicializa puerto joy 2
}

// Función para leer el estado del joystick 
void read_joystick1(JoystickState *state) {
   
    // Leer el estado de cada botón 1 si no esta pulsado 0 si lo pulsamos
    state->up = gpio_get(joy1Up);
    state->down = gpio_get(joy1Down);
    state->left = gpio_get(joy1Left);
    state->right = gpio_get(joy1Right);
    state->fire = gpio_get(joy1Fire);
}

void read_joystick2(JoystickState *state) {
   
    // Leer el estado de cada botón 1 si no esta pulsado 0 si lo pulsamos
    state->up = gpio_get(joy2Up);
    state->down = gpio_get(joy2Down);
    state->left = gpio_get(joy2Left);
    state->right = gpio_get(joy2Right);
    state->fire = gpio_get(joy2Fire);
}
void db9Report()
{
    read_joystick1(&joy1);
    //printf("Joystick1: UP=%d, DOWN=%d, LEFT=%d, RIGHT=%d, FIRE=%d\n",  joy1.up, joy1.down, joy1.left,joy1.right, joy1.fire);}
    read_joystick2(&joy2);
    //printf("Joystick2: UP=%d, DOWN=%d, LEFT=%d, RIGHT=%d, FIRE=%d\n",  joy2.up, joy2.down, joy2.left,joy2.right, joy2.fire);}
    //if ((joy1.fire==0) || (joy1.up==0) || (joy1.down==0)||(joy1.lastDown==0)||(joy1.right==0)){verde;}
//Procesado Joy1
//----------------------------------------------------------------------------------------------------------------------
//disparo boton B
if (joy1.fire==0 && joy1.lastFire==1){/*kb_send_key(0x2b, 1, 0);*/Esp_code_send(ESP_JOY1A,1);}   //si ya  esta pulsado       
else if (joy1.fire==1 && joy1.lastFire==1){/*kb_send_key(0x2b, 0, 0);*/Esp_code_send(ESP_JOY1A,0);joy1.lastFire=0;}// lo hemos soltado hacem0s el release
if (joy1.fire==0 && joy1.lastFire==0){/*kb_send_key(0x2b, 1, 0);*/Esp_code_send(ESP_JOY1A,1);joy1.lastFire=1;}//lo acabamos de pulsar

//izquierda
if (joy1.left==0 && joy1.lastLeft==1){/*kb_send_key(0x50, 1, 0);*/Esp_code_send(ESP_JOY1LEFT,1);} 
else if (joy1.left==1 && joy1.lastLeft==1){/*kb_send_key(0x50, 0, 0);*/Esp_code_send(ESP_JOY1LEFT,0);joy1.lastLeft=0;}
if (joy1.left==0 && joy1.lastLeft==0){/*kb_send_key(0x50, 1, 0);*/Esp_code_send(ESP_JOY1LEFT,1);joy1.lastLeft=1;}  

//Derecha
if (joy1.right==0 && joy1.lastRight==1){/*kb_send_key(0x4f, 1, 0);*/Esp_code_send(ESP_JOY1RIGHT,1);} 
else if (joy1.right==1 && joy1.lastRight==1){/*kb_send_key(0x4f, 0, 0);*/Esp_code_send(ESP_JOY1RIGHT,0);joy1.lastRight=0;}
if (joy1.right==0 && joy1.lastRight==0){/*kb_send_key(0x4f, 1, 0);*/Esp_code_send(ESP_JOY1RIGHT,1);joy1.lastRight=1;}  


//arriba
if (joy1.up==0 && joy1.lastUp==1){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY1UP,1);} 
else if (joy1.up==1 && joy1.lastUp==1){/*kb_send_key(0x52, 0, 0);*/Esp_code_send(ESP_JOY1UP,0);joy1.lastUp=0;}
if (joy1.up==0 && joy1.lastUp==0){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY1UP,1);joy1.lastUp=1;}  
 

//abajo
if (joy1.down==0 && joy1.lastDown==1){/*kb_send_key(0x51, 1, 0);*/Esp_code_send(ESP_JOY1DOWN,1);} 
else if (joy1.down==1 && joy1.lastDown==1){/*kb_send_key(0x51, 0, 0);*/Esp_code_send(ESP_JOY1DOWN,0);joy1.lastDown=0;}
if (joy1.down==0 && joy1.lastDown==0){/*kb_send_key(0x51, 1, 0);*/Esp_code_send(ESP_JOY1DOWN,1);joy1.lastDown=1;}  
//----------------------------------------------------------------------------------------------------------------------

//Procesado Joy2
//----------------------------------------------------------------------------------------------------------------------
//disparo boton B
if (joy2.fire==0 && joy2.lastFire==1){/*kb_send_key(0x2b, 1, 0);*/Esp_code_send(ESP_JOY2A,1);}   //si ya  esta pulsado       
else if (joy2.fire==1 && joy2.lastFire==1){/*kb_send_key(0x2b, 0, 0);*/Esp_code_send(ESP_JOY2A,0);joy2.lastFire=0;}// lo hemos soltado hacem0s el release
if (joy2.fire==0 && joy2.lastFire==0){/*kb_send_key(0x2b, 1, 0);*/Esp_code_send(ESP_JOY2A,1);joy2.lastFire=1;}//lo acabamos de pulsar

//izquierda
if (joy2.left==0 && joy2.lastLeft==1){/*kb_send_key(0x50, 1, 0);*/Esp_code_send(ESP_JOY2LEFT,1);} 
else if (joy2.left==1 && joy2.lastLeft==1){/*kb_send_key(0x50, 0, 0);*/Esp_code_send(ESP_JOY2LEFT,0);joy2.lastLeft=0;}
if (joy2.left==0 && joy2.lastLeft==0){/*kb_send_key(0x50, 1, 0);*/Esp_code_send(ESP_JOY2LEFT,1);joy2.lastLeft=1;}  

//Derecha
if (joy2.right==0 && joy2.lastRight==1){/*kb_send_key(0x4f, 1, 0);*/Esp_code_send(ESP_JOY2RIGHT,1);} 
else if (joy2.right==1 && joy2.lastRight==1){/*kb_send_key(0x4f, 0, 0);*/Esp_code_send(ESP_JOY2RIGHT,0);joy2.lastRight=0;}
if (joy2.right==0 && joy2.lastRight==0){/*kb_send_key(0x4f, 1, 0);*/Esp_code_send(ESP_JOY2RIGHT,1);joy2.lastRight=1;}  


//arriba
if (joy2.up==0 && joy2.lastUp==1){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY2UP,1);} 
else if (joy2.up==1 && joy2.lastUp==1){/*kb_send_key(0x52, 0, 0);*/Esp_code_send(ESP_JOY2UP,0);joy2.lastUp=0;}
if (joy2.up==0 && joy2.lastUp==0){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY2UP,1);joy2.lastUp=1;}  
 

//abajo
if (joy2.down==0 && joy2.lastDown==1){/*kb_send_key(0x51, 1, 0);*/Esp_code_send(ESP_JOY2DOWN,1);} 
else if (joy2.down==1 && joy2.lastDown==1){/*kb_send_key(0x51, 0, 0);*/Esp_code_send(ESP_JOY2DOWN,0);joy2.lastDown=0;}
if (joy2.down==0 && joy2.lastDown==0){/*kb_send_key(0x51, 1, 0);*/Esp_code_send(ESP_JOY2DOWN,1);joy2.lastDown=1;}  
//----------------------------------------------------------------------------------------------------------------------


//sleep_ms(80);

}
