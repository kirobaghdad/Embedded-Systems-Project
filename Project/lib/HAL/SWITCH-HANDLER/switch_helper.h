#ifndef HAL_SWITCH_DRIVER_SWITCH_HELPER_H_
#define HAL_SWITCH_DRIVER_SWITCH_HELPER_H_
#include "TYPES.h"
#include "DIO-DRIVER/dio_int.h"
uint8_t SWITCH_u8InitializeSwitch(uint8_t port_id, uint8_t switch_pin);
uint8_t SWITCH_u8InitializeSwitchPort(uint8_t port_id);

#endif
