#include "std_types.h"
#include "DIO-DRIVER/dio_int.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"
#include "TIMERS-DRIVER/TIMER1/tmr1_config.h"
#include "ICU-HANDLER/ICU_cfg.h"
#include "ICU-HANDLER/ICU_int.h"

// Static callback pointer
static void (*ICU_CallbackPtr)(void) = NULL_PTR;

// Helper macros for validation
#define IS_VALID_EDGE(edge) ((edge) == ICU_EDGE_FALLING || (edge) == ICU_EDGE_RISING)
#define IS_VALID_VALUE_PTR(ptr) ((ptr) != NULL_PTR)
#define IS_VALID_CALLBACK(func) ((func) != NULL_PTR)

/*
 * Initialize the ICU
 */
uint8_t ICU_Init(void)
{
    // Configure ICP1 (PB0) as input
    uint8_t status = DIO_u8SetPinMode(PORT_B, PIN_0, INPUT);
    if (status != E_OK)
        return E_NOK;

    // Clear Timer1 control registers
    TCCR1A = 0;
    TCCR1B = 0;

    // Configure noise canceler
#if ICU_NOISE_CANCELER_ENABLE == 1
    SET_BIT(TCCR1B, ICNC1);
#else
    CLR_BIT(TCCR1B, ICNC1);
#endif

    // Configure initial edge detection
#if ICU_DEFAULT_INITIAL_EDGE == ICU_EDGE_RISING
    SET_BIT(TCCR1B, ICES1);
#else
    CLR_BIT(TCCR1B, ICES1);
#endif

    // Set prescaler
    switch (ICU_DEFAULT_PRESCALER)
    {
    case TMR1_PRESCALER_NO:
        SET_BIT(TCCR1B, CS10);
        break;
    case TMR1_PRESCALER_8:
        SET_BIT(TCCR1B, CS11);
        break;
    case TMR1_PRESCALER_64:
        SET_BIT(TCCR1B, CS10);
        SET_BIT(TCCR1B, CS11);
        break;
    case TMR1_PRESCALER_256:
        SET_BIT(TCCR1B, CS12);
        break;
    case TMR1_PRESCALER_1024:
        SET_BIT(TCCR1B, CS10);
        SET_BIT(TCCR1B, CS12);
        break;
    default:
        return E_NOK;
    }

    // Reset timer counter
    TCNT1 = 0;

    // Clear input capture flag
    SET_BIT(TIFR1, ICF1);

    // Enable global interrupts
    return GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
}

/*
 * Deinitialize the ICU
 */
uint8_t ICU_DeInit(void)
{
    // Stop Timer1 by clearing clock select bits
    CLR_BIT(TCCR1B, CS10);
    CLR_BIT(TCCR1B, CS11);
    CLR_BIT(TCCR1B, CS12);

    // Disable input capture interrupt
    CLR_BIT(TIMSK1, ICIE1);

    // Clear callback
    ICU_CallbackPtr = NULL_PTR;

    return E_OK;
}

/*
 * Set edge detection
 */
uint8_t ICU_SetEdgeDetection(ICU_EdgeType edge)
{
    if (!IS_VALID_EDGE(edge))
        return E_NOK;

    if (edge == ICU_EDGE_RISING)
        SET_BIT(TCCR1B, ICES1);
    else
        CLR_BIT(TCCR1B, ICES1);

    return E_OK;
}

/*
 * Get the captured timer value
 */
uint16_t ICU_GetCaptureValue()
{
    return ICR1;
}

/*
 * Set callback function
 */
uint8_t ICU_SetCallback(void (*callbackFunc)(void))
{
    if (!IS_VALID_CALLBACK(callbackFunc))
    {
        CLR_BIT(TIMSK1, ICIE1);
        ICU_CallbackPtr = NULL_PTR;
        return E_NOK;
    }

    ICU_CallbackPtr = callbackFunc;
    SET_BIT(TIMSK1, ICIE1);
    return E_OK;
}

/*
 * Clear Timer1 counter
 */
uint8_t ICU_ClearTimer(void)
{
    TCNT1 = 0;
    return E_OK;
}

/*
 * Enable input capture interrupt
 */
uint8_t ICU_EnableInterrupt(void)
{
    SET_BIT(TIMSK1, ICIE1);
    return E_OK;
}

/*
 * Disable input capture interrupt
 */
uint8_t ICU_DisableInterrupt(void)
{
    CLR_BIT(TIMSK1, ICIE1);
    return E_OK;
}
