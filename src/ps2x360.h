#include "ps2x2pico.h"
//XINPUT defines and struct format from
//https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
#define XINPUT_GAMEPAD_DPAD_UP 0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN 0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT 0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT 0x0008
#define XINPUT_GAMEPAD_START 0x0010
#define XINPUT_GAMEPAD_BACK 0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB 0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB 0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER 0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER 0x0200
#define XINPUT_GAMEPAD_GUIDE 0x0400
#define XINPUT_GAMEPAD_SHARE 0x0800
#define XINPUT_GAMEPAD_A 0x1000
#define XINPUT_GAMEPAD_B 0x2000
#define XINPUT_GAMEPAD_X 0x4000
#define XINPUT_GAMEPAD_Y 0x8000
#define MAX_PACKET_SIZE 32

typedef struct {
       bool d_up;
       bool d_down;
       bool d_left;
       bool d_right;
       bool buttonA;
       bool buttonB;
       bool buttonX;
       bool buttonY;
       bool back;
       bool start;
       bool guide;
       bool Ltrigger;
       bool Rtrigger;
       bool Lshoulder;
       bool Rshoulder;
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
       bool lastbuttonY;
       bool lastback;
       bool laststart;
       bool lastguide;
       bool lastLtrigger;
       bool lastRtrigger;
       bool lastLshoulder;
       bool lastRshoulder;
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

    void readx360pad1(x360PadState *state,u16 const* report);
    void readx360pad2(x360PadState *state,u16 const* report);
    void x360Process1(int16_t myreport[]);
    void x360Process2(int16_t myreport[]);

  