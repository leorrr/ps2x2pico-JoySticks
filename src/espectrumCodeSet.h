// 20250130 Codigos ps2 para utilizar con emulador ESPectrum
/* keycodes por kempston ESPectrum emulator:
    // All press transmissions using these are preceded with 0xE2
    // All release transmissions using these are preceded with 0xE2, 0xF0

      ESP_JOY1LEFT = 0x40,
      ESP_JOY1RIGHT = 0x41,
      ESP_JOY1UP = 0x42,
      ESP_JOY1DOWN = 0x43,
      ESP_JOY1START = 0x44,
      ESP_JOY1MODE = 0x45,
      ESP_JOY1A = 0x46,
      ESP_JOY1B = 0x47,
      ESP_JOY1C = 0x48,
      ESP_JOY1X = 0x49,
      ESP_JOY1Y = 0x4a,
      ESP_JOY1Z = 0x4b,
      ESP_JOY2LEFT = 0x4c,
      ESP_JOY2RIGHT = 0x4d,
      ESP_JOY2UP = 0x4e,
      ESP_JOY2DOWN = 0x4f,
      ESP_JOY2START = 0x50,
      ESP_JOY2MODE = 0x51,
      ESP_JOY2A = 0x52,
      ESP_JOY2B = 0x53,
      ESP_JOY2C = 0x54,
      ESP_JOY2X = 0x55,
      ESP_JOY2Y = 0x56,
      ESP_JOY2Z = 0x57
 */
      
#define ESP_prefix 0xE2
#define ESP_prefix_release 0xf0

#define ESP_JOY1LEFT  0x40
#define ESP_JOY1RIGHT  0x41
#define ESP_JOY1UP  0x42
#define ESP_JOY1DOWN  0x43
#define ESP_JOY1START  0x44
#define ESP_JOY1MODE  0x45
#define ESP_JOY1A  0x46
#define ESP_JOY1B  0x47
#define ESP_JOY1C  0x48
#define ESP_JOY1X  0x49
#define ESP_JOY1Y  0x4a
#define ESP_JOY1Z  0x4b

#define ESP_JOY2LEFT  0x4c
#define ESP_JOY2RIGHT  0x4d
#define ESP_JOY2UP  0x4e
#define ESP_JOY2DOWN  0x4f
#define ESP_JOY2START  0x50
#define ESP_JOY2MODE  0x51
#define ESP_JOY2A  0x52
#define ESP_JOY2B  0x53
#define ESP_JOY2C  0x54
#define ESP_JOY2X  0x55
#define ESP_JOY2Y  0x56
#define ESP_JOY2Z  0x57