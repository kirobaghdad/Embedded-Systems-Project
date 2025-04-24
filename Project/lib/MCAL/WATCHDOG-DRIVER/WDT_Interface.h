#ifndef WDT_INTERFACE_H_
#define WDT_INTERFACE_H_

#include "std_types.h"
#include "WATCHDOG-DRIVER/WDT_Config.h"

// Initialize the Watchdog Timer with a specified timeout
uint8_t WDT_Init(WDT_Timeout_t timeout);

// Reset ("kick") the Watchdog Timer to prevent a system reset
uint8_t WDT_Reset(void);

// Disable the Watchdog Timer
uint8_t WDT_Disable(void);

#endif /* MCAL_WDT_DRIVER_WDT_INT_H_ */