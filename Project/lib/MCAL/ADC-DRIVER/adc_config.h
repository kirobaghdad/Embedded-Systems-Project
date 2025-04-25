#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_
#include "DIO-DRIVER/dio_int.h"
#include "CPU_Configuration.h"

/* Choose triggering mode:
 * - AUTO_TRIGGER: Hardware auto-triggering
 * - SOFTWARE_TRIGGER: Software-initiated conversion
 */
#define ADC_TRIGGERING_MODE SOFTWARE_TRIGGER

#define ADC_AUTO_TRIGGER_PIN ADC_0

/* Choose ADC resolution:
 * - _10_BIT_MODE: Full 10-bit resolution (right-adjusted)
 * - _8_BIT_MODE: 8-bit resolution (left-adjusted, faster read)
 */
#define ADC_READING_NO_OF_BITS _10_BIT_MODE

/* Choose reading technique:
 * - INTERRUPT_MODE: Use ADC interrupt to update data
 * - POLLING_MODE: Wait for conversion to complete (software polling)
 */
#define ADC_READING_TECHNIQUE INTERRUPT_MODE

#define ADC_0_CONNECTION {PORT_C, PIN_0} // PC0
#define ADC_1_CONNECTION {PORT_C, PIN_1} // PC1
#define ADC_2_CONNECTION {PORT_C, PIN_2} // PC2
#define ADC_3_CONNECTION {PORT_C, PIN_3} // PC3
#define ADC_4_CONNECTION {PORT_C, PIN_4} // PC4
#define ADC_5_CONNECTION {PORT_C, PIN_5} // PC5

#endif