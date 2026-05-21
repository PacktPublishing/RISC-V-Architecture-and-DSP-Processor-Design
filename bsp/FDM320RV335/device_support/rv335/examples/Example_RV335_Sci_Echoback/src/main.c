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
#include "../routines/include/RV335_Examples.h"

//
// Function Prototypes
//
void scia_echoback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);

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

	Uint16 ReceivedChar;
	char *msg;


	//  Initialize GPIO:

	InitSciaGpio();

	//
	//  User specific code
	//
	LoopCount = 0;
	ErrorCount = 0;

	scia_fifo_init();	   // Initialize the SCI FIFO
	scia_echoback_init();  // Initialize SCI for echoback

	msg = "\r\n\n\nHello World!\0";
	scia_msg(msg);

	msg = "\r\nYou will enter a character, and the DSP will echo \
			it back! \n\0";
	scia_msg(msg);

	for(;;)
	{
		msg = "\r\nEnter a character: \0";
		scia_msg(msg);

		//
		// Wait for inc character
		//
		while(SciaRegs.SCIFFRX.bit.RXFFST !=1)
		{
			//
			// wait for XRDY =1 for empty state
			//
		}

		//
		// Get character
		//
		ReceivedChar = SciaRegs.SCIRXBUF.all;

		//
		// Echo character back
		//
		msg = "  You sent: \0";
		scia_msg(msg);
		scia_xmit(ReceivedChar);

		LoopCount++;
	}
}

//
// scia_echoback_init - Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F,
// default, 1 STOP bit, no parity
//
void
scia_echoback_init()
{
	//
	// Note: Clocks were turned on to the SCIA peripheral
	// in the InitSysCtrl() function
	//

	// 1 stop bit,  No loopback, No parity,8 char bits,
	// async mode, idle-line protocol
	//
	SciaRegs.SCICCR.all =0x0007;

	//
	// enable TX, RX, internal SCICLK,
	// Disable RX ERR, SLEEP, TXWAKE
	//
	SciaRegs.SCICTL1.all =0x0003;
	SciaRegs.SCICTL2.all =0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA =0;
	SciaRegs.SCICTL2.bit.RXBKINTENA =0;
#if (CPU_FRQ_150MHZ)
	SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
	SciaRegs.SCILBAUD    =0x00E7;
#endif
#if (CPU_FRQ_100MHZ)
	SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 20MHz.
	SciaRegs.SCILBAUD    =0x0044;
#endif
	SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

//
// scia_xmit - Transmit a character from the SCI
//
void
scia_xmit(int a)
{
	while (SciaRegs.SCIFFTX.bit.TXFFST != 0)
	{

	}
	SciaRegs.SCITXBUF=a;
}

//
// scia_msg -
//
void
scia_msg(char * msg)
{
	int i;
	i = 0;
	while(msg[i] != '\0')
	{
		scia_xmit(msg[i]);
		i++;
	}
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

