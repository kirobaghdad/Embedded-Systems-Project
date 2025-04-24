#include "TYPES.h"
#ifndef HAL_LED_DRIVER_LED_INT_H_
#define HAL_LED_DRIVER_LED_INT_H_
#include "DIO-DRIVER/dio_int.h"



uint8_t LED_u8LedOn(uint8_t port_id,uint8_t led_pin);
uint8_t LED_u8LedOff(uint8_t port_id,uint8_t led_pin);
uint8_t LED_u8LedToggle(uint8_t port_id,uint8_t led_pin);
uint8_t LED_u8LedBlink(uint8_t port_id,uint8_t led_pin,uint16_t delay_time_ms);

uint8_t LED_u8LedPortOn(uint8_t port_id);
uint8_t LED_u8LedPortOff(uint8_t port_id);
uint8_t LED_u8LedPortToggle(uint8_t port_id);
uint8_t LED_u8LedPortBlink(uint8_t port_id,uint16_t delay_time_ms);

#endif
