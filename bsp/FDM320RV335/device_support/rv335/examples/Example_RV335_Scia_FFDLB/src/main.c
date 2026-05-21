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
// Function Prototypes
//
void scia_loopback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void error();
void scia_rx_isr(void);
void scia_tx_isr(void);

//
// Globals
//
Uint16 LoopCount;
Uint16 ErrorCount;

int main(void)
{
	// Initialize trap
	trap_init();
	init_plic(); //init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/

	Uint16 SendChar;
	Uint16 ReceivedChar;

	InitSciGpio();



	// User specific functions, Reassign vectors (optional),
	// Enable Interrupts:
	//
	LoopCount = 0;
	ErrorCount = 0;

	scia_fifo_init();	   // Initialize the SCI FIFO
	scia_loopback_init();  // Initialize SCI for digital loop back

	//
	// Note: Autobaud lock is not required for this example
	//

	//
	// Send a character starting with 0
	//
	SendChar = 0;

	//
	// Step 6. Send Characters forever starting with 0x00 and going through
	// 0xFF.  After sending each, check the receive buffer for the correct
	// value
	//
	for(;;)
	{
		scia_xmit(SendChar);

		//
		// wait for RRDY/RXFFST =1 for 1 data available in FIFO
		//
		while(SciaRegs.SCIFFRX.bit.RXFFST !=1)
		{

		}

		//
		// Check received character
		//
		ReceivedChar = SciaRegs.SCIRXBUF.all;
		if(ReceivedChar != SendChar)
		{
			error();
		}

		//
		// Move to the next character and repeat the test
		//
		SendChar++;

		//
		// Limit the character to 8-bits
		//
		SendChar &= 0x00FF;
		LoopCount++;
	}
}

//
// Step 7. Insert all local Interrupt Service Routines (ISRs) and
// functions here:
//

//
// error -
//
void
error()
{
	ErrorCount++;
	//for (;;);
}

//
// scia_loopback_init - Test 1, SCIA  DLB, 8-bit word, baud rate 0x000F,
// default, 1 STOP bit, no parity
//
void
scia_loopback_init()
{
	//
	// Note: Clocks were turned on to the SCIA peripheral
	// in the InitSysCtrl() function
	//

	//
	// 1 stop bit,  No loopback, No parity,8 char bits, async mode,
	// idle-line protocol
	//
	SciaRegs.SCICCR.all =0x0007;

	//
	// enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE
	//
	SciaRegs.SCICTL1.all =0x0003;
	SciaRegs.SCICTL2.all =0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA =0;
	SciaRegs.SCICTL2.bit.RXBKINTENA =0;
	SciaRegs.SCIHBAUD    =0x0000;
	SciaRegs.SCILBAUD    =0x000F;
	SciaRegs.SCICCR.bit.LOOPBKENA =1;   // Enable loop back
	SciaRegs.SCICTL1.all =0x0023;       // Relinquish SCI from Reset
}

//
// scia_xmit - Transmit a character from the SCI
//
void
scia_xmit(int a)
{
	SciaRegs.SCITXBUF=a;
}

//
// scia_fifo_init - Initialize the SCI FIFO
//
void
scia_fifo_init()
{
	SciaRegs.SCIFFTX.all=0xE040;
	SciaRegs.SCIFFRX.all=0x204f;
	SciaRegs.SCIFFCT.all=0x0;
}

//
// End of File
//

