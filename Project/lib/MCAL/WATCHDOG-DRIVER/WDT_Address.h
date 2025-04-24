#ifndef WDT_ADDRESS_H_
#define WDT_ADDRESS_H_

#include "std_types.h"

// Watchdog Timer Control and Status Register
#define WDTCSR (*((volatile uint8_t *)0x60)) // WDTCSR register address for ATmega328P

// Bit positions in WDTCSR
#define WDP0 0 // Watchdog Prescaler bit 0
#define WDP1 1 // Watchdog Prescaler bit 1
#define WDP2 2 // Watchdog Prescaler bit 2
#define WDE 3  // Watchdog Enable
#define WDCE 4 // Watchdog Change Enable
#define WDP3 5 // Watchdog Prescaler bit 3
#define WDIE 6 // Watchdog Interrupt Enable
#define WDIF 7 // Watchdog Interrupt Flag

#endif /* MCAL_WDT_DRIVER_WDT_REGISTER_H_ */