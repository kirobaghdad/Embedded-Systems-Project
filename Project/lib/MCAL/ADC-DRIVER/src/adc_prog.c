#include "std_types.h"
#include "CPU_Configuration.h"
#include "ADC-DRIVER/adc_config.h"
#include "ADC-DRIVER/adc_int.h"
#include "ADC-DRIVER/adc_register.h"
#include "GLOBAL-INTERRUPT-DRIVER/global_interrupt_int.h"

typedef struct
{
	uint8_t port;
	uint8_t pin;
} ADC_PIN;

ADC_PIN ADC_PINS[6] = {
	ADC_0_CONNECTION,
	ADC_1_CONNECTION,
	ADC_2_CONNECTION,
	ADC_3_CONNECTION,
	ADC_4_CONNECTION,
	ADC_5_CONNECTION};

static uint16_t *ADC_READ = NULL_PTR;
static void (*ADC_Callback)(void) = NULL_PTR;

#define IS_VALID_ADC_PIN(pin) ((pin) <= ADC_5)
#define IS_VALID_VREF(vref) ((vref) <= INTERNAL_2_56V)
#define IS_VALID_TRIGGER_MODE(mode) ((mode) <= TIMER1_CAPTURE)

static void ADC_vidSetMultiplexer(uint8_t adc_pin);

/*
 * Initialize the ADC with the specified voltage reference
 */
void ADC_vidAdcInit(uint8_t voltage_reference)
{
	if (!IS_VALID_VREF(voltage_reference))
		return;

	// Enable ADC
	SET_BIT(ADCSRA, 7); // ADEN = 1

	// Set prescaler to 128 (16 MHz / 128 = 125 kHz, within 50–200 kHz range)
	SET_BIT(ADCSRA, 2); // ADPS2 = 1
	SET_BIT(ADCSRA, 1); // ADPS1 = 1
	SET_BIT(ADCSRA, 0); // ADPS0 = 1

	// Set voltage reference
	switch (voltage_reference)
	{
	case AREF_VOLTAGE:
		CLR_BIT(ADMUX, 7); // REFS1 = 0
		CLR_BIT(ADMUX, 6); // REFS0 = 0
		break;
	case VCC:
		CLR_BIT(ADMUX, 7); // REFS1 = 0
		SET_BIT(ADMUX, 6); // REFS0 = 1
		break;
	case INTERNAL_2_56V:
		SET_BIT(ADMUX, 7); // REFS1 = 1
		SET_BIT(ADMUX, 6); // REFS0 = 1
		break;
	}

	// Configure triggering mode
#if ADC_TRIGGERING_MODE == AUTO_TRIGGER
	ADC_vidAdcPinInit(ADC_AUTO_TRIGGER_PIN);
	ADC_vidSetMultiplexer(ADC_AUTO_TRIGGER_PIN);
	ADC_vidSetAutoTriggerMode(EXTERNAL_INT0); // Default to INT0 for auto-trigger
	SET_BIT(ADCSRA, 5);						  // ADATE = 1 (enable auto-trigger)
#else
	CLR_BIT(ADCSRA, 5); // ADATE = 0 (disable auto-trigger)
#endif

	// Configure reading technique
#if ADC_READING_TECHNIQUE == INTERRUPT_MODE
	SET_BIT(ADCSRA, 3); // ADIE = 1 (enable interrupt)
	GLOBAL_INTERRUPT_vidGlobalInterruptEnable(ENABLED);
#else
	CLR_BIT(ADCSRA, 3); // ADIE = 0 (disable interrupt)
#endif

	// Configure resolution
#if ADC_READING_NO_OF_BITS == _8_BIT_MODE
	SET_BIT(ADMUX, 5); // ADLAR = 1 (left-adjust for 8-bit mode)
#else
	CLR_BIT(ADMUX, 5); // ADLAR = 0 (right-adjust for 10-bit mode)
#endif
}

/*
 * Initialize the specified ADC pin as input
 */
void ADC_vidAdcPinInit(uint8_t adc_pin)
{
	if (!IS_VALID_ADC_PIN(adc_pin))
		return;

	DIO_u8SetPinMode(ADC_PINS[adc_pin].port, ADC_PINS[adc_pin].pin, INPUT);
}

/*
 * Start an ADC conversion and get the result (polling or interrupt)
 */
void ADC_vidAdcGetRead(uint8_t adc_pin)
{
	if (!IS_VALID_ADC_PIN(adc_pin) || ADC_READ == NULL_PTR)
		return;

	ADC_vidSetMultiplexer(adc_pin);
	SET_BIT(ADCSRA, 6); // ADSC = 1 (start conversion)

#if ADC_READING_TECHNIQUE == POLLING_MODE
	while (GET_BIT(ADCSRA, 4) == 0) // Wait for ADIF = 1
	{
		asm("NOP");
	}
#if ADC_READING_NO_OF_BITS == _8_BIT_MODE
	*ADC_READ = ADCH; // Read 8-bit result from ADCH
#else
	*ADC_READ = ADC; // Read 10-bit result from ADC
#endif
	SET_BIT(ADCSRA, 4); // Clear ADIF by writing 1
#endif
}

/*
 * Get the result for auto-triggered conversions
 */
void ADC_vidGetAutoTriggerRead(void)
{
#if ADC_READING_TECHNIQUE == POLLING_MODE
	if (ADC_READ == NULL_PTR)
		return;

	while (GET_BIT(ADCSRA, 4) == 0) // Wait for ADIF = 1
	{
		asm("NOP");
	}
#if ADC_READING_NO_OF_BITS == _8_BIT_MODE
	*ADC_READ = ADCH;
#else
	*ADC_READ = ADC;
#endif
	SET_BIT(ADCSRA, 4); // Clear ADIF
#endif
}

/*
 * Set the auto-trigger source
 */
void ADC_vidSetAutoTriggerMode(uint8_t mode)
{
	if (!IS_VALID_TRIGGER_MODE(mode))
		return;

	// Set ADTS[2:0] in ADCSRB
	ADCSRB &= ~(0x07);		 // Clear ADTS bits
	ADCSRB |= (mode & 0x07); // Set trigger source
}

/*
 * Set the variable to store ADC readings
 */
void ADC_vidSetReadingVariable(uint16_t *adc_read)
{
	if (adc_read != NULL_PTR)
		ADC_READ = adc_read;
}

/*
 * Set the callback function for ADC interrupt
 */
void ADC_vidSetCallBack(void (*func)(void))
{
	if (func != NULL_PTR)
		ADC_Callback = func;
}

/*
 * ADC Conversion Complete Interrupt Service Routine
 */
void __vector_16(void) __attribute__((signal));
void __vector_16(void)
{
#if ADC_READING_TECHNIQUE == INTERRUPT_MODE
	if (ADC_READ != NULL_PTR)
	{
#if ADC_READING_NO_OF_BITS == _8_BIT_MODE
		*ADC_READ = ADCH;
#else
		*ADC_READ = ADC;
#endif
	}
	if (ADC_Callback != NULL_PTR)
		ADC_Callback();
#endif
}

/*
 * Set the ADC multiplexer for the specified channel
 */
static void ADC_vidSetMultiplexer(uint8_t adc_pin)
{
	if (!IS_VALID_ADC_PIN(adc_pin))
		return;

	// Set MUX[3:0] in ADMUX (MUX4 = 0 for single-ended inputs)
	ADMUX &= ~(0x1F);		   // Clear MUX[4:0]
	ADMUX |= (adc_pin & 0x0F); // Set channel (ADC0–ADC5)
}