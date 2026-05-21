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
// Defines
//
#define CPU_FREQ 	 (Uint32)150E6
#define LSPCLK_FREQ  (Uint32)(CPU_FREQ / 4)
#define SCI_FREQ 	 (Uint32)100E3
#define SCI_PRD 	 (Uint16)((LSPCLK_FREQ / (SCI_FREQ * 8)) - 1)

//
// Function Prototypes
//
void sciaTxFifoIsr(void);
void sciaRxFifoIsr(void);
void scibTxFifoIsr(void);
void scibRxFifoIsr(void);
void scia_fifo_init(void);
void scib_fifo_init(void);
void error(void);

//
// Globals
//
Uint16 sdataA[8];    // Send data for SCI-A
Uint16 sdataB[8];    // Send data for SCI-B
Uint16 rdataA[8];    // Received data for SCI-A
Uint16 rdataB[8];    // Received data for SCI-A
Uint16 rdata_pointA; // Used for checking the received data
Uint16 rdata_pointB;

int main(void)
{
	// Initialize trap
	trap_init();
	init_plic(); //init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/

	Uint16 i;

	InitSciGpio();

	IntVectTable.st.SCIRXINTA = &sciaRxFifoIsr;
	IntVectTable.st.SCITXINTA = &sciaTxFifoIsr;
	IntVectTable.st.SCIRXINTB = &scibRxFifoIsr;
	IntVectTable.st.SCITXINTB = &scibTxFifoIsr;


	//
	// Initialize all the Device Peripherals:
	// This function is found in RV335_InitPeripherals.c
	//
	// InitPeripherals(); // Not required for this example
	scia_fifo_init();  // Init SCI-A
	scib_fifo_init();  // Init SCI-B

	//
	// User specific code, enable interrupts:
	//

	//
	// Init send data.  After each transmission this data will be updated for
	// the next transmission
	//
	for(i = 0; i<8; i++)
	{
		sdataA[i] = i;
	}

	for(i = 0; i<8; i++)
	{
		sdataB[i] = 0xFF - i;
	}

	rdata_pointA = sdataA[0];
	rdata_pointB = sdataB[0];

	//
	// Enable interrupts required for this example
	//
	enable_interrupt( GET_INT_NUM(IntVectTable.st.SCIRXINTA));
	enable_interrupt( GET_INT_NUM(IntVectTable.st.SCITXINTA));
	enable_interrupt( GET_INT_NUM(IntVectTable.st.SCIRXINTB));
	enable_interrupt( GET_INT_NUM(IntVectTable.st.SCITXINTB));

	//
	// Step 6. IDLE loop. Just sit and loop forever (optional):
	//
	for(;;);
}

//
// errror -
//
void
error(void)
{
	for (;;);
}

//
// sciaTxFifoIsr -
//
void
sciaTxFifoIsr(void)
{
	Uint16 i;
	for(i=0; i< 8; i++)
	{
		SciaRegs.SCITXBUF=sdataA[i];     // Send data
	}

	for(i=0; i< 8; i++)                 // Increment send data for next cycle
	{
		sdataA[i] = (sdataA[i]+1) & 0x00FF;
	}

	SciaRegs.SCIFFTX.bit.TXFFINTCLR=1;	// Clear SCI Interrupt flag
}

//
// sciaRxFifoIsr -
//
void
sciaRxFifoIsr(void)
{
	Uint16 i;
	for(i=0;i<8;i++)
	{
		rdataA[i]=SciaRegs.SCIRXBUF.all;	 // Read data
	}

	for(i=0;i<8;i++)                         // Check received data
	{
		if(rdataA[i] != ( (rdata_pointA+i) & 0x00FF) )
		{
			error();
		}
	}

	rdata_pointA = (rdata_pointA+1) & 0x00FF;

	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;      // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;      // Clear Interrupt flag

}

//
// scia_fifo_init -
//
void
scia_fifo_init()
{
	//
	// 1 stop bit,  No loopback, No parity,8 char bits, async mode,
	// idle-line protocol
	//
	SciaRegs.SCICCR.all =0x0007;

	//
	// enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE
	//
	SciaRegs.SCICTL1.all =0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
	SciaRegs.SCIHBAUD = SCI_PRD >> 8;
	SciaRegs.SCILBAUD = SCI_PRD;
	SciaRegs.SCICCR.bit.LOOPBKENA =1;   // Enable loop back
	SciaRegs.SCIFFTX.all=0xC028;
	SciaRegs.SCIFFRX.all=0x0028;
	SciaRegs.SCIFFCT.all=0x00;

	SciaRegs.SCICTL1.all =0x0023;      // Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
}

//
// scibTxFifoIsr -
//
void
scibTxFifoIsr(void)
{
	Uint16 i;
	for(i=0; i< 8; i++)
	{
		ScibRegs.SCITXBUF=sdataB[i];     // Send data
	}

	for(i=0; i< 8; i++)                  // Increment send data for next cycle
	{
		sdataB[i] = (sdataB[i]-1) & 0x00FF;
	}

	ScibRegs.SCIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag
}

//
// scibRxFifoIsr -
//
void
scibRxFifoIsr(void)
{
	Uint16 i;
	for(i=0;i<8;i++)
	{
		rdataB[i]=ScibRegs.SCIRXBUF.all;	// Read data
	}
	for(i=0;i<8;i++)                        // Check received data
	{
		if(rdataB[i] != ( (rdata_pointB-i) & 0x00FF) )
		{
			error();
		}
	}
	rdata_pointB = (rdata_pointB-1) & 0x00FF;

	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;      // Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1; 	    // Clear Interrupt flag
}

//
// scib_fifo_init -
//
void
scib_fifo_init()
{
	//
	// 1 stop bit,  No loopback, No parity,8 char bits,
	// async mode, idle-line protocol
	//
	ScibRegs.SCICCR.all =0x0007;

	//
	// enable TX, RX, internal SCICLK,
	// Disable RX ERR, SLEEP, TXWAKE
	//
	ScibRegs.SCICTL1.all =0x0003;
	ScibRegs.SCICTL2.bit.TXINTENA =1;
	ScibRegs.SCICTL2.bit.RXBKINTENA =1;
	ScibRegs.SCIHBAUD    =0x0000;
	ScibRegs.SCILBAUD    =SCI_PRD;
	ScibRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	ScibRegs.SCIFFTX.all=0xC028;
	ScibRegs.SCIFFRX.all=0x0028;
	ScibRegs.SCIFFCT.all=0x00;

	ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
	ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	ScibRegs.SCIFFRX.bit.RXFIFORESET=1;
}

//
// End of File
//

