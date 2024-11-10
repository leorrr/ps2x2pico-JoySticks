#include "tusb.h"
#include "ps2out.h"
#include "ps2in.h"
#include <stdlib.h>
#include <stdio.h>
#include "scancodesets.h"


void gamePad_usb_receive(u8 const* report) {
//printf("none report:      %02x ,%02x ,%02x ,%02x ,%02x ,%02x ,%02x ,%02x  \r",report[0],report[1],report[2],report[3],report[4],report[5],report[6],report[7]);



u8 is_modifiers=0; //por defecto a 0 a nosotros nos va bien eso se usa cuando hay el sift o el control apretados creo

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