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


// Function Prototypes
void mcbsp_init_dlb(void);
void mcbsp_xmit(int a, int b);
void error(void);
//
// Globals
//
Uint16 sdata1 = 0x000;    // Sent Data
Uint16 rdata1 = 0x000;    // Received Data

Uint16 sdata2 = 0x000;    // Sent Data
Uint16 rdata2 = 0x000;    // Received Data

Uint16 rdata1_point;
Uint16 rdata2_point;

Uint16 data_size = 16;

//
// Main
//
int main(void)
{
	// Initialize trap
	trap_init();
	 init_plic();//init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/

	// enable the GPIO for McBSP-A
	InitMcbspaGpio();
	mcbsp_init_dlb();

	// Run a loopback test in 8-bit mode
	if(data_size == 8)
	{
		sdata2 = 0x0000;           // value is a don't care for 8-bit mode
		sdata1 = 0x0000;           // 8-bit value to send
		rdata2_point = 0x0000;     // value is a don't care for 8-bit mode
		rdata1_point = sdata1;
		for(;;)
		{
			mcbsp_xmit(sdata1,sdata2);
			sdata1++;
			sdata1 = sdata1 & 0x00FF;            // Keep it to 8-bits

			// Check for receive
			while(McbspaRegs.SPCR1.bit.RRDY == 0 )
			{

			}

			rdata1 = McbspaRegs.DRR1.all;         // read DRR1
			if(rdata1 != rdata1_point)
			{
				error();
			}
			rdata1_point++;
			rdata1_point = rdata1_point & 0x00FF;  // Keep it to 8-bits

			//
			// Good place for a breakpoint
			// Check: rdatax_point = sdatax
			//        rdata1 = sdata1 - 1
			//
			__asm("    nop");
		}
	}
	// Run a loopback test in 16-bit mode
	else if(data_size == 16)
	{
		sdata2 = 0x0000;          // value is a don't care for 16-bit mode
		sdata1 = 0x0000;          // 16-bit value to send
		rdata2_point = 0x0000;    // value is a don't care for 16-bit mode
		rdata1_point = sdata1;
		for(;;)
		{
			mcbsp_xmit(sdata1,sdata2);
			sdata1++;

			// Check for receive
			while(McbspaRegs.SPCR1.bit.RRDY == 0 )
			{

			}

			rdata1 = McbspaRegs.DRR1.all;     // read DRR1

			if(rdata1 != rdata1_point)
			{
				error();
			}
			rdata1_point++;

			//
			// Good place for a breakpoint
			// Check: rdatax_point = sdatax
			//        rdata1 = sdata1 - 1
			//
			__asm("    nop");
		}
	}

	// Run a loopback test in 16-bit mode
	else if(data_size == 32)
	{
		sdata1 = 0x0000;
		sdata2 = 0xFFFF;
		rdata1_point = sdata1;
		rdata2_point = sdata2;
		for(;;)
		{
			mcbsp_xmit(sdata1,sdata2);
			sdata1++;
			sdata2--;
			// Check for receive
			while(McbspaRegs.SPCR1.bit.RRDY == 0 )
			{

			}
			rdata2 = McbspaRegs.DRR2.all;
			rdata1 = McbspaRegs.DRR1.all;
			if(rdata1 != rdata1_point)
			{
				error();
			}
			if(rdata2 != rdata2_point)
			{
				error();
			}
			rdata1_point++;
			rdata2_point--;

			//
			// Good place for a breakpoint
			// Check: rdatax_point = sdatax
			//        rdata1 = sdata1 - 1
			//        rdata2 = sdata2 + 1
			//
			__asm("    nop");
		}
	}
	return 0;
}

//
// error -
//
void error(void)
{
	__asm("     EBREAK");       // test failed!! Stop!
	for (;;);
}

//
// mcbsp_init_dlb -
//
void mcbsp_init_dlb()
{
	//
	// Reset FS generator, sample rate generator & transmitter
	//
	McbspaRegs.SPCR2.all=0x0000;

	McbspaRegs.SPCR1.all=0x0000;		// Reset Receiver, Right justify word

	//
	// Enable loopback mode for test. Comment out for normal McBSP
	// transfer mode.
	//
	McbspaRegs.SPCR1.bit.DLB = 1;

	McbspaRegs.MFFINT.all=0x0;			// Disable all interrupts

	//
	// Single-phase frame, 1 word/frame, No companding	(Receive)
	//
	McbspaRegs.RCR2.all=0x0;
	McbspaRegs.RCR1.all=0x0;

	//
	// Single-phase frame, 1 word/frame, No companding	(Transmit)
	//
	McbspaRegs.XCR2.all=0x0;
	McbspaRegs.XCR1.all=0x0;

	//
	// FSX generated internally, FSR derived from an external source
	//
	McbspaRegs.PCR.bit.FSXM = 1;

	//
	// CLKX generated internally, CLKR derived from an external source
	//
	McbspaRegs.PCR.bit.CLKXM = 1;

	//
	// CLKSM=1 (If SCLKME=0, i/p clock to SRG is LSPCLK)
	//
	McbspaRegs.SRGR2.bit.CLKSM = 1;

	McbspaRegs.SRGR2.bit.FPER = 31;		// FPER = 32 CLKG periods

	McbspaRegs.SRGR1.bit.FWID = 0;      // Frame Width = 1 CLKG period
	McbspaRegs.SRGR1.bit.CLKGDV = 1;	// CLKG frequency = LSPCLK/(CLKGDV+1)

	delay_loop();                		// Wait at least 2 SRG clock cycles

	//
	// Initialize McBSP Data Length
	//
	if(data_size == 8)             	// Run a loopback test in 8-bit mode
	{
		InitMcbspa8bit();
	}
	if(data_size == 16)            	// Run a loopback test in 16-bit mode
	{
		InitMcbspa16bit();
	}
	if(data_size == 32)            	// Run a loopback test in 32-bit mode
	{
		InitMcbspa32bit();
	}

	McbspaRegs.SPCR2.bit.GRST=1;    // Enable the sample rate generator
	delay_loop();
	McbspaRegs.SPCR2.bit.XRST=1;    // Release TX from Reset
	McbspaRegs.SPCR1.bit.RRST=1;    // Release RX from Reset
	McbspaRegs.SPCR2.bit.FRST=1;    // Frame Sync Generator reset
}


//
// mcbsp_xmit -
//
void mcbsp_xmit(int a, int b)
{
	McbspaRegs.DXR2.all=b;
	McbspaRegs.DXR1.all=a;
}
