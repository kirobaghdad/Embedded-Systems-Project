#include "std_types.h"
#ifndef HAL_SWITCH_DRIVER_SWITCH_INT_H_
#define HAL_SWITCH_DRIVER_SWITCH_INT_H_
#include "DIO-DRIVER/dio_int.h"

/*
 * macros to be used with value param
 */
#define PRESSED (uint8_t)1
#define NOTPRESSED (uint8_t)0

/*
 * macros to be used in pullup mode param
 */
#define Enabled (uint8_t)1
#define Disabled  (uint8_t)0

uint8_t SWITCH_u8GetSwitch(uint8_t port_id,uint8_t switch_pin,uint8_t pull_up_enable,uint8_t * value);

uint8_t SWITCH_u8GetSwitchPort(uint8_t port_id,uint8_t pull_up_enable,uint8_t * value);

#endif
