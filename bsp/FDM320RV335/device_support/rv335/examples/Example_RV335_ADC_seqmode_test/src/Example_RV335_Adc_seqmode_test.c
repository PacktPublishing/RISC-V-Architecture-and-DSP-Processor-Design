//###########################################################################
//
// FILE:   Example_RV335_Adc_seqmode_test.c
//
// TITLE:   
//
//###########################################################################
// $FDMTEK Release: $
// $Release Date:  $
// $Copyright:
//  (C) Copyright 2023 QingDao Fundamental Microelectronic .Ltd - http://www.fdmtek.com
//###########################################################################

//###########################################################################
//
// FILE:   Example_RV335_Adc_seqmode_test.c
//
// TITLE:  ADC Seq Mode Test Example
//
//! \addtogroup RV335_example_list
//! <h1> ADC Seq Mode Test (adc_seqmode_test)</h1>
//!
//! In this example, channel A0 is converted forever(with CONT_RUN=0) and logged in a buffer
//! (SampleTable)
//!
//! \b Watch \b Variables \n
//! - SampleTable - Log of converted values
//
//###########################################################################

//
// Included Files
//
#include "../include/RV335_Project.h"     // Device Headerfile and Examples Include File
#include "../include/RV335_SysCtrl.h"
#include "../routines/include/RV335_Device.h"
#include "../routines/include/RV335_Examples.h"

//
// Defines for ADC start parameters
//
//#define CPU_FRQ_150MHZ 150  //wuyn

#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
    //
    // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
    //
    #define ADC_MODCLK 0x3
#endif
#if (CPU_FRQ_100MHZ)
    //
    // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
    //
    #define ADC_MODCLK 0x2
#endif

//
// ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
//
#define ADC_CKPS   0x1

#define ADC_SHCLK  0xf   // S/H width in ADC module periods = 16 ADC clocks
#define AVG        1000  // Average sample limit
#define ZOFFSET    0x00  // Average Zero offset
#define BUF_SIZE   2048  // Sample buffer size

//
// Globals
//
Uint16 SampleTable[BUF_SIZE];
Uint16 temp;
//
// Main
//
void main(void)
{
    Uint16 i;

    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the RV335_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Specific clock setting for this example
    //
    SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/ADC_MODCLK

    //
    // Step 2. Initialize GPIO:
    // This example function is found in the RV335_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    // InitGpio();  // Skipped for this example

    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    /*DINT;*/

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the RV335_PieCtrl.c file.
    //
    /*InitPieCtrl();*/

    //
    // Disable CPU interrupts and clear all CPU interrupt flags
    //
    /*IER = 0x0000;*/
    /*IFR = 0x0000;*/

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in RV335_DefaultIsr.c.
    // This function is found in RV335_PieVect.c.
    //
    //InitPieVectTable();  //wuyn

    //
    // Step 4. Initialize all the Device Peripherals:
    // This function is found in RV335_InitPeripherals.c
    //
    // InitPeripherals(); // Not required for this example
    InitAdc();  // For this example, init the ADC

    //
    // Specific ADC setup for this example:
    //
    AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;
    AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // Cascaded mode (async save data in result1) //wuyn
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;   // Conv chanel A0
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;       //  Setup NO continuous run (run once for one loop for clearer monitor) //wuyn
    AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0;
    //
    // Step 5. User specific code, enable interrupts:
    //

    //
    // Clear SampleTable
    //
    for (i=0; i<BUF_SIZE; i++)
    {
        SampleTable[i] = 0;
    }

    //
    // Start SEQ1 for 1st conv
    //
    AdcRegs.ADCTRL2.all = 0x2000;

    //
    // Take ADC data and log the in SampleTable array
    //
    for(;;)
    {
        for (i=0; i<AVG; i++)
        {
            //
            // Wait for interrupt
            //
            while (AdcRegs.ADCST.bit.INT_SEQ1== 0){

            }

        	SampleTable[i] =((AdcRegs.ADCRESULT0>>4));  //align to actual value
            AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;         //clear interrupt
            AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;   //wuyn
            // Start SEQ1 in loops
            AdcRegs.ADCTRL2.all = 0x2000;

        }
//        break;

    }
}

//
// End of File
//

