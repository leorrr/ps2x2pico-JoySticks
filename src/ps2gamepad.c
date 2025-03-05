#include "tusb.h"
#include "ps2out.h"
#include "ps2in.h"
#include <stdlib.h>
#include <stdio.h>
#include "scancodesets.h"
#include "espec_to_ps2.c"
#include "ps2x2pico.h"
//leo
// Estructura para manejar el estado del gamepad
typedef struct {
       bool up;
       bool down;
       bool left;
       bool right;
       bool buttonA;
       bool buttonB;
       bool buttonX;
       bool ButtonY;
       bool select;
       bool start;
       bool Ltrigger;
       bool Rtrigger;
       bool lastUp;
       bool lastDown;
       bool lastLeft;
       bool lastRight;
       bool lastButtonA;
       bool lastButtonB;
       bool lastButtonX;
       bool lastButtonY;
       bool lastSelect;
       bool lastStart;
       bool lastLtrigger;
       bool lastRtrigger;
   } gamepadState;
//leo   

//inicializa los valores por defecto de la estructura, como el pull up de los gpio estan activado
//utilizamos logica inversa 1 desactivados , 0 para activados, para guardar el estado anterior
// lo hacemos normal 0 para desactivado y 1 para activado


gamepadState gamePad1={0,0,0,0,0,0,0,0,0,0,0,0}; //inicializamos la estrucutura con el valores por defecto
gamepadState gamePad2={0,0,0,0,0,0,0,0,0,0,0,0}; //inicializamos la estrucutura con el valores por defecto

void readGamepad1(gamepadState *state,u8 const* report)
{
       state->up =(report[4]==0)?1:0; //esto se llama operador ternario
       state->down = (report[4])==255?1:0;
       state->left = (report[3])==0?1:0;
       state->right = (report[3])==255?1:0;
       state->buttonA = (report[5]==47)?1:0;
       state->buttonB = (report[5]==79)?1:0;
       state->buttonX = (report[5]==31)?1:0;
       state->ButtonY = (report[5]==143)?1:0;
       state->select =(report[6]==16)?1:0;
       state->start = (report[6]==32)?1:0;
       state->Ltrigger = (report[6]==1)?1:0;
       state->Rtrigger = (report[6]==2)?1:0;

}

void readGamepad2(gamepadState *state,u8 const* report)
{
       state->up =(report[4]==0)?1:0; //esto se llama operador ternario
       state->down = (report[4])==255?1:0;
       state->left = (report[3])==0?1:0;
       state->right = (report[3])==255?1:0;
       state->buttonA = (report[5]==47)?1:0;
       state->buttonB = (report[5]==79)?1:0;
       state->buttonX = (report[5]==31)?1:0;
       state->ButtonY = (report[5]==143)?1:0;
       state->select =(report[6]==16)?1:0;
       state->start = (report[6]==32)?1:0;
       state->Ltrigger = (report[6]==1)?1:0;
       state->Rtrigger = (report[6]==2)?1:0;

}

