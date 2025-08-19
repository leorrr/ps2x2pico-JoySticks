//#include "ps2x2pico.h"
//Este codigo inicializa el joystick db 9 y ejecuta la rutina de lectura de Joystick y mapea su correspondiente codigo
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
#define joy2Start 13  // Para joyStick SEGA

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

// inicilializa gpios
void db9Init();

// Función para leer el estado del joystick 
void read_joystick1(JoystickState *state);
void read_joystick2(JoystickState *state);


//Procesa Lectura de Joysticks
void db9Process();
