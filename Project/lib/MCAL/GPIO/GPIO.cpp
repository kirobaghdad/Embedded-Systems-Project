#include <avr/io.h>
#include "std_types.h"
#include "CPU_Configuration.h"
#include "GPIO/GPIO_int.h"

void GPIO_SetPinMode(uint8_t port_id, uint8_t pin_no, uint8_t mode)
{
    switch (port_id)
    {
    case PORT_B:
        mode == OUTPUT ? SET_BIT(DDRB, pin_no) : CLR_BIT(DDRB, pin_no);
        break;
    case PORT_C:
        mode == OUTPUT ? SET_BIT(DDRC, pin_no) : CLR_BIT(DDRC, pin_no);
        break;
    case PORT_D:
        mode == OUTPUT ? SET_BIT(DDRD, pin_no) : CLR_BIT(DDRD, pin_no);
        break;
    }
}
void GPIO_SetPinValue(uint8_t port_id, uint8_t pin_no, uint8_t value)
{
    switch (port_id)
    {
    case PORT_B:
        value == HIGH ? SET_BIT(PORTB, pin_no) : CLR_BIT(PORTB, pin_no);
        break;
    case PORT_C:
        value == HIGH ? SET_BIT(PORTC, pin_no) : CLR_BIT(PORTC, pin_no);
        break;
    case PORT_D:
        value == HIGH ? SET_BIT(PORTD, pin_no) : CLR_BIT(PORTD, pin_no);
        break;
    }
}
uint8_t GPIO_GetPinValue(uint8_t port_id, uint8_t pin_no)
{
    switch (port_id)
    {
    case PORT_B:
        return GET_BIT(PINB, pin_no);
        break;
    case PORT_C:
        return GET_BIT(PINC, pin_no);
        break;
    case PORT_D:
        return GET_BIT(PIND, pin_no);
        break;
    }
}
void GPIO_SetPullUpMode(uint8_t port_id, uint8_t pin_no, uint8_t mode)
{
    GPIO_SetPinMode(port_id, pin_no, INPUT);
    switch (port_id)
    {
    case PORT_B:
        mode == PULLUP_ENABLED ? SET_BIT(PORTB, pin_no) : CLR_BIT(PORTB, pin_no);
        break;
    case PORT_C:
        mode == PULLUP_ENABLED ? SET_BIT(PORTC, pin_no) : CLR_BIT(PORTC, pin_no);
        break;
    case PORT_D:
        mode == PULLUP_ENABLED ? SET_BIT(PORTD, pin_no) : CLR_BIT(PORTD, pin_no);
        break;
    }
}
void GPIO_SetPortMode(uint8_t port_id, uint8_t mode)
{
    switch (port_id)
    {
    case PORT_B:
        DDRB = mode;
        break;
    case PORT_C:
        DDRC = mode;
        break;
    case PORT_D:
        DDRD = mode;
        break;
    }
}
void GPIO_SetPortValue(uint8_t port_id, uint8_t value)
{

    switch (port_id)
    {
    case PORT_B:
        PORTB = value;
        break;
    case PORT_C:
        PORTC = value;
        break;
    case PORT_D:
        PORTD = value;
        break;
    }
}
uint8_t GPIO_GetPortValue(uint8_t port_id)
{
    switch (port_id)
    {
    case PORT_B:
        return PINB;
        break;
    case PORT_C:
        return PINC;
        break;
    case PORT_D:
        return PIND;
        break;
    }
}
