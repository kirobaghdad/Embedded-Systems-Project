#include "std_types.h"
#include "CPU_Configuration.h"
#include "DIO-DRIVER/dio_register.h"
#include "DIO-DRIVER/dio_int.h"

#define IS_VALID_PORT(port_id) ((port_id) <= PORT_D)
#define IS_VALID_PIN(pin_no) ((pin_no) <= PIN_7)

uint8_t DIO_u8SetPinMode(uint8_t port_id, uint8_t pin_no, uint8_t mode)
{
	if (!IS_VALID_PORT(port_id) || !IS_VALID_PIN(pin_no) || (mode != INPUT && mode != OUTPUT))
		return E_NOK;

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
	return E_OK;
}

/*
 * Sets the value (HIGH or LOW) of a specific pin
 */
uint8_t DIO_u8SetPinValue(uint8_t port_id, uint8_t pin_no, uint8_t value)
{
	if (!IS_VALID_PORT(port_id) || !IS_VALID_PIN(pin_no) || (value != HIGH && value != LOW))
		return E_NOK;

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
	return E_OK;
}

/*
 * Gets the value (HIGH or LOW) of a specific pin
 */
uint8_t DIO_u8GetPinValue(uint8_t port_id, uint8_t pin_no, uint8_t *value)
{
	if (!IS_VALID_PORT(port_id) || !IS_VALID_PIN(pin_no) || value == NULL_PTR)
		return E_NOK;

	switch (port_id)
	{
	case PORT_B:
		*value = GET_BIT(PINB_REG, pin_no);
		break;
	case PORT_C:
		*value = GET_BIT(PINC_REG, pin_no);
		break;
	case PORT_D:
		*value = GET_BIT(PIND_REG, pin_no);
		break;
	}
	return E_OK;
}

/*
 * Checks if a specific pin is configured as OUTPUT (returns 1) or INPUT (returns 0)
 */
uint8_t DIO_u8IsPinOutput(uint8_t port_id, uint8_t pin_no, uint8_t *value)
{
	if (!IS_VALID_PORT(port_id) || !IS_VALID_PIN(pin_no) || value == NULL_PTR)
		return E_NOK;

	switch (port_id)
	{
	case PORT_B:
		*value = GET_BIT(DDRB_REG, pin_no);
		break;
	case PORT_C:
		*value = GET_BIT(DDRC_REG, pin_no);
		break;
	case PORT_D:
		*value = GET_BIT(DDRD_REG, pin_no);
		break;
	}
	return E_OK;
}

/*
 * Sets the pull-up mode (ENABLED or DISABLED) for a specific pin
 * Ensures the pin is in INPUT mode before enabling pull-up
 */
uint8_t DIO_u8SetPullUpMode(uint8_t port_id, uint8_t pin_no, uint8_t mode)
{
	if (!IS_VALID_PORT(port_id) || !IS_VALID_PIN(pin_no) || (mode != PULLUP_ENABLED && mode != PULLUP_DISABLED))
		return E_NOK;

	// Ensure pin is in INPUT mode
	uint8_t error_status = DIO_u8SetPinMode(port_id, pin_no, INPUT);
	if (error_status != E_OK)
		return E_NOK;

	// Set pull-up mode
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
	return E_OK;
}

/*
 * Sets the mode (INPUT or OUTPUT) of an entire port
 */
uint8_t DIO_u8SetPortMode(uint8_t port_id, uint8_t mode)
{
	if (!IS_VALID_PORT(port_id))
		return E_NOK;

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
	return E_OK;
}

/*
 * Sets the value of an entire port
 */
uint8_t DIO_u8SetPortValue(uint8_t port_id, uint8_t value)
{
	if (!IS_VALID_PORT(port_id))
		return E_NOK;

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
	return E_OK;
}

/*
 * Gets the value of an entire port
 */
uint8_t DIO_u8GetPortValue(uint8_t port_id, uint8_t *value)
{
	if (!IS_VALID_PORT(port_id) || value == NULL_PTR)
		return E_NOK;

	switch (port_id)
	{
	case PORT_B:
		*value = PINB_REG;
		break;
	case PORT_C:
		*value = PINC_REG;
		break;
	case PORT_D:
		*value = PIND_REG;
		break;
	}
	return E_OK;
}

/*
 * Sets the pull-up mode (ENABLED or DISABLED) for an entire port
 * Ensures the port is in INPUT mode before enabling pull-ups
 */
uint8_t DIO_u8SetPortPullUpMode(uint8_t port_id, uint8_t mode)
{
	if (!IS_VALID_PORT(port_id))
		return E_NOK;

	// Ensure port is in INPUT mode
	uint8_t error_status = DIO_u8SetPortMode(port_id, 0x00); // All pins INPUT
	if (error_status != E_OK)
		return E_NOK;

	// Set pull-up mode
	switch (port_id)
	{
	case PORT_B:
		PORTB_REG = (mode == PULLUP_ENABLED) ? 0xFF : 0x00;
		break;
	case PORT_C:
		PORTC_REG = (mode == PULLUP_ENABLED) ? 0xFF : 0x00;
		break;
	case PORT_D:
		PORTD_REG = (mode == PULLUP_ENABLED) ? 0xFF : 0x00;
		break;
	}
	return E_OK;
}