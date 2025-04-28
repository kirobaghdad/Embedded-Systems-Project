#ifndef _ULTRASONIC_INT_H_
#define _ULTRASONIC_INT_H_
#include "std_types.h"

typedef struct
{
    IO echo = {PORT_B, PIN_0};
    IO trigger;
} ultrasonic_config_t;

void Ultrasonic_init(ultrasonic_config_t *ultrasonic_config);
void Ultrasonic_Trigger(ultrasonic_config_t *ultrasonic_config);
uint16_t Ultrasonic_readDistance(ultrasonic_config_t *ultrasonic_config);
void Ultrasonic_edgeProcessing(void);
#endif