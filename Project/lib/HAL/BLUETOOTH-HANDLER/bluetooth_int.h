#ifndef BLUETOOTH_INT_H_
#define BLUETOOTH_INT_H_

#include "std_types.h"
#include "USART-DRIVER/USART_Interface.h"

// ****** Functions Prototypes ****** //
void BLUETOOTH_init();
void BLUETOOTH_sendChar(uint8_t character);
void BLUETOOTH_sendString(uint8_t *message);
uint8_t BLUETOOTH_receiveChar(void);
#endif