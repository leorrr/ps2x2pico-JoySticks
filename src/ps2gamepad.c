#include "tusb.h"
#include "ps2out.h"
#include "ps2in.h"
#include <stdlib.h>
#include <stdio.h>
#include "scancodesets.h"
#include "espec_to_ps2.c"

// El estado de los botones es normalmente 1, y cuando se pulsan 0, porque estan activados los pullUp internos
// creamos una variable por cada boton para guardar el ultimo estado actual del boton al ser pulsado o liberado

bool joy1Up_state=0;
bool joy1Down_state=0;
bool joy1Left_state=0;
bool joy1Right_state=0;
bool joy1A_state=0;
bool joy1B_state=0;
bool joy1X_state=0;
bool joy1Y_state=0;
bool joy1Select_state=0;
bool joy1Start_state=0;
bool joy1Ltrigger_state=0;
bool joy1Rtrigger_state=0;


bool joy2Up_state=0;
bool joy2Down_state=0;
bool joy2Left_state=0;
bool joy2Right_state=0;
bool joy2A_state=0;
bool joy2B_state=0;
bool joy2X_state=0;
bool joy2Y_state=0;
bool joy2Select_state=0;
bool joy2Start_state=0;
bool joy2Ltrigger_state=0;
bool joy2Rtrigger_state=0;


void gamePad_usb_receive(u8 const* report) {
//printf("none report:      %02x ,%02x ,%02x ,%02x ,%02x ,%02x ,%02x ,%02x  \r",report[0],report[1],report[2],report[3],report[4],report[5],report[6],report[7]);

 //printf("none report:      %d,%d,%d,%d,%d,%d,%d,       \r",report[0],report[1],report[2],report[3],report[4],report[5],report[6]);

//disparo boton B
       if (report[5]==79 && joy1B_state==1){/*kb_send_key(0x2b, 1, 0);*/Esp_code_send(ESP_JOY1B,1);}          
       else if (report[5]==15 && joy1B_state==1){/*kb_send_key(0x2b, 0, 0);*/Esp_code_send(ESP_JOY1B,0);joy1B_state=0;}
       if (report[5]==79 && joy1B_state==0){/*kb_send_key(0x2b, 1, 0);*/Esp_code_send(ESP_JOY1B,1);joy1B_state=1;}
       
//izquierda
       if (report[3]==0 && joy1Left_state==1){/*kb_send_key(0x50, 1, 0);*/Esp_code_send(ESP_JOY1LEFT,1);} 
       else if (report[3]==127 && joy1Left_state==1){/*kb_send_key(0x50, 0, 0);*/Esp_code_send(ESP_JOY1LEFT,0);joy1Left_state=0;}
       if (report[3]==0 && joy1Left_state==0){/*kb_send_key(0x50, 1, 0);*/Esp_code_send(ESP_JOY1LEFT,1);joy1Left_state=1;}  
       
//Derecha
       if (report[3]==255 && joy1Right_state==1){/*kb_send_key(0x4f, 1, 0);*/Esp_code_send(ESP_JOY1RIGHT,1);} 
       else if (report[3]==127 && joy1Right_state==1){/*kb_send_key(0x4f, 0, 0);*/Esp_code_send(ESP_JOY1RIGHT,0);joy1Right_state=0;}
       if (report[3]==255 && joy1Right_state==0){/*kb_send_key(0x4f, 1, 0);*/Esp_code_send(ESP_JOY1RIGHT,1);joy1Right_state=1;}  
       

//arriba
       if (report[4]==0 && joy1Up_state==1){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY1UP,1);} 
       else if (report[4]==127 && joy1Up_state==1){/*kb_send_key(0x52, 0, 0);*/Esp_code_send(ESP_JOY1UP,0);joy1Up_state=0;}
       if (report[4]==0 && joy1Up_state==0){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY1UP,1);joy1Up_state=1;}  
        
       
//abajo
      if (report[4]==255 && joy1Down_state==1){/*kb_send_key(0x51, 1, 0);*/Esp_code_send(ESP_JOY1DOWN,1);} 
      else if (report[4]==127 && joy1Down_state==1){/*kb_send_key(0x51, 0, 0);*/Esp_code_send(ESP_JOY1DOWN,0);joy1Down_state=0;}
      if (report[4]==255 && joy1Down_state==0){/*kb_send_key(0x51, 1, 0);*/Esp_code_send(ESP_JOY1DOWN,1);joy1Down_state=1;}     
     
