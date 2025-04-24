#ifndef HAL_MOTOR_DRIVER_MOTOR_INT_H_
#define HAL_MOTOR_DRIVER_MOTOR_INT_H_

#include "std_types.h"

// Initialize motor pins as outputs
uint8_t MOTOR_u8MotorInit(void);

// Rotate motor to the right
uint8_t MOTOR_u8RightRotate(void);

// Rotate motor to the left
uint8_t MOTOR_u8LeftRotate(void);

// Turn off the motor
uint8_t MOTOR_u8MotorOff(void);

#endif /* HAL_MOTOR_DRIVER_MOTOR_INT_H_ */