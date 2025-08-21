//Para procesar report X360 controler

//#include "tusb.h" si falla descomentarlo
//#include "ps2out.h"
//#include "ps2in.h"
#include <stdlib.h>
#include <stdio.h>
#include "scancodesets.h"
//#include "espec_to_ps2.c"
#include "espectrumCodeSet.h"
//#include "ps2x2pico.h"
//#include "xinput_host.h"
#include "ps2x360.h"
#define DEADZONE 9000


/*
//leo


// Estructura para manejar el estado del x360 controler 23 acciones
typedef struct {
       bool d_up;
       bool d_down;
       bool d_left;
       bool d_right;
       bool buttonA;
       bool buttonB;
       bool buttonX;
       bool ButtonY;
       bool back;
       bool start;
       bool guide;
       bool Ltrigger;
       bool Rtrigger;
       bool Lbumper;
       bool Rbumper;
       bool ls_up;
       bool ls_down;
       bool ls_left;
       bool ls_right;
       bool rs_up;
       bool rs_down;
       bool rs_left;
       bool rs_right;

       bool lastd_up;
       bool lastd_down;
       bool lastd_left;
       bool lastd_right;
       bool lastbuttonA;
       bool lastbuttonB;
       bool lastbuttonX;
       bool lastButtonY;
       bool lastback;
       bool laststart;
       bool lastguide;
       bool lastLtrigger;
       bool lastRtrigger;
       bool lastLbumper;
       bool lastRbumper;
       bool lastls_up;
       bool lastls_down;
       bool lastls_left;
       bool lastls_right;
       bool lastrs_up;
       bool lastrs_down;
       bool lastrs_left;
       bool lastrs_right;
   } x360PadState;
//leo   
*/
//inicializa los valores por defecto de la estructura, En DB9 como el pull up de los gpio estan activado
//utilizamos logica inversa 1 desactivados , 0 para activados, para guardar el estado anterior
// En el gamepad lo hacemos normal 0 para desactivado y 1 para activado