void Gamepad1Process(u8 const* report)
{
       readGamepad1(&gamePad1,report);
//arriba
if (gamePad1.up==1 && gamePad1.lastUp==1){Esp_code_send(ESP_JOY1UP,1);} //si esta pulsado
else if (gamePad1.up==0 && gamePad1.lastUp==1){Esp_code_send(ESP_JOY1UP,0);gamePad1.lastUp=0;}//si hemos soltado
if (gamePad1.up==1 && gamePad1.lastUp==0){Esp_code_send(ESP_JOY1UP,1);gamePad1.lastUp=1;}//Lo acabamos de pulsar

//Abajo
if (gamePad1.down==1 && gamePad1.lastDown==1){Esp_code_send(ESP_JOY1DOWN,1);} //si esta pulsado
else if (gamePad1.down==0 && gamePad1.lastDown==1){Esp_code_send(ESP_JOY1DOWN,0);gamePad1.lastDown=0;}//si hemos soltado
if (gamePad1.down==1 && gamePad1.lastDown==0){Esp_code_send(ESP_JOY1DOWN,1);gamePad1.lastDown=1;}//Lo acabamos de pulsar

//izquierda
if (gamePad1.left==1 && gamePad1.lastLeft==1){Esp_code_send(ESP_JOY1LEFT,1);} //si esta pulsado
else if (gamePad1.left==0 && gamePad1.lastLeft==1){Esp_code_send(ESP_JOY1LEFT,0);gamePad1.lastLeft=0;}//si hemos soltado
if (gamePad1.left==1 && gamePad1.lastLeft==0){Esp_code_send(ESP_JOY1LEFT,1);gamePad1.lastLeft=1;}//Lo acabamos de pulsar

//derecha
if (gamePad1.right==1 && gamePad1.lastRight==1){Esp_code_send(ESP_JOY1RIGHT,1);} //si esta pulsado
else if (gamePad1.right==0 && gamePad1.lastRight==1){Esp_code_send(ESP_JOY1RIGHT,0);gamePad1.lastRight=0;}//si hemos soltado
if (gamePad1.right==1 && gamePad1.lastRight==0){Esp_code_send(ESP_JOY1RIGHT,1);gamePad1.lastRight;}//Lo acabamos de pulsar

//Boton A
if (gamePad1.buttonA==1 && gamePad1.lastButtonA==1){Esp_code_send(ESP_JOY1A,1);} //si esta pulsado
else if (gamePad1.buttonA==0 && gamePad1.lastButtonA==1){Esp_code_send(ESP_JOY1A,0);gamePad1.lastButtonA=0;}//si hemos soltado
if (gamePad1.buttonA==1 && gamePad1.lastButtonA==0){Esp_code_send(ESP_JOY1A,1);gamePad1.lastButtonA;}//Lo acabamos de pulsar

//Boton B
if (gamePad1.buttonB==1 && gamePad1.lastButtonB==1){Esp_code_send(ESP_JOY1B,1);} //si esta pulsado
else if (gamePad1.buttonA==0 && gamePad1.lastButtonB==1){Esp_code_send(ESP_JOY1B,0);gamePad1.lastButtonB=0;}//si hemos soltado
if (gamePad1.buttonB==1 && gamePad1.lastButtonB==0){Esp_code_send(ESP_JOY1B,1);gamePad1.lastButtonB;}//Lo acabamos de pulsar

//Boton X
if (gamePad1.buttonX==1 && gamePad1.lastButtonX==1){Esp_code_send(ESP_JOY1X,1);} //si esta pulsado
else if (gamePad1.buttonX==0 && gamePad1.lastButtonX==1){Esp_code_send(ESP_JOY1X,0);gamePad1.lastButtonX=0;}//si hemos soltado
if (gamePad1.buttonX==1 && gamePad1.lastButtonX==0){Esp_code_send(ESP_JOY1X,1);gamePad1.lastButtonX;}//Lo acabamos de pulsar

//Select
if (gamePad1.select==1 && gamePad1.lastSelect==1){Esp_code_send(ESP_JOY1MODE,1);} //si esta pulsado
else if (gamePad1.select==0 && gamePad1.lastSelect==1){Esp_code_send(ESP_JOY1MODE,0);gamePad1.lastSelect=0;}//si hemos soltado
if (gamePad1.select==1 && gamePad1.lastSelect==0){Esp_code_send(ESP_JOY1MODE,1);gamePad1.lastSelect;}//Lo acabamos de pulsar

//Start
if (gamePad1.start==1 && gamePad1.lastStart==1){Esp_code_send(ESP_JOY1START,1);} //si esta pulsado
else if (gamePad1.start==0 && gamePad1.lastStart==1){Esp_code_send(ESP_JOY1START,0);gamePad1.lastStart=0;}//si hemos soltado
if (gamePad1.start==1 && gamePad1.lastStart==0){Esp_code_send(ESP_JOY1START,1);gamePad1.lastStart;}//Lo acabamos de pulsar

//LTrigger
if (gamePad1.Ltrigger==1 && gamePad1.lastLtrigger==1){Esp_code_send(ESP_JOY1C,1);} //si esta pulsado
else if (gamePad1.Ltrigger==0 && gamePad1.lastLtrigger==1){Esp_code_send(ESP_JOY1START,0);gamePad1.lastLtrigger=0;}//si hemos soltado
if (gamePad1.Ltrigger==1 && gamePad1.lastLtrigger==0){Esp_code_send(ESP_JOY1START,1);gamePad1.lastLtrigger;}//Lo acabamos de pulsar

//RTrigger
if (gamePad1.Rtrigger==1 && gamePad1.lastRtrigger==1){Esp_code_send(ESP_JOY1Z,1);} //si esta pulsado
else if (gamePad1.Rtrigger==0 && gamePad1.lastRtrigger==1){Esp_code_send(ESP_JOY1Z,0);gamePad1.lastRtrigger=0;}//si hemos soltado
if (gamePad1.Rtrigger==1 && gamePad1.lastRtrigger==0){Esp_code_send(ESP_JOY1Z,1);gamePad1.lastRtrigger;}//Lo acabamos de pulsar

}

