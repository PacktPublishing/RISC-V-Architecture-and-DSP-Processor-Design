//###########################################################################
//
// FILE:   RV335_Examples.h
//
// TITLE:   
//
//###########################################################################
// $FDMTEK Release: $
// $Release Date:  $
// $Copyright:
//  (C) Copyright 2023 QingDao Fundamental Microelectronic .Ltd - http://www.fdmtek.com
//###########################################################################

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



//
// Include files not used with DSP/BIOS
//
#ifndef SPRINGCORE_2204_BIOS
#include "RV335_DefaultIsr.h"
#endif



#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of RV335_EXAMPLES_H definition

//
// End of file
//

