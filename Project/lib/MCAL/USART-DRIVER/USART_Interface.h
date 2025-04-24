#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_

#include "std_types.h"
#include "USART-DRIVER/USART_Configuration.h"

// Initialize USART with specified baud rate, character size, parity, and stop bits
uint8_t USART_Init(uint32_t baud_rate, USART_CharSize_t char_size, USART_Parity_t parity, USART_StopBits_t stop_bits);

// Transmit a single byte
uint8_t USART_Transmit(uint8_t data);

// Receive a single byte
uint8_t USART_Receive();

// Transmit a null-terminated string
uint8_t USART_TransmitString(uint8_t *str, uint8_t send_null);

#endif /* MCAL_USART_DRIVER_USART_INT_H_ */