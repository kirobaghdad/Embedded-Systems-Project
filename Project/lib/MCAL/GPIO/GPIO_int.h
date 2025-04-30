#ifndef GPIO_INT_H_
#define GPIO_INT_H_
#include "std_types.h"

#define PORT_B (uint8_t)0
#define PORT_C (uint8_t)1
#define PORT_D (uint8_t)2
#define PIN_0 (uint8_t)0
#define PIN_1 (uint8_t)1
#define PIN_2 (uint8_t)2
#define PIN_3 (uint8_t)3
#define PIN_4 (uint8_t)4
#define PIN_5 (uint8_t)5
#define PIN_6 (uint8_t)6
#define PIN_7 (uint8_t)7
#define OUTPUT (uint8_t)1
#define INPUT (uint8_t)0
#define HIGH (uint8_t)1
#define LOW (uint8_t)0
#define PULLUP_ENABLED (uint8_t)1
#define PULLUP_DISABLED (uint8_t)0

void GPIO_SetPinMode(uint8_t port_id, uint8_t pin_no, uint8_t mode);
void GPIO_SetPinValue(uint8_t port_id, uint8_t pin_no, uint8_t value);
uint8_t GPIO_GetPinValue(uint8_t port_id, uint8_t pin_no);
void GPIO_SetPullUpMode(uint8_t port_id, uint8_t pin_no, uint8_t mode);
void GPIO_SetPortMode(uint8_t port_id, uint8_t mode);
void GPIO_SetPortValue(uint8_t port_id, uint8_t value);
uint8_t GPIO_GetPortValue(uint8_t port_id);

#endif