#include "std_types.h"
#include "DIO-DRIVER/dio_int.h"
#include "MOTOR-HANDLER/motor_cfg.h"
#include "MOTOR-HANDLER/motor_int.h"

typedef struct
{
	uint8_t port;
	uint8_t pin;
} MOTOR_PIN;

// Motor pin configuration
static const MOTOR_PIN motor_pins[2] = {
	{MOTOR_LEFT_PORT, MOTOR_LEFT_PIN},	// Left control pin (e.g., IN1)
	{MOTOR_RIGHT_PORT, MOTOR_RIGHT_PIN} // Right control pin (e.g., IN2)
};

/*
 * Initialize motor pins
 */
uint8_t MOTOR_u8MotorInit(void)
{
	uint8_t status;

	// Set left pin as output
	status = DIO_u8SetPinMode(motor_pins[0].port, motor_pins[0].pin, OUTPUT);
	if (status != E_OK)
		return E_NOK;

	// Set right pin as output
	status = DIO_u8SetPinMode(motor_pins[1].port, motor_pins[1].pin, OUTPUT);
	if (status != E_OK)
		return E_NOK;

	// Initialize pins to LOW (motor off)
	status = DIO_u8SetPinValue(motor_pins[0].port, motor_pins[0].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	status = DIO_u8SetPinValue(motor_pins[1].port, motor_pins[1].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	return E_OK;
}

/*
 * Rotate motor to the right
 */
uint8_t MOTOR_u8RightRotate(void)
{
	uint8_t status;

	// Left pin HIGH, right pin LOW (e.g., IN1=1, IN2=0)
	status = DIO_u8SetPinValue(motor_pins[0].port, motor_pins[0].pin, HIGH);
	if (status != E_OK)
		return E_NOK;

	status = DIO_u8SetPinValue(motor_pins[1].port, motor_pins[1].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	return E_OK;
}

/*
 * Rotate motor to the left
 */
uint8_t MOTOR_u8LeftRotate(void)
{
	uint8_t status;

	// Left pin LOW, right pin HIGH (e.g., IN1=0, IN2=1)
	status = DIO_u8SetPinValue(motor_pins[0].port, motor_pins[0].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	status = DIO_u8SetPinValue(motor_pins[1].port, motor_pins[1].pin, HIGH);
	if (status != E_OK)
		return E_NOK;

	return E_OK;
}

/*
 * Turn off the motor
 */
uint8_t MOTOR_u8MotorOff(void)
{
	uint8_t status;

	// Both pins LOW (e.g., IN1=0, IN2=0)
	status = DIO_u8SetPinValue(motor_pins[0].port, motor_pins[0].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	status = DIO_u8SetPinValue(motor_pins[1].port, motor_pins[1].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	return E_OK;
}