//###########################################################################
//
// FILE:   RV335_I2C.c
//
// TITLE:   
//
//###########################################################################
// $FDMTEK Release: $
// $Release Date:  $
// $Copyright:
//  (C) Copyright 2023 QingDao Fundamental Microelectronic .Ltd - http://www.fdmtek.com
//###########################################################################

//
// Included Files
//
#include "../include/RV335_Device.h"     // RV335 Headerfile Include File
#include "../include/RV335_Examples.h"   // RV335 Examples Include File

//
// InitI2C - This function initializes the I2C to a known state.
//
void 
InitI2C(void)
{
    //
	// Initialize I2C-A
    //
}	

//
// InitI2CGpio - This function initializes GPIO pins to function as I2C pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 
// Caution: 
// Only one GPIO pin should be enabled for SDAA operation.
// Only one GPIO pin shoudl be enabled for SCLA operation. 
// Comment out other unwanted lines.
//
void 
InitI2CGpio()
{

    //
    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled disabled by the user.  
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)

    //
    // Set qualification for selected pins to asynch only
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

    //
    // Configure SCI pins using GPIO regs
    // This specifies which of the possible GPIO pins will be I2C functional 
    // pins. Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 to SDAA 
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 to SCLA

}
	
//
// End of file
//

