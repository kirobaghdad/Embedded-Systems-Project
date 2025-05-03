#include "BLUETOOTH-HANDLER/bluetooth_int.h"
#include "BLUETOOTH-HANDLER/bluetooth_cfg.h"
#include "GPIO/GPIO_int.h"
#include "USART-DRIVER/USART_int.h"
#define bluetooth_portNumber PORT_D
#
void BLUETOOTH_init()
{
    GPIO_SetPinMode(Bluetooth_portNumber, Bluetooth_pinNumber, INPUT);
    USART_Init(USART_BAUD_9600, USART_CHAR_SIZE_8, USART_PARITY_NONE, USART_STOP_1);
}

void BLUETOOTH_sendChar(uint8_t character)
{
    GPIO_SetPinMode(Bluetooth_portNumber, Bluetooth_pinNumber, INPUT);
    USART_Transmit(character);
}

void BLUETOOTH_sendString(uint8_t *message)
{
    GPIO_SetPinMode(Bluetooth_portNumber, Bluetooth_pinNumber, INPUT);
    USART_TransmitString(message);
}

uint8_t BLUETOOTH_receiveChar(void)
{
    GPIO_SetPinMode(Bluetooth_portNumber, Bluetooth_pinNumber, OUTPUT);
    return USART_Receive();
}