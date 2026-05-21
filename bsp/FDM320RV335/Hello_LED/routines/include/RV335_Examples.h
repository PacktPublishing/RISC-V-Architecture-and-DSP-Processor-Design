#ifndef RV335_EXAMPLES_H
#define RV335_EXAMPLES_H


#ifdef __cplusplus
extern "C" {
#endif

//
// Specify the PLL control register (PLLCR) and divide select (DIVSEL) value.
//

#define CPU_FRQ_100MHZ  0  //
#define CPU_FRQ_150MHZ  1  // 150 MHz CPU Freq (30 MHz input freq) by DEFAULT


//
// Include Example Header Files
//

//
// Prototypes for global functions within the .c files.
//
#include "RV335_GlobalPrototypes.h"
#include "RV335_EPwm_defines.h"       // Macros used for PWM examples.
#include "RV335_Dma_defines.h"        // Macros used for DMA examples.
#include "RV335_I2c_defines.h"        // Macros used for I2C examples.

#define PARTNO_28335  0xEF
#define PARTNO_28334  0xEE
#define PARTNO_28333  0xEA
#define PARTNO_28332  0xED

//
// Include files not used with DSP/BIOS
//
#ifndef DSP28_BIOS
#include "RV335_DefaultIsr.h"
#endif

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of RV335_EXAMPLES_H definition

//
// End of file
//

