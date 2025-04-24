#include "BLUETOOTH-HANDLER/bluetooth_int.h"
#include "BLUETOOTH-HANDLER/bluetooth_cfg.h"

void BLUETOOTH_init()
{
    DIO_u8SetPinMode(Bluetooth_portNumber, Bluetooth_pinNumber, INPUT);
    USART_Init(USART_DEFAULT_BAUD_RATE, USART_CHAR_SIZE_8, USART_PARITY_NONE, USART_STOP_1);
}

void BLUETOOTH_sendChar(uint8_t character)
{
    DIO_u8SetPinMode(Bluetooth_portNumber, Bluetooth_pinNumber, INPUT);
    USART_Transmit(character);
}

void BLUETOOTH_sendString(uint8_t *message)
{
    DIO_u8SetPinMode(Bluetooth_portNumber, Bluetooth_pinNumber, INPUT);
    USART_TransmitString(message, 0);
}

uint8_t BLUETOOTH_receiveChar(void)
{
    DIO_u8SetPinMode(Bluetooth_portNumber, Bluetooth_pinNumber, OUTPUT);
    return USART_Receive();
}