#ifndef HAL_MOTOR_DRIVER_MOTOR_INT_H_
#define HAL_MOTOR_DRIVER_MOTOR_INT_H_

#include "std_types.h"
typedef struct
{
	uint8_t port;
	uint8_t pin;
} MOTOR_PIN;

// Initialize motor pins as outputs
uint8_t MOTOR_u8MotorInit(MOTOR_PIN *motor_pins);

// Rotate motor to the right
uint8_t MOTOR_u8RightRotate(MOTOR_PIN *motor_pins, uint8_t motorSpeed);

// Rotate motor to the left
uint8_t MOTOR_u8LeftRotate(uint8_t motorSpeed, MOTOR_PIN *motor_pins);

// Turn off the motor
uint8_t MOTOR_u8MotorOff(MOTOR_PIN *motor_pins);

uint8_t MOTOR_u8SetSpeed(uint8_t duty_cycle);

#endif