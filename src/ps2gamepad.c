#include "tusb.h"
#include "ps2out.h"
#include "ps2in.h"
#include <stdlib.h>
#include <stdio.h>
#include "scancodesets.h"
// El estado de los botones es normalmente 1, y cuando se pulsan 0, porque estan activados los pullUp internos
// creamos una variable por cada boton para guardar el ultimo estado actual del boton al ser pulsado o liberado

bool joy1Up_state=0;
bool joy1Down_state=0;
bool joy1Left_state=0;
bool joy1Right_state=0;
bool joy1Fire_state=0;
bool joy1Select_state=0;
bool joy1Start_state=0;

bool joy2Up_state=0;
bool joy2Down_state=0;
bool joy2Left_state=0;
bool joy2Right_state=0;
bool joy2Fire_state=0;
bool joy2Select_state=0;
bool joy2Start_state=0;


void gamePad_usb_receive(u8 const* report) {
//printf("none report:      %02x ,%02x ,%02x ,%02x ,%02x ,%02x ,%02x ,%02x  \r",report[0],report[1],report[2],report[3],report[4],report[5],report[6],report[7]);
/*u8 is_modifiers=0; //por defecto a 0 a nosotros nos va bien eso se usa cuando hay el sift o el control apretados creo
if (report[5]> 15){
u8 key_TAB=0x2b; //Disparo
//kb_send_key_scs2(key, is_key_pressed, is_ctrl);
do{kb_send_key(key_TAB, 1, is_modifiers);sleep_ms(25);}
while(report[5]== 15);
  kb_send_key(key_TAB, 0, is_modifiers);
}


if (report[3]==0){
u8 key_izq=0x50; //izquierda
do{kb_send_key(key_izq, 1, is_modifiers);sleep_ms(25);}
while (report[3]==127);
  kb_send_key(key_izq, 0, is_modifiers);
}

if (report[3]==255){
u8 key_der=0x4f; //Derecha
do{kb_send_key(key_der, 1, is_modifiers);sleep_ms(25);}
while (report[3]==127);  
  kb_send_key(key_der, 0, is_modifiers);
}


if (report[4]==0){
u8 key_arriba=0x52; //Arriba
do{kb_send_key(key_arriba, true, is_modifiers);sleep_ms(25);}
while (report[4]==127);
  kb_send_key(key_arriba, false, is_modifiers);
}

if (report[4]==255){
u8 key_abajo=0x51; // abajo
do{kb_send_key(key_abajo, 1, is_modifiers);sleep_ms(25);}
while (report[4]==127);
  kb_send_key(key_abajo, 0, is_modifiers);
}
 //printf("none report:      %d,%d,%d,%d,%d,%d,%d,       \r",report[0],report[1],report[2],report[3],report[4],report[5],report[6]);
*/
//disparo
       if (report[5]>15 && joy1Fire_state==1){kb_send_key(0x2b, 1, 0);}          
       else if (report[5]==15 && joy1Fire_state==1){kb_send_key(0x2b, 0, 0);joy1Fire_state=0;}
       if (report[5]>15 && joy1Fire_state==0){kb_send_key(0x2b, 1, 0);joy1Fire_state=1;}
       
      //izquierda
       if (report[3]==0 && joy1Left_state==1){kb_send_key(0x50, 1, 0);} 
       else if (report[3]==127 && joy1Left_state==1){kb_send_key(0x50, 0, 0);joy1Left_state=0;}
       if (report[3]==0 && joy1Left_state==0){kb_send_key(0x50, 1, 0);joy1Left_state=1;}  
       
        //Derecha
       if (report[3]==255 && joy1Right_state==1){kb_send_key(0x4f, 1, 0);} 
       else if (report[3]==127 && joy1Right_state==1){kb_send_key(0x4f, 0, 0);joy1Right_state=0;}
       if (report[3]==255 && joy1Right_state==0){kb_send_key(0x4f, 1, 0);joy1Right_state=1;}  
       

       //arriba
       if (report[4]==0 && joy1Up_state==1){kb_send_key(0x52, 1, 0);} 
       else if (report[4]==127 && joy1Up_state==1){kb_send_key(0x52, 0, 0);joy1Up_state=0;}
       if (report[4]==0 && joy1Up_state==0){kb_send_key(0x52, 1, 0);joy1Up_state=1;}  
        
       
       //abajo
      if (report[4]==255 && joy1Down_state==1){kb_send_key(0x51, 1, 0);} 
      else if (report[4]==127 && joy1Down_state==1){kb_send_key(0x51, 0, 0);joy1Down_state=0;}
      if (report[4]==255 && joy1Down_state==0){kb_send_key(0x51, 1, 0);joy1Down_state=1;}     
     
     sleep_ms(80);  

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