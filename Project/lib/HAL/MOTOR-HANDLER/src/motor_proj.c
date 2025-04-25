#include "std_types.h"
#include "DIO-DRIVER/dio_int.h"
#include "MOTOR-HANDLER/motor_cfg.h"
#include "MOTOR-HANDLER/motor_int.h"
#include "TIMERS-DRIVER/TIMER2/TMR2_Interface.h"


// Motor pin configuration
/* static const MOTOR_PIN motor_pins[3] = {
	{MOTOR_LEFT_PORT, MOTOR_LEFT_PIN},	  // Left control pin (e.g., IN1)
	{MOTOR_RIGHT_PORT, MOTOR_RIGHT_PIN},  // Right control pin (e.g., IN2)
	{MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN} // Enable pin (e.g., ENA)
}; */

/*
 * Initialize motor pins
 */
uint8_t MOTOR_u8MotorInit(MOTOR_PIN *motor_pins)
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

	// Set enable pin as output
	status = DIO_u8SetPinMode(motor_pins[2].port, motor_pins[2].pin, OUTPUT);
	if (status != E_OK)
		return E_NOK;

	// Initialize pins to LOW (motor off)
	status = DIO_u8SetPinValue(motor_pins[0].port, motor_pins[0].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	status = DIO_u8SetPinValue(motor_pins[1].port, motor_pins[1].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	// Initialize Timer2 for PWM
	status = TMR2_InitPWM();
	if (status != E_OK)
		return E_NOK;

	// Set initial speed to 0 (PWM off)
	status = MOTOR_u8SetSpeed(0);
	if (status != E_OK)
		return E_NOK;

	return E_OK;
}

/*
 * Rotate motor to the right
 */
uint8_t MOTOR_u8RightRotate(MOTOR_PIN *motor_pins, uint8_t motorSpeed)
{
	uint8_t status;

	// Left pin HIGH, right pin LOW (e.g., IN1=1, IN2=0)
	status = DIO_u8SetPinValue(motor_pins[0].port, motor_pins[0].pin, HIGH);
	if (status != E_OK)
		return E_NOK;

	status = DIO_u8SetPinValue(motor_pins[1].port, motor_pins[1].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	// Set speed
	status = MOTOR_u8SetSpeed(motorSpeed);
	if (status != E_OK)
		return E_NOK;

	return E_OK;
}

/*
 * Rotate motor to the left
 */
uint8_t MOTOR_u8LeftRotate(uint8_t motorSpeed, MOTOR_PIN *motor_pins)
{
	uint8_t status;

	// Left pin LOW, right pin HIGH (e.g., IN1=0, IN2=1)
	status = DIO_u8SetPinValue(motor_pins[0].port, motor_pins[0].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	status = DIO_u8SetPinValue(motor_pins[1].port, motor_pins[1].pin, HIGH);
	if (status != E_OK)
		return E_NOK;

	// Set speed
	status = MOTOR_u8SetSpeed(motorSpeed);
	if (status != E_OK)
		return E_NOK;

	return E_OK;
}

/*
 * Turn off the motor
 */
uint8_t MOTOR_u8MotorOff(MOTOR_PIN *motor_pins)
{
	uint8_t status;

	// Both pins LOW (e.g., IN1=0, IN2=0)
	status = DIO_u8SetPinValue(motor_pins[0].port, motor_pins[0].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	status = DIO_u8SetPinValue(motor_pins[1].port, motor_pins[1].pin, LOW);
	if (status != E_OK)
		return E_NOK;

	// Set speed to 0 (instead of stopping Timer2)
	status = MOTOR_u8SetSpeed(0);
	if (status != E_OK)
		return E_NOK;

	return E_OK;
}

/*
 * Set motor speed (0–255)
 */
uint8_t MOTOR_u8SetSpeed(uint8_t duty_cycle)
{
	return TMR2_SetPWMDutyCycle(duty_cycle);
}