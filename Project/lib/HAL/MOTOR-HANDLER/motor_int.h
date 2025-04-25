#ifndef HAL_MOTOR_DRIVER_MOTOR_INT_H_
#define HAL_MOTOR_DRIVER_MOTOR_INT_H_

#include "std_types.h"

// Initialize motor pins as outputs
uint8_t MOTOR_u8MotorInit(void);

// Rotate motor to the right
uint8_t MOTOR_u8RightRotate(uint8_t motorSpeed);

// Rotate motor to the left
uint8_t MOTOR_u8LeftRotate(uint8_t motorSpeed);

// Turn off the motor
uint8_t MOTOR_u8MotorOff(void);

uint8_t MOTOR_u8SetSpeed(uint8_t duty_cycle);

#endif