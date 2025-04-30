#include "std_types.h"
#include "CPU_Configuration.h"
#include "GPIO/GPIO_address.h"
#include "GPIO/GPIO_int.h"

void GPIO_SetPinMode(uint8_t port_id, uint8_t pin_no, uint8_t mode)
{
    switch (port_id)
    {
    case PORT_B:
        mode == OUTPUT ? SET_BIT(DDRB_REG, pin_no) : CLR_BIT(DDRB_REG, pin_no);
        break;
    case PORT_C:
        mode == OUTPUT ? SET_BIT(DDRC_REG, pin_no) : CLR_BIT(DDRC_REG, pin_no);
        break;
    case PORT_D:
        mode == OUTPUT ? SET_BIT(DDRD_REG, pin_no) : CLR_BIT(DDRD_REG, pin_no);
        break;
    }
}
void GPIO_SetPinValue(uint8_t port_id, uint8_t pin_no, uint8_t value)
{
    switch (port_id)
    {
    case PORT_B:
        value == HIGH ? SET_BIT(PORTB_REG, pin_no) : CLR_BIT(PORTB_REG, pin_no);
        break;
    case PORT_C:
        value == HIGH ? SET_BIT(PORTC_REG, pin_no) : CLR_BIT(PORTC_REG, pin_no);
        break;
    case PORT_D:
        value == HIGH ? SET_BIT(PORTD_REG, pin_no) : CLR_BIT(PORTD_REG, pin_no);
        break;
    }
}
uint8_t GPIO_GetPinValue(uint8_t port_id, uint8_t pin_no)
{
    switch (port_id)
    {
    case PORT_B:
        return GET_BIT(PINB_REG, pin_no);
        break;
    case PORT_C:
        return GET_BIT(PINC_REG, pin_no);
        break;
    case PORT_D:
        return GET_BIT(PIND_REG, pin_no);
        break;
    }
}
void GPIO_SetPullUpMode(uint8_t port_id, uint8_t pin_no, uint8_t mode)
{
    GPIO_SetPinMode(port_id, pin_no, INPUT);
    switch (port_id)
    {
    case PORT_B:
        mode == PULLUP_ENABLED ? SET_BIT(PORTB_REG, pin_no) : CLR_BIT(PORTB_REG, pin_no);
        break;
    case PORT_C:
        mode == PULLUP_ENABLED ? SET_BIT(PORTC_REG, pin_no) : CLR_BIT(PORTC_REG, pin_no);
        break;
    case PORT_D:
        mode == PULLUP_ENABLED ? SET_BIT(PORTD_REG, pin_no) : CLR_BIT(PORTD_REG, pin_no);
        break;
    }
}
void GPIO_SetPortMode(uint8_t port_id, uint8_t mode)
{
    switch (port_id)
    {
    case PORT_B:
        DDRB_REG = mode;
        break;
    case PORT_C:
        DDRC_REG = mode;
        break;
    case PORT_D:
        DDRD_REG = mode;
        break;
    }
}
void GPIO_SetPortValue(uint8_t port_id, uint8_t value)
{

    switch (port_id)
    {
    case PORT_B:
        PORTB_REG = value;
        break;
    case PORT_C:
        PORTC_REG = value;
        break;
    case PORT_D:
        PORTD_REG = value;
        break;
    }
}
uint8_t GPIO_GetPortValue(uint8_t port_id)
{
    switch (port_id)
    {
    case PORT_B:
        return PINB_REG;
        break;
    case PORT_C:
        return PINC_REG;
        break;
    case PORT_D:
        return PIND_REG;
        break;
    }
}
