#ifndef HAL_MOTOR_DRIVER_MOTOR_INT_H_
#define HAL_MOTOR_DRIVER_MOTOR_INT_H_

#include "std_types.h"

typedef enum
{
	TIMER1_CallBack,
	TIMER2_CallBack
} MOTOR_CALLBACK_TYPE;
typedef struct
{
	IO motor_pins[3];
	MOTOR_CALLBACK_TYPE PWM_Callback; // Callback function for PWM
} MOTOR_CONFIG;

// Initialize motor pins as outputs
uint8_t MOTOR_u8MotorInit(MOTOR_CONFIG *motor_config);

// Rotate motor to the right
uint8_t MOTOR_u8RightRotate(MOTOR_CONFIG *motor_config, uint8_t motorSpeed);

// Rotate motor to the left
uint8_t MOTOR_u8LeftRotate(MOTOR_CONFIG *motor_config, uint8_t motorSpeed);

// Turn off the motor
uint8_t MOTOR_u8MotorOff(MOTOR_CONFIG *motor_config);

uint8_t MOTOR_u8SetSpeed(uint8_t duty_cycle, MOTOR_CALLBACK_TYPE callbackType);

#endif