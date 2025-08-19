#include "ps2x2pico.h"
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

void readGamepad1(gamepadState *state,u8 const* report);
void readGamepad2(gamepadState *state,u8 const* report);
void Gamepad1Process(u8 const* report);
void Gamepad2Process(u8 const* report);