//###########################################################################
//
// FILE:   main.c
//
// TITLE:   
//
//###########################################################################
// $FDMTEK Release: $
// $Release Date:  $
// $Copyright:
//  (C) Copyright 2023 QingDao Fundamental Microelectronic .Ltd - http://www.fdmtek.com
//###########################################################################

#include "../include/RV335_Project.h"     // Device Headerfile and Examples Include File
#include "../include/RV335_SysCtrl.h"
#include "../routines/include/RV335_Device.h"


//
// Defines that select the example to compile in.
// Only one example should be set as 1 the rest should be set as 0.
//
#define EXAMPLE1 1  // Use DATA registers to toggle I/O's
#define EXAMPLE2 0 // Use SET/CLEAR registers to toggle I/O's
#define EXAMPLE3 0  // Use TOGGLE registers to toggle I/O's

//
// Function Prototypes
//
void Gpio_delay_loop(void);
void Gpio_select(void);
void Gpio_example1(void);
void Gpio_example2(void);
void Gpio_example3(void);


int main(void)
{
	// Initialize trap
	trap_init();
	 init_plic();//init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/
	//
	// For this example use the following configuration
	//
	Gpio_select();

#if EXAMPLE1
	//
	// This example uses DATA registers to toggle I/O's
	//
	Gpio_example1();
#endif  // - EXAMPLE1

#if EXAMPLE2
	//
	// This example uses SET/CLEAR registers to toggle I/O's
	//
	Gpio_example2();
#endif

#if EXAMPLE3
	//
	// This example uses TOGGLE registers to toggle I/O's
	//
	Gpio_example3();
#endif

	return 0;
}



//
// delay_loop -
//
void Gpio_delay_loop()
{
	volatile long i;
	for (i = 0; i < 1000000; i++)
	{

	}
}

//
// Gpio_example1 -
//
void Gpio_example1(void)
{
	//
	// Example 1: Toggle I/Os using DATA registers
	//
	for(;;)
	{
		GpioDataRegs.GPADAT.all    =0xAAAAAAAA;
		GpioDataRegs.GPBDAT.all    =0x0000000A;

		Gpio_delay_loop();

		GpioDataRegs.GPADAT.all    =0x55555555;
		GpioDataRegs.GPBDAT.all    =0x00000005;

		Gpio_delay_loop();
	}
}

//
// Gpio_example2 -
//
void
Gpio_example2(void)
{
	//
	// Example 2: Toggle I/Os using SET/CLEAR registers
	//
	for(;;)
	{
		GpioDataRegs.GPASET.all    =0xAAAAAAAA;
		GpioDataRegs.GPACLEAR.all  =0x55555555;

		GpioDataRegs.GPBSET.all    =0x0000000A;
		GpioDataRegs.GPBCLEAR.all  =0x00000005;

		Gpio_delay_loop();

		GpioDataRegs.GPACLEAR.all    =0xAAAAAAAA;
		GpioDataRegs.GPASET.all      =0x55555555;

		GpioDataRegs.GPBCLEAR.all    =0x0000000A;
		GpioDataRegs.GPBSET.all      =0x00000005;

		Gpio_delay_loop();
	}
}


// Gpio_example3 -
void  Gpio_example3(void)
{
	// Example 2: Toggle I/Os using TOGGLE registers

	// Set pins to a known state
	GpioDataRegs.GPASET.all    =0xAAAAAAAA;
	GpioDataRegs.GPACLEAR.all  =0x55555555;

	GpioDataRegs.GPBSET.all    =0x0000000A;
	GpioDataRegs.GPBCLEAR.all  =0x00000005;

	//
	// Use TOGGLE registers to flip the state of the pins.
	// Any bit set to a 1 will flip state (toggle)
	// Any bit set to a 0 will not toggle.
	//
	for(;;)
	{
		GpioDataRegs.GPATOGGLE.all =0xFFFFFFFF;
		GpioDataRegs.GPBTOGGLE.all =0x0000000F;
		Gpio_delay_loop();
	}
}


//
// Gpio_select -
//
void Gpio_select(void)
{
	GpioCtrlRegs.GPAMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPAMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPAMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;   // All outputs
	GpioCtrlRegs.GPBDIR.all = 0x0000000F;   // All outputs
}
