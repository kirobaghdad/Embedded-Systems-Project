#ifndef WDT_CONFIGURATION_H_
#define WDT_CONFIGURATION_H_

#include "CPU_Configuration.h"

// Watchdog Timer Timeout Options (ATmega328P, Vcc=5V)
typedef enum
{
    WDT_TIMEOUT_16MS = 0x00,  // 16 milliseconds
    WDT_TIMEOUT_32MS = 0x01,  // 32 milliseconds
    WDT_TIMEOUT_64MS = 0x02,  // 64 milliseconds
    WDT_TIMEOUT_125MS = 0x03, // 125 milliseconds
    WDT_TIMEOUT_250MS = 0x04, // 250 milliseconds
    WDT_TIMEOUT_500MS = 0x05, // 500 milliseconds
    WDT_TIMEOUT_1S = 0x06,    // 1 second
    WDT_TIMEOUT_2S = 0x07,    // 2 seconds
    WDT_TIMEOUT_4S = 0x20,    // 4 seconds (WDP3=1, WDP2:0=000)
    WDT_TIMEOUT_8S = 0x21     // 8 seconds (WDP3=1, WDP2:0=001)
} WDT_Timeout_t;

// Default Timeout
#define WDT_DEFAULT_TIMEOUT WDT_TIMEOUT_2S

#endif /* MCAL_WDT_DRIVER_WDT_CONFIG_H_ */