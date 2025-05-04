#ifndef TIMER0_INT_H_
#define TIMER0_INT_H_
#include <avr/interrupt.h>
#include "CPU_Configuration.h"
#include "std_types.h"

#define IS_VALID_MILLISECONDS(ms) ((ms) > 0)
#define IS_VALID_CALLBACK(func) ((func) != NULL_PTR)
typedef enum
{
    TIMER0_NORMAL_MODE,
    TIMER0_CTC_MODE,
    TIMER0_FAST_PWM_MODE
} TIMER0_Mode_t;
typedef enum
{
    TIMER0_PRESCALER_NO = 0b001,
    TIMER0_PRESCALER_8 = 0b010,
    TIMER0_PRESCALER_64 = 0b011,
    TIMER0_PRESCALER_256 = 0b100,
    TIMER0_PRESCALER_1024 = 0b101
} TIMER0_Prescaler_t;

typedef struct
{
    TIMER0_Mode_t timer0_mode;
    TIMER0_Prescaler_t timer0_prescaler;
    uint8_t compare_val;
    COM_Status_t com_rega;
    COM_Status_t com_regb;
    uint8_t OCR0A_val;
    uint8_t OCR0B_val;
} Timer0_params;

static TIMER0_Mode_t TMR0_Mode = TIMER0_NORMAL_MODE;  // Default mode
static void (*TMR0_OverflowCallback)(void) = nullptr; // For Normal mode
static void (*TMR0_CompareACallback)(void) = nullptr; // For CTC or PWM (OC0A)
static void (*TMR0_CompareBCallback)(void) = nullptr; // For PWM (OC0B)
static uint16_t TMR0_RequiredMatches = 0;             // For CTC mode
static uint16_t TMR0_CurrentMatches = 0;              // For CTC mode
static uint8_t TIMER0_CalculateTicks(Timer0_params *timer0_conf, uint16_t milliseconds, uint16_t *matches);
void TIMER0_INIT(Timer0_params *timer_conf);
void TMR0_SetOverflowCallback(void (*callbackFunc)(void));
void TMR0_SetCompareACallback(void (*callbackFunc)(void));
void TMR0_SetCompareBCallback(void (*callbackFunc)(void));
void TIMER0_Set_PWM_DutyCycle_A(Timer0_params *timer_conf, uint8_t speed);
void TIMER0_Set_PWM_DutyCycle_B(Timer0_params *timer_conf, uint8_t speed);
#endif