//boton A
       if (report[5]==47 && joy1A_state==1){/*kb_send_key(0x29, 1, 0);*/Esp_code_send(ESP_JOY1A,1);}          
       else if (report[5]==15 && joy1A_state==1){/*kb_send_key(0x29, 0, 0);*/Esp_code_send(ESP_JOY1A,0);joy1A_state=0;}
       if (report[5]==47 && joy1A_state==0){/*kb_send_key(0x29, 1, 0);*/Esp_code_send(ESP_JOY1A,1);joy1A_state=1;}

//arriba boton X
       if (report[5]==31 && joy1X_state==1){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY1X,1);} 
       else if (report[5]==15 && joy1X_state==1){/*kb_send_key(0x52, 0, 0);*/Esp_code_send(ESP_JOY1X,0);joy1X_state=0;}
       if (report[5]==31 && joy1X_state==0){/*kb_send_key(0x52, 1, 0);*/Esp_code_send(ESP_JOY1X,1);joy1X_state=1;}  

//boton Y
       if (report[5]==143 && joy1Y_state==1){/*kb_send_key(0x29, 1, 0);*/Esp_code_send(ESP_JOY1Y,1);}          
       else if (report[5]==15 && joy1Y_state==1){/*kb_send_key(0x29, 0, 0);*/Esp_code_send(ESP_JOY1Y,0);joy1Y_state=0;}
       if (report[5]==143 && joy1Y_state==0){/*kb_send_key(0x29, 1, 0);*/Esp_code_send(ESP_JOY1Y,1);joy1Y_state=1;}

//boton Select
       if (report[6]==16 && joy1Select_state==1){/*b_send_key(0x3A, 1, 0);*/Esp_code_send(ESP_JOY1MODE,1);} 
       else if (report[6]==0 && joy1Select_state==1){/*kb_send_key(0x3A, 0, 0);*/Esp_code_send(ESP_JOY1MODE,0);joy1Select_state=0;}
       if (report[6]==16 && joy1Select_state==0){/*kb_send_key(0x3A, 1, 0);*/Esp_code_send(ESP_JOY1MODE,1);joy1Select_state=1;} 

//boton Start
       if (report[6]==32 && joy1Start_state==1){/*kb_send_key(0x28, 1, 0);*/Esp_code_send(ESP_JOY1START,1);} 
       else if (report[6]==0 && joy1Start_state==1){/*kb_send_key(0x28, 0, 0);*/Esp_code_send(ESP_JOY1START,0);joy1Start_state=0;}
       if (report[6]==32 && joy1Start_state==0){/*kb_send_key(0x28, 1, 0);*/Esp_code_send(ESP_JOY1START,1);joy1Start_state=1;}        

//boton Trigger L
       if (report[6]==1 && joy1Ltrigger_state==1){/*kb_send_key(0x3B, 1, 0);*/Esp_code_send(ESP_JOY1C,1);} 
       else if (report[6]==0 && joy1Ltrigger_state==1){/*kb_send_key(0x3B, 0, 0);*/Esp_code_send(ESP_JOY1C,0);joy1Ltrigger_state=0;}
       if (report[6]==1 && joy1Ltrigger_state==0){/*kb_send_key(0x3B, 1, 0);*/Esp_code_send(ESP_JOY1C,1);joy1Ltrigger_state=1;} 

 //boton Trigger R
       if (report[6]==2 && joy1Rtrigger_state==1){/*kb_send_key(0x3E, 1, 0);*/Esp_code_send(ESP_JOY1Z,1);} 
       else if (report[6]==0 && joy1Rtrigger_state==1){/*kb_send_key(0x3E, 0, 0);*/Esp_code_send(ESP_JOY1Z,0);joy1Rtrigger_state=0;}
       if (report[6]==2 && joy1Rtrigger_state==0){/*kb_send_key(0x3E, 1, 0);*/Esp_code_send(ESP_JOY1Z,1);joy1Rtrigger_state=1;}       

     //sleep_ms(80);  

}







/*bool gp_task() {
  ps2out_task(&ms_out);
  ps2in_task(&ms_in, &ms_out);
  return ms_streaming && !ms_out.busy;
}*/
/*
/*
bool kb_task() {
  ps2out_task(&kb_out);
  ps2in_task(&kb_in, &kb_out);
  return kb_enabled && !kb_out.busy;// TODO: return value can probably be void
} */