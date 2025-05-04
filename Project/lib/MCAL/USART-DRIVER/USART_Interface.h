#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_

#include "std_types.h"
#include "USART-DRIVER/USART_Configuration.h"

uint8_t USART_Init(uint32_t baud_rate, USART_CharSize_t char_size, USART_Parity_t parity, USART_StopBits_t stop_bits);

uint8_t USART_Transmit(uint8_t data);

uint8_t USART_Receive();

uint8_t USART_TransmitString(uint8_t *str, uint8_t send_null);

void USART_ReceiveString(uint8_t *str);

#endif