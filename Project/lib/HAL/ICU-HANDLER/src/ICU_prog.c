#include "ICU-HANDLER/ICU_cfg.h"
#include "ICU-HANDLER/ICU_int.h"

#include <avr/interrupt.h>

static void (*ICU_CallbackPtr)(void) = 0;

void ICU_Init(void)
{

    CLR_BIT(DDRB, PB0);

    TCCR1A_REG = 0;

    TCCR1B_REG &= ~((1 << ICNC1) | (1 << ICES1) | (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS11) | (1 << CS10));

#if (ICU_NOISE_CANCELER_ENABLE == 1)
    SET_BIT(TCCR1B_REG, ICNC1);
#endif

#if (ICU_DEFAULT_INITIAL_EDGE == ICU_EDGE_RISING)
    SET_BIT(TCCR1B_REG, ICES1);

#endif

    switch (ICU_DEFAULT_PRESCALER)
    {
    case TMR1_PRESCALER_NO:
        SET_BIT(TCCR1B_REG, CS10);
        break;
    case TMR1_PRESCALER_8:
        SET_BIT(TCCR1B_REG, CS11);
        break;
    case TMR1_PRESCALER_64:
        SET_BIT(TCCR1B_REG, CS10);
        SET_BIT(TCCR1B_REG, CS11);
        break;
    case TMR1_PRESCALER_256:
        SET_BIT(TCCR1B_REG, CS12);
        break;
    case TMR1_PRESCALER_1024:
        SET_BIT(TCCR1B_REG, CS10);
        SET_BIT(TCCR1B_REG, CS12);
        break;
    }

    TCNT1_REG = 0;

    SET_BIT(TIFR_REG, ICF1);
}

void ICU_DeInit(void)
{

    CLR_BIT(TCCR1B_REG, CS10);
    CLR_BIT(TCCR1B_REG, CS11);
    CLR_BIT(TCCR1B_REG, CS12);

    CLR_BIT(TIMSK_REG, ICIE1);

    ICU_CallbackPtr = 0;
}

void ICU_SetEdgeDetection(ICU_EdgeType edge)
{

    if (edge == ICU_EDGE_RISING)
    {
        SET_BIT(TCCR1B_REG, ICES1);
    }
    else
    {
        CLR_BIT(TCCR1B_REG, ICES1);
    }
}

uint16_t ICU_GetCaptureValue(void)
{
    return ICR1_REG;
}

void ICU_SetCallback(void (*callbackFunc)(void))
{
    ICU_CallbackPtr = callbackFunc;

    if (ICU_CallbackPtr != 0)
    {
        SET_BIT(TIMSK_REG, ICIE1);

        sei();
    }
    else
    {

        CLR_BIT(TIMSK_REG, ICIE1);
    }
}

void ICU_ClearTimer(void)
{
    TCNT1_REG = 0;
}

void ICU_EnableInterrupt(void)
{
    SET_BIT(TIMSK_REG, ICIE1);
}

void ICU_DisableInterrupt(void)
{
    CLR_BIT(TIMSK_REG, ICIE1);
}

ISR(TIMER1_CAPT_vect)
{

    if (ICU_CallbackPtr != 0)
    {

        ICU_CallbackPtr();
    }
}