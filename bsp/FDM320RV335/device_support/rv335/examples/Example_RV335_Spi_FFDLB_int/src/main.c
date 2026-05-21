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

void spiTxFifoIsr(void);
void spiRxFifoIsr(void);
void spi_fifo_init(void);
void error();

//
// Globals
//
Uint16 sdata[8];     // Send data buffer
Uint16 rdata[8];     // Receive data buffer

//
// Keep track of where we are in the data stream to check received data
//
Uint16 rdata_point;


int main(void)
{
	// Initialize trap
	trap_init();
	init_plic(); //init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/

	Uint16 i;


	//
	// Setup only the GP I/O only for SPI-A functionality
	//
	InitSpiaGpio();

	//
	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	//

	IntVectTable.st.SPIRXINTA = &spiRxFifoIsr;
	IntVectTable.st.SPITXINTA = &spiTxFifoIsr;

	//
	// Initialize all the Device Peripherals:
	//

	spi_fifo_init();	  // Initialize the SPI only

	//
	// User specific code, enable interrupts:
	//

	//
	// Initialize the send data buffer
	//
	for(i=0; i<8; i++)
	{
		sdata[i] = i;
	}
	rdata_point = 0;

	//
	// Enable interrupts required for this example
	//

	enable_interrupt( GET_INT_NUM(IntVectTable.st.SPIRXINTA));
	enable_interrupt( GET_INT_NUM(IntVectTable.st.SPITXINTA));


	//
	// Step 6. IDLE loop. Just sit and loop forever (optional)
	//
	for(;;);
}

//
// Some Useful local functions
//


//
// error -
//
void
error(void)
{
	for (;;);
}

//
// spi_fifo_init -
//
void
spi_fifo_init()
{
	//
	// Initialize SPI FIFO registers
	//
	SpiaRegs.SPICCR.bit.SPISWRESET=0; // Reset SPI

	SpiaRegs.SPICCR.all=0x001F; // 16-bit character, Loopback mode
	SpiaRegs.SPICTL.all=0x0017; // Interrupt enabled, Master/Slave XMIT enabled
	SpiaRegs.SPISTS.all=0x0000;
	SpiaRegs.SPIBRR=0x0063;     // Baud rate
	SpiaRegs.SPIFFTX.all=0xC028;       // Enable FIFO's, set TX FIFO level to 8
	SpiaRegs.SPIFFRX.all=0x0028;       // Set RX FIFO level to 8
	SpiaRegs.SPIFFCT.all=0x00;
	SpiaRegs.SPIPRI.all=0x0010;

	SpiaRegs.SPICCR.bit.SPISWRESET=1;  // Enable SPI

	SpiaRegs.SPIFFTX.bit.TXFIFO=1;
	SpiaRegs.SPIFFRX.bit.RXFIFORESET=1;
}

//
// spiTxFifoIsr -
//
void
spiTxFifoIsr(void)
{
	Uint16 i;
	for(i=0;i<8;i++)
	{
	   SpiaRegs.SPITXBUF=sdata[i];      // Send data
	}

	for(i=0;i<8;i++)                    // Increment data for next cycle
	{
		sdata[i] = sdata[i]+ 1;
	}

	SpiaRegs.SPIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag
}

//
// spiRxFifoIsr -
//
void
spiRxFifoIsr(void)
{
	Uint16 i;
	for(i=0;i<8;i++)
	{
		rdata[i]=SpiaRegs.SPIRXBUF;		// Read data
	}

	for(i=0;i<8;i++)                    // Check received data
	{
		if(rdata[i] != rdata_point+i)
		{
			error();
		}
	}
	rdata_point++;
	SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
}

//
// End of File
//

