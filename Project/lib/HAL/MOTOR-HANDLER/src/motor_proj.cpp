#include "std_types.h"
#include "GPIO/GPIO_int.h"
#include "MOTOR-HANDLER/motor_cfg.h"
#include "MOTOR-HANDLER/motor_int.h"
#include "TIMER0/TIMER0_int.h"

void MOTOR_u8MotorInit(MOTOR_CONFIG *motor_config, Timer0_params *timer0_conf)
{

	GPIO_SetPinMode(motor_config->motor_pins[0].port, motor_config->motor_pins[0].pin, OUTPUT);
	GPIO_SetPinMode(motor_config->motor_pins[1].port, motor_config->motor_pins[1].pin, OUTPUT);
	GPIO_SetPinMode(motor_config->motor_pins[2].port, motor_config->motor_pins[2].pin, OUTPUT);
	GPIO_SetPinValue(motor_config->motor_pins[0].port, motor_config->motor_pins[0].pin, LOW);
	GPIO_SetPinValue(motor_config->motor_pins[1].port, motor_config->motor_pins[1].pin, LOW);

	MOTOR_u8SetSpeed(0, motor_config->PWM_Callback, timer0_conf); // Set speed to 0%
	TIMER0_INIT(timer0_conf);
	
}

/*
 * Rotate motor to the right
 */
void MOTOR_u8RightRotate(MOTOR_CONFIG *motor_pins, uint8_t motorSpeed, Timer0_params *timer_conf)
{

	GPIO_SetPinValue(motor_pins->motor_pins[0].port, motor_pins->motor_pins[0].pin, HIGH);
	GPIO_SetPinValue(motor_pins->motor_pins[1].port, motor_pins->motor_pins[1].pin, LOW);
	MOTOR_u8SetSpeed(motorSpeed, motor_pins->PWM_Callback, timer_conf); // Set speed for Timer0 (Channel A)
}

/*
 * Rotate motor to the left
 */
void MOTOR_u8LeftRotate(MOTOR_CONFIG *motor_pins, uint8_t motorSpeed, Timer0_params *timer_conf)
{

	GPIO_SetPinValue(motor_pins->motor_pins[0].port, motor_pins->motor_pins[0].pin, LOW);

	GPIO_SetPinValue(motor_pins->motor_pins[1].port, motor_pins->motor_pins[1].pin, HIGH);


	MOTOR_u8SetSpeed(motorSpeed, motor_pins->PWM_Callback, timer_conf);
}

/*
 * Turn off the motor
 */
void MOTOR_u8MotorOff(MOTOR_CONFIG *motor_config, Timer0_params *timer_conf)
{

	GPIO_SetPinValue(motor_config->motor_pins[0].port, motor_config->motor_pins[0].pin, LOW);
	GPIO_SetPinValue(motor_config->motor_pins[1].port, motor_config->motor_pins[1].pin, LOW);
	MOTOR_u8SetSpeed(0, motor_config->PWM_Callback, timer_conf); // Set speed to 0%
}

/*
 * Set motor speed (0–100%)
 */
void MOTOR_u8SetSpeed(uint8_t duty_cycle, MOTOR_CALLBACK_TYPE callbackType, Timer0_params *timer_conf)
{
	switch (callbackType)
	{
	case TIMER0_CallBack_A:
		TIMER0_Set_PWM_DutyCycle_A(timer_conf, duty_cycle); // Set duty cycle for Timer0 (Channel A)
		break;
	case TIMER0_CallBack_B:
		TIMER0_Set_PWM_DutyCycle_B(timer_conf, duty_cycle); // Set duty cycle for Timer0 (Channel B)
		break;
	}
}