void Gamepad2Process(u8 const* report)
{
       readGamepad2(&gamePad2, report);
//arriba
if (gamePad2.up==1 && gamePad2.lastUp==1){Esp_code_send(ESP_JOY1UP,1);} //si esta pulsado
else if (gamePad2.up==0 && gamePad2.lastUp==1){Esp_code_send(ESP_JOY1UP,0);gamePad2.lastUp=0;}//si hemos soltado
if (gamePad2.up==1 && gamePad2.lastUp==0){Esp_code_send(ESP_JOY1UP,1);gamePad2.lastUp=1;}//Lo acabamos de pulsar

//Abajo
if (gamePad2.down==1 && gamePad2.lastDown==1){Esp_code_send(ESP_JOY1DOWN,1);} //si esta pulsado
else if (gamePad2.down==0 && gamePad2.lastDown==1){Esp_code_send(ESP_JOY1DOWN,0);gamePad2.lastDown=0;}//si hemos soltado
if (gamePad2.down==1 && gamePad2.lastDown==0){Esp_code_send(ESP_JOY1DOWN,1);gamePad2.lastDown=1;}//Lo acabamos de pulsar

//izquierda
if (gamePad2.left==1 && gamePad2.lastLeft==1){Esp_code_send(ESP_JOY1LEFT,1);} //si esta pulsado
else if (gamePad2.left==0 && gamePad2.lastLeft==1){Esp_code_send(ESP_JOY1LEFT,0);gamePad2.lastLeft=0;}//si hemos soltado
if (gamePad2.left==1 && gamePad2.lastLeft==0){Esp_code_send(ESP_JOY1LEFT,1);gamePad2.lastLeft=1;}//Lo acabamos de pulsar

//derecha
if (gamePad2.right==1 && gamePad2.lastRight==1){Esp_code_send(ESP_JOY1RIGHT,1);} //si esta pulsado
else if (gamePad2.right==0 && gamePad2.lastRight==1){Esp_code_send(ESP_JOY1RIGHT,0);gamePad2.lastRight=0;}//si hemos soltado
if (gamePad2.right==1 && gamePad2.lastRight==0){Esp_code_send(ESP_JOY1RIGHT,1);gamePad2.lastRight;}//Lo acabamos de pulsar

//Boton A
if (gamePad2.buttonA==1 && gamePad2.lastButtonA==1){Esp_code_send(ESP_JOY1A,1);} //si esta pulsado
else if (gamePad2.buttonA==0 && gamePad2.lastButtonA==1){Esp_code_send(ESP_JOY1A,0);gamePad2.lastButtonA=0;}//si hemos soltado
if (gamePad2.buttonA==1 && gamePad2.lastButtonA==0){Esp_code_send(ESP_JOY1A,1);gamePad2.lastButtonA;}//Lo acabamos de pulsar

//Boton B
if (gamePad2.buttonB==1 && gamePad2.lastButtonB==1){Esp_code_send(ESP_JOY1B,1);} //si esta pulsado
else if (gamePad2.buttonA==0 && gamePad2.lastButtonB==1){Esp_code_send(ESP_JOY1B,0);gamePad2.lastButtonB=0;}//si hemos soltado
if (gamePad2.buttonB==1 && gamePad2.lastButtonB==0){Esp_code_send(ESP_JOY1B,1);gamePad2.lastButtonB;}//Lo acabamos de pulsar

//Boton X
if (gamePad2.buttonX==1 && gamePad2.lastButtonX==1){Esp_code_send(ESP_JOY1X,1);} //si esta pulsado
else if (gamePad2.buttonX==0 && gamePad2.lastButtonX==1){Esp_code_send(ESP_JOY1X,0);gamePad2.lastButtonX=0;}//si hemos soltado
if (gamePad2.buttonX==1 && gamePad2.lastButtonX==0){Esp_code_send(ESP_JOY1X,1);gamePad2.lastButtonX;}//Lo acabamos de pulsar

//Select
if (gamePad2.select==1 && gamePad2.lastSelect==1){Esp_code_send(ESP_JOY1MODE,1);} //si esta pulsado
else if (gamePad2.select==0 && gamePad2.lastSelect==1){Esp_code_send(ESP_JOY1MODE,0);gamePad2.lastSelect=0;}//si hemos soltado
if (gamePad2.select==1 && gamePad2.lastSelect==0){Esp_code_send(ESP_JOY1MODE,1);gamePad2.lastSelect;}//Lo acabamos de pulsar

//Start
if (gamePad2.start==1 && gamePad2.lastStart==1){Esp_code_send(ESP_JOY1START,1);} //si esta pulsado
else if (gamePad2.start==0 && gamePad2.lastStart==1){Esp_code_send(ESP_JOY1START,0);gamePad2.lastStart=0;}//si hemos soltado
if (gamePad2.start==1 && gamePad2.lastStart==0){Esp_code_send(ESP_JOY1START,1);gamePad2.lastStart;}//Lo acabamos de pulsar

//LTrigger
if (gamePad2.Ltrigger==1 && gamePad2.lastLtrigger==1){Esp_code_send(ESP_JOY1C,1);} //si esta pulsado
else if (gamePad2.Ltrigger==0 && gamePad2.lastLtrigger==1){Esp_code_send(ESP_JOY1START,0);gamePad2.lastLtrigger=0;}//si hemos soltado
if (gamePad2.Ltrigger==1 && gamePad2.lastLtrigger==0){Esp_code_send(ESP_JOY1START,1);gamePad2.lastLtrigger;}//Lo acabamos de pulsar

//RTrigger
if (gamePad2.Rtrigger==1 && gamePad2.lastRtrigger==1){Esp_code_send(ESP_JOY1Z,1);} //si esta pulsado
else if (gamePad2.Rtrigger==0 && gamePad2.lastRtrigger==1){Esp_code_send(ESP_JOY1Z,0);gamePad2.lastRtrigger=0;}//si hemos soltado
if (gamePad2.Rtrigger==1 && gamePad2.lastRtrigger==0){Esp_code_send(ESP_JOY1Z,1);gamePad2.lastRtrigger;}//Lo acabamos de pulsar

}

