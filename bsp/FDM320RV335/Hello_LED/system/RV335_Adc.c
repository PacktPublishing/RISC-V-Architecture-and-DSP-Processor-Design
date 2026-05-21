//
// Included Files
//

#include "../include/RV335_GlobalVariableDefs.h"

//
// Defines
//
#define ADC_usDELAY  5000L


// add for rv
void ADC_cal (void)
{
	AdcRegs.ADCREFSEL.all = 0x3561;
	AdcRegs.ADCOFFTRIM.all = 0x0;
}

//
// InitAdc - This function initializes ADC to a known state.
//
void 
InitAdc(void)
{
    extern void RV335_usDelay(int Count);

    //
    //                          *IMPORTANT*
    // The ADC_cal function, which  copies the ADC calibration values from 
    // TI reserved OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs 
    // automatically in the Boot ROM. If the boot ROM code is bypassed during
    // the debug process, the following function MUST be called for the ADC to
    // function according to specification. The clocks to the ADC MUST be 
    // enabled before calling this function. See the device data manual and/or
    // the ADC Reference Manual for more information.
    //

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    ADC_cal();


    //
    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and 
    // ADC core. Before the first conversion is performed a 5ms delay must be
    // observed after power up to give all analog circuits time to power up 
    // and settle
    //

    //
    // Please note that for the delay function below to operate correctly the
    // CPU_RATE define statement in the RV335_Examples.h file must
    // contain the correct CPU clock period in nanoseconds.
    //
    AdcRegs.ADCTRL3.all = 0x00E0;  // Power up bandgap/reference/ADC circuits
    // DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels
}

//
// End of file
//