x360PadState x360Pad1={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //inicializamos la estrucutura con el valores por defecto
x360PadState x360Pad2={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //inicializamos la estrucutura con el valores por defecto

void readx360pad1(x360PadState *state,int16_t const* report)
{
       uint16_t btn = report[0];
       state->d_up =(btn & XINPUT_GAMEPAD_DPAD_UP)?1:0; //esto se llama operador ternario
       state->d_down = (btn & XINPUT_GAMEPAD_DPAD_DOWN)?1:0;
       state->d_left = (btn & XINPUT_GAMEPAD_DPAD_LEFT)?1:0;
       state->d_right = (btn & XINPUT_GAMEPAD_DPAD_RIGHT)?1:0;
       state->buttonA = (btn & XINPUT_GAMEPAD_A)?1:0;
       state->buttonB = (btn & XINPUT_GAMEPAD_B)?1:0;
       state->buttonX = (btn & XINPUT_GAMEPAD_X)?1:0;
       state->buttonY = (btn & XINPUT_GAMEPAD_Y)?1:0;
       state->back =(btn & XINPUT_GAMEPAD_BACK)?1:0;
       state->start = (btn & XINPUT_GAMEPAD_START)?1:0;
       state->guide = (btn & XINPUT_GAMEPAD_GUIDE)?1:0;
       state->Lshoulder = (btn & XINPUT_GAMEPAD_LEFT_SHOULDER)?1:0;
       state->Rshoulder=(btn & XINPUT_GAMEPAD_RIGHT_SHOULDER)?1:0;
       state->Lbumper = (btn & XINPUT_GAMEPAD_LEFT_THUMB)?1:0;
       state->Rbumper=(btn & XINPUT_GAMEPAD_RIGHT_THUMB)?1:0;
       state->Ltrigger = (report[1] > 100) ?1:0;
       state->Rtrigger = (report[2] > 10) ?1:0;
       state->ls_up=(report[4] > DEADZONE)?1:0;
       state->ls_down=(report[4] < -DEADZONE)?1:0;
       state->ls_left=(report[3] < -DEADZONE)?1:0;
       state->ls_right=(report[3] > DEADZONE)?1:0;
       state->rs_up=(report[5] > DEADZONE)?1:0;
       state->rs_down=(report[5] < -DEADZONE)?1:0;
       state->rs_left=(report[6] > DEADZONE)?1:0;
       state->rs_right=(report[6] < -DEADZONE)?1:0;

}

void readx360Pad2(x360PadState *state,int16_t const* report)
{    uint16_t btn = report[0];
       state->d_up =(btn & XINPUT_GAMEPAD_DPAD_UP)?1:0; //esto se llama operador ternario
       state->d_down = (btn & XINPUT_GAMEPAD_DPAD_DOWN)?1:0;
       state->d_left = (btn & XINPUT_GAMEPAD_DPAD_LEFT)?1:0;
       state->d_right = (btn & XINPUT_GAMEPAD_DPAD_RIGHT)?1:0;
       state->buttonA = (btn & XINPUT_GAMEPAD_A)?1:0;
       state->buttonB = (btn & XINPUT_GAMEPAD_B)?1:0;
       state->buttonX = (btn & XINPUT_GAMEPAD_X)?1:0;
       state->buttonY = (btn & XINPUT_GAMEPAD_Y)?1:0;
       state->back =(btn & XINPUT_GAMEPAD_BACK)?1:0;
       state->start = (btn & XINPUT_GAMEPAD_START)?1:0;
       state->guide = (btn & XINPUT_GAMEPAD_GUIDE)?1:0;
       state->Lshoulder = (btn & XINPUT_GAMEPAD_LEFT_SHOULDER)?1:0;
       state->Rshoulder=(btn & XINPUT_GAMEPAD_RIGHT_SHOULDER)?1:0;
       state->Lbumper = (btn & XINPUT_GAMEPAD_LEFT_THUMB)?1:0;
       state->Rbumper=(btn & XINPUT_GAMEPAD_RIGHT_THUMB)?1:0;
       state->Ltrigger = (report[1] > 100) ?1:0;
       state->Rtrigger = (report[2] > 10) ?1:0;
       state->ls_up=(report[4] > DEADZONE)?1:0;
       state->ls_down=(report[4] < -DEADZONE)?1:0;
       state->ls_left=(report[3] < -DEADZONE)?1:0;
       state->ls_right=(report[3] > DEADZONE)?1:0;
       state->rs_up=(report[5] > DEADZONE)?1:0;
       state->rs_down=(report[5] < -DEADZONE)?1:0;
       state->rs_left=(report[6] > DEADZONE)?1:0;
       state->rs_right=(report[6] < -DEADZONE)?1:0;

}
//void x360Process1(u16 const* report)
void x360Process1(int16_t myreport[])

{ 
/*
        if (myreport[3] > DEADZONE)        printf("Stick Izq → \n");
        else if (myreport[3]< -DEADZONE)  printf("Stick Izq ←\n");

        if (myreport[4] > DEADZONE)        printf("Stick Izq ↑ %d\n",myreport[4]);
        else if (myreport[4] < -DEADZONE)  printf("Stick Izq ↓ %d\n",myreport[4]);

        // --- Stick derecho ---
        if (myreport[5] > DEADZONE)        printf("Stick Der →\n");
        else if (myreport   [5] < -DEADZONE)  printf("Stick Der ←\n");

        if (myreport[6] > DEADZONE)        printf("Stick Der ↑\n");
        else if (myreport[6] < -DEADZONE)  printf("Stick Der ↓\n");

        // --- Gatillos ---
        if (myreport[1] > 10)              printf("Trigger Izq presionado (%d)\n", myreport[1]);
        if (myreport[2] > 10)              printf("Trigger Der presionado (%d)\n", myreport[2]);

        uint16_t btn = myreport[0];
        // --- Botones ---
        if (btn & XINPUT_GAMEPAD_A)      printf("Botón A\n");
        if (btn & XINPUT_GAMEPAD_B)      printf("Botón B\n");
        if (btn & XINPUT_GAMEPAD_X)      printf("Botón X\n");
        if (btn & XINPUT_GAMEPAD_Y)      printf("Botón Y\n");

        if (btn & XINPUT_GAMEPAD_LEFT_SHOULDER)  printf("Botón LB\n");
        if (btn & XINPUT_GAMEPAD_RIGHT_SHOULDER) printf("Botón RB\n");

        if (btn & XINPUT_GAMEPAD_BACK)   printf("Botón Back\n");
        if (btn & XINPUT_GAMEPAD_START)  printf("Botón Start\n");

        if (btn & XINPUT_GAMEPAD_LEFT_THUMB)  printf("Stick Izq presionado\n");
        if (btn & XINPUT_GAMEPAD_RIGHT_THUMB) printf("Stick Der presionado\n");

        if (btn & XINPUT_GAMEPAD_DPAD_UP)    printf("DPad ↑\n");
        if (btn & XINPUT_GAMEPAD_DPAD_DOWN)  printf("DPad ↓\n");
        if (btn & XINPUT_GAMEPAD_DPAD_LEFT)  printf("DPad ←\n");
        if (btn & XINPUT_GAMEPAD_DPAD_RIGHT) printf("DPad →\n");
      //printf("report0(Buttons): %04x, report1(BLT): %02x, report2(BRT): %02x, report3(LX): %d, report4(LY): %d, report5(RX): %d report6(RY): %d \n",report[0],report[1],report[2],report[3],report[4],report[5],report[6]);
    /*   if (report[0] & 0x1000) printf("You are pressing A\n");
        // --- Stick izquierdo ---
       if (report[3] > DEADZONE)      printf("Stick Izq →\n");
        else if (report[3] < -DEADZONE) printf("Stick Izq ←\n");

       if (report[4] > DEADZONE)      printf("Stick Izq ↑\n");
        else if (report[4] < -DEADZONE) printf("Stick Izq ↓\n");
  
        // --- Stick derecho ---
        if (report[5] > DEADZONE)      printf("Stick Der →\n");
        else if (report[5]< -DEADZONE) printf("Stick Der ←\n");

        if (report[6] > DEADZONE)      printf("Stick Der ↑\n");
        else if (report[6] < -DEADZONE) printf("Stick Der ↓\n"); */

       readx360pad1(&x360Pad1,myreport);

//arriba

if (x360Pad1.d_up==1 && x360Pad1.lastd_up==1){Esp_code_send(ESP_JOY1UP,1);} //si esta pulsado
else if (x360Pad1.d_up==0 && x360Pad1.lastd_up==1){Esp_code_send(ESP_JOY1UP,0);x360Pad1.lastd_up=0;}//si hemos soltado
if (x360Pad1.d_up==1 && x360Pad1.lastd_up==0){Esp_code_send(ESP_JOY1UP,1);x360Pad1.lastd_up=1; }//Lo acabamos de pulsar

 //arriba LS
if (x360Pad1.ls_up==1 && x360Pad1.lastls_up==1){Esp_code_send(ESP_JOY1UP,1);} //si esta pulsado
else if (x360Pad1.ls_up==0 && x360Pad1.lastls_up==1){Esp_code_send(ESP_JOY1UP,0);x360Pad1.lastls_up=0;}//si hemos soltado
if (x360Pad1.ls_up==1 && x360Pad1.lastls_up==0){Esp_code_send(ESP_JOY1UP,1);x360Pad1.lastls_up=1;}//Lo acabamos de pulsar


//Abajo
if (x360Pad1.d_down==1 && x360Pad1.lastd_down==1){Esp_code_send(ESP_JOY1DOWN,1);} //si esta pulsado
else if (x360Pad1.d_down==0 && x360Pad1.lastd_down==1){Esp_code_send(ESP_JOY1DOWN,0);x360Pad1.lastd_down=0;}//si hemos soltado
if (x360Pad1.d_down==1 && x360Pad1.lastd_down==0){Esp_code_send(ESP_JOY1DOWN,1);x360Pad1.lastd_down=1;}//Lo acabamos de pulsar

//abajo LS

if (x360Pad1.ls_down==1 && x360Pad1.lastls_down==1){Esp_code_send(ESP_JOY1DOWN,1);} //si esta pulsado
else if (x360Pad1.ls_down==0 && x360Pad1.lastls_down==1){Esp_code_send(ESP_JOY1DOWN,0);x360Pad1.lastls_down=0;}//si hemos soltado
if (x360Pad1.ls_down==1 && x360Pad1.lastls_down==0){Esp_code_send(ESP_JOY1DOWN,1);x360Pad1.lastls_down=1;}//Lo acabamos de pulsar


//izquierda
if (x360Pad1.d_left==1 && x360Pad1.lastd_left==1){Esp_code_send(ESP_JOY1LEFT,1);} //si esta pulsado
else if (x360Pad1.d_left==0 && x360Pad1.lastd_left==1){Esp_code_send(ESP_JOY1LEFT,0);x360Pad1.lastd_left=0;}//si hemos soltado
if (x360Pad1.d_left==1 && x360Pad1.lastd_left==0){Esp_code_send(ESP_JOY1LEFT,1);x360Pad1.lastd_left=1;}//Lo acabamos de pulsar

 //Izquierda LS

if (x360Pad1.ls_left==1 && x360Pad1.lastls_left==1){Esp_code_send(ESP_JOY1LEFT,1);} //si esta pulsado
else if (x360Pad1.ls_left==0 && x360Pad1.lastls_left==1){Esp_code_send(ESP_JOY1LEFT,0);x360Pad1.lastls_left=0;}//si hemos soltado
if (x360Pad1.ls_left==1 && x360Pad1.lastls_left==0){Esp_code_send(ESP_JOY1LEFT,1);x360Pad1.lastls_left=1;}//Lo acabamos de pulsar

//derecha
if (x360Pad1.d_right==1 && x360Pad1.lastd_right==1){Esp_code_send(ESP_JOY1RIGHT,1);} //si esta pulsado
else if (x360Pad1.d_right==0 && x360Pad1.lastd_right==1){Esp_code_send(ESP_JOY1RIGHT,0);x360Pad1.lastd_right=0;}//si hemos soltado
if (x360Pad1.d_right==1 && x360Pad1.lastd_right==0){Esp_code_send(ESP_JOY1RIGHT,1);x360Pad1.lastd_right=1;}//Lo acabamos de pulsar

//Derecha LS

if (x360Pad1.ls_right==1 && x360Pad1.lastls_right==1){Esp_code_send(ESP_JOY1RIGHT,1);} //si esta pulsado
else if (x360Pad1.ls_right==0 && x360Pad1.lastls_right==1){Esp_code_send(ESP_JOY1RIGHT,0);x360Pad1.lastls_right=0;}//si hemos soltado
if (x360Pad1.ls_right==1 && x360Pad1.lastls_right==0){Esp_code_send(ESP_JOY1RIGHT,1);x360Pad1.lastls_right=1;}//Lo acabamos de pulsar

//Boton A
if (x360Pad1.buttonA==1 && x360Pad1.lastbuttonA==1){Esp_code_send(ESP_JOY1A,1);} //si esta pulsado
else if (x360Pad1.buttonA==0 && x360Pad1.lastbuttonA==1){Esp_code_send(ESP_JOY1A,0);x360Pad1.lastbuttonA=0;}//si hemos soltado
if (x360Pad1.buttonA==1 && x360Pad1.lastbuttonA==0){Esp_code_send(ESP_JOY1A,1);x360Pad1.lastbuttonA=1;}//Lo acabamos de pulsar

//Boton B
if (x360Pad1.buttonB==1 && x360Pad1.lastbuttonB==1){Esp_code_send(ESP_JOY1B,1);} //si esta pulsado
else if (x360Pad1.buttonB==0 && x360Pad1.lastbuttonB==1){Esp_code_send(ESP_JOY1B,0);x360Pad1.lastbuttonB=0;}//si hemos soltado
if (x360Pad1.buttonB==1 && x360Pad1.lastbuttonB==0){Esp_code_send(ESP_JOY1B,1);x360Pad1.lastbuttonB=1;}//Lo acabamos de pulsar

//Boton X
if (x360Pad1.buttonX==1 && x360Pad1.lastbuttonX==1){Esp_code_send(ESP_JOY1X,1);} //si esta pulsado
else if (x360Pad1.buttonX==0 && x360Pad1.lastbuttonX==1){Esp_code_send(ESP_JOY1X,0);x360Pad1.lastbuttonX=0;}//si hemos soltado
if (x360Pad1.buttonX==1 && x360Pad1.lastbuttonX==0){Esp_code_send(ESP_JOY1X,1);x360Pad1.lastbuttonX=1;}//Lo acabamos de pulsar

//Boton Y
if (x360Pad1.buttonY==1 && x360Pad1.lastbuttonY==1){Esp_code_send(ESP_JOY1Y,1);} //si esta pulsado
else if (x360Pad1.buttonY==0 && x360Pad1.lastbuttonY==1){Esp_code_send(ESP_JOY1Y,0);x360Pad1.lastbuttonY=0;}//si hemos soltado
if (x360Pad1.buttonY==1 && x360Pad1.lastbuttonY==0){Esp_code_send(ESP_JOY1Y,1);x360Pad1.lastbuttonY=1;}//Lo acabamos de pulsar



//Select
if (x360Pad1.back==1 && x360Pad1.lastback==1){Esp_code_send(ESP_JOY1MODE,1);} //si esta pulsado
else if (x360Pad1.back==0 && x360Pad1.lastback==1){Esp_code_send(ESP_JOY1MODE,0);x360Pad1.lastback=0;}//si hemos soltado
if (x360Pad1.back==1 && x360Pad1.lastback==0){Esp_code_send(ESP_JOY1MODE,1);x360Pad1.lastback=1;}//Lo acabamos de pulsar

//Start
if (x360Pad1.start==1 && x360Pad1.laststart==1){Esp_code_send(ESP_JOY1START,1);} //si esta pulsado
else if (x360Pad1.start==0 && x360Pad1.laststart==1){Esp_code_send(ESP_JOY1START,0);x360Pad1.laststart=0;}//si hemos soltado
if (x360Pad1.start==1 && x360Pad1.laststart==0){Esp_code_send(ESP_JOY1START,1);x360Pad1.laststart=1;}//Lo acabamos de pulsar

//LShoulder
if (x360Pad1.Lshoulder==1 && x360Pad1.lastLshoulder==1){Esp_code_send(ESP_JOY1C,1);} //si esta pulsado
else if (x360Pad1.Lshoulder==0 && x360Pad1.lastLshoulder==1){Esp_code_send(ESP_JOY1C,0);x360Pad1.lastLshoulder=0;}//si hemos soltado
if (x360Pad1.Lshoulder==1 && x360Pad1.lastLshoulder==0){Esp_code_send(ESP_JOY1C,1);x360Pad1.lastLshoulder=1;}//Lo acabamos de pulsar

//RShoulder
if (x360Pad1.Rshoulder==1 && x360Pad1.lastRshoulder==1){Esp_code_send(ESP_JOY1Z,1);} //si esta pulsado
else if (x360Pad1.Rshoulder==0 && x360Pad1.lastRshoulder==1){Esp_code_send(ESP_JOY1Z,0);x360Pad1.lastRshoulder=0;}//si hemos soltado
if (x360Pad1.Rshoulder==1 && x360Pad1.lastRshoulder==0){Esp_code_send(ESP_JOY1Z,1);x360Pad1.lastRshoulder=1;}//Lo acabamos de pulsar

}

void x360Process2(int16_t myreport[])

 { 
       return;

/*
          readx360pad2(&x360Pad2,myreport);
     


//arriba

if (x360Pad2.d_up==1 && x360Pad2.lastd_up==1){Esp_code_send(ESP_JOY2UP,1);} //si esta pulsado
else if (x360Pad2.d_up==0 && x360Pad2.lastd_up==1){Esp_code_send(ESP_JOY2UP,0);x360Pad2.lastd_up=0;}//si hemos soltado
if (x360Pad2.d_up==1 && x360Pad2.lastd_up==0){Esp_code_send(ESP_JOY2UP,1);x360Pad2.lastd_up=1;}//Lo acabamos de pulsar

       //Abajo
if (x360Pad2.d_down==1 && x360Pad2.lastd_down==1){Esp_code_send(ESP_JOY2DOWN,1);} //si esta pulsado
else if (x360Pad2.d_down==0 && x360Pad2.lastd_down==1){Esp_code_send(ESP_JOY2DOWN,0);x360Pad2.lastd_down=0;}//si hemos soltado
if (x360Pad2.d_down==1 && x360Pad2.lastd_down==0){Esp_code_send(ESP_JOY2DOWN,1);x360Pad2.lastd_down=1;}//Lo acabamos de pulsar

//izquierda
if (x360Pad2.d_left==1 && x360Pad2.lastd_left==1){Esp_code_send(ESP_JOY2LEFT,1);} //si esta pulsado
else if (x360Pad2.d_left==0 && x360Pad2.lastd_left==1){Esp_code_send(ESP_JOY2LEFT,0);x360Pad2.lastd_left=0;}//si hemos soltado
if (x360Pad2.d_left==1 && x360Pad2.lastd_left==0){Esp_code_send(ESP_JOY2LEFT,1);x360Pad2.lastd_left=1;}//Lo acabamos de pulsar

//derecha
if (x360Pad2.d_right==1 && x360Pad2.lastd_right==1){Esp_code_send(ESP_JOY2RIGHT,1);} //si esta pulsado
else if (x360Pad2.d_right==0 && x360Pad2.lastd_right==1){Esp_code_send(ESP_JOY2RIGHT,0);x360Pad2.lastd_right=0;}//si hemos soltado
if (x360Pad2.d_right==1 && x360Pad2.lastd_right==0){Esp_code_send(ESP_JOY2RIGHT,1);x360Pad2.lastd_right=1;}//Lo acabamos de pulsar

//Boton A
if (x360Pad2.buttonA==1 && x360Pad2.lastbuttonA==1){Esp_code_send(ESP_JOY2A,1);} //si esta pulsado
else if (x360Pad2.buttonA==0 && x360Pad2.lastbuttonA==1){Esp_code_send(ESP_JOY2A,0);x360Pad2.lastbuttonA=0;}//si hemos soltado
if (x360Pad2.buttonA==1 && x360Pad2.lastbuttonA==0){Esp_code_send(ESP_JOY2A,1);x360Pad2.lastbuttonA=1;}//Lo acabamos de pulsar

//Boton B
if (x360Pad2.buttonB==1 && x360Pad2.lastbuttonB==1){Esp_code_send(ESP_JOY2B,1);} //si esta pulsado
else if (x360Pad2.buttonB==0 && x360Pad2.lastbuttonB==1){Esp_code_send(ESP_JOY2B,0);x360Pad2.lastbuttonB=0;}//si hemos soltado
if (x360Pad2.buttonB==1 && x360Pad2.lastbuttonB==0){Esp_code_send(ESP_JOY2B,1);x360Pad2.lastbuttonB=1;}//Lo acabamos de pulsar

//Boton X
if (x360Pad2.buttonX==1 && x360Pad2.lastbuttonX==1){Esp_code_send(ESP_JOY2X,1);} //si esta pulsado
else if (x360Pad2.buttonX==0 && x360Pad2.lastbuttonX==1){Esp_code_send(ESP_JOY2X,0);x360Pad2.lastbuttonX=0;}//si hemos soltado
if (x360Pad2.buttonX==1 && x360Pad2.lastbuttonX==0){Esp_code_send(ESP_JOY2X,1);x360Pad2.lastbuttonX=1;}//Lo acabamos de pulsar

//Boton Y
if (x360Pad2.buttonY==1 && x360Pad2.lastbuttonY==1){Esp_code_send(ESP_JOY2Y,1);} //si esta pulsado
else if (x360Pad2.buttonY==0 && x360Pad2.lastbuttonY==1){Esp_code_send(ESP_JOY2Y,0);x360Pad2.lastbuttonY=0;}//si hemos soltado
if (x360Pad2.buttonY==1 && x360Pad2.lastbuttonY==0){Esp_code_send(ESP_JOY2Y,1);x360Pad2.lastbuttonY=1;}//Lo acabamos de pulsar



//Select
if (x360Pad2.back==1 && x360Pad2.lastback==1){Esp_code_send(ESP_JOY2MODE,1);} //si esta pulsado
else if (x360Pad2.back==0 && x360Pad2.lastback==1){Esp_code_send(ESP_JOY2MODE,0);x360Pad2.lastback=0;}//si hemos soltado
if (x360Pad2.back==1 && x360Pad2.lastback==0){Esp_code_send(ESP_JOY2MODE,1);x360Pad2.lastback=1;}//Lo acabamos de pulsar

//Start
if (x360Pad2.start==1 && x360Pad2.laststart==1){Esp_code_send(ESP_JOY2START,1);} //si esta pulsado
else if (x360Pad2.start==0 && x360Pad2.laststart==1){Esp_code_send(ESP_JOY2START,0);x360Pad2.laststart=0;}//si hemos soltado
if (x360Pad2.start==1 && x360Pad2.laststart==0){Esp_code_send(ESP_JOY2START,1);x360Pad2.laststart=1;}//Lo acabamos de pulsar

//LTrigger
if (x360Pad2.Ltrigger==1 && x360Pad2.lastLtrigger==1){Esp_code_send(ESP_JOY2C,1);} //si esta pulsado
else if (x360Pad2.Ltrigger==0 && x360Pad2.lastLtrigger==1){Esp_code_send(ESP_JOY2C,0);x360Pad2.lastLtrigger=0;}//si hemos soltado
if (x360Pad2.Ltrigger==1 && x360Pad2.lastLtrigger==0){Esp_code_send(ESP_JOY2C,1);x360Pad2.lastLtrigger=1;}//Lo acabamos de pulsar

//RTrigger
if (x360Pad2.Rtrigger==1 && x360Pad2.lastRtrigger==1){Esp_code_send(ESP_JOY2Z,1);} //si esta pulsado
else if (x360Pad2.Rtrigger==0 && x360Pad2.lastRtrigger==1){Esp_code_send(ESP_JOY2Z,0);x360Pad2.lastRtrigger=0;}//si hemos soltado
if (x360Pad2.Rtrigger==1 && x360Pad2.lastRtrigger==0){Esp_code_send(ESP_JOY2Z,1);x360Pad2.lastRtrigger=1;}//Lo acabamos de pulsar
*/
}
