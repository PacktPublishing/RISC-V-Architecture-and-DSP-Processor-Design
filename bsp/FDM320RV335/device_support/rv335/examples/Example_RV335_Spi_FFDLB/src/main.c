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
//__interrupt void ISRTimer2(void);
void spi_xmit(Uint16 a);
void spi_fifo_init(void);
void spi_init(void);
void error(void);

int main(void)
{
	// Initialize trap
	trap_init();
	init_plic(); //init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/


    Uint16 sdata;  // send data
    Uint16 rdata;  // received data


    //
    // Setup only the GP I/O only for SPI-A functionality
    // This function is found in RV335_Spi.c
    //
    InitSpiaGpio();

    //
    // Step 4. Initialize all the Device Peripherals:
    // This function is found in RV335_InitPeripherals.c
    //
    // InitPeripherals();    // Not required for this example
    spi_fifo_init();	     // Initialize the Spi FIFO
    spi_init();		         // init SPI

    //
    // Step 5. User specific code:
    //

    //
    // Interrupts are not used in this example.
    //
    sdata = 0x0000;
    for(;;)
    {
        //
        // Transmit data
        //
        spi_xmit(sdata);

        //
        // Wait until data is received
        //
        while(SpiaRegs.SPIFFRX.bit.RXFFST !=1)
        {

        }

        //
        // Check against sent data
        //
        rdata = SpiaRegs.SPIRXBUF;

        if(rdata != sdata)
        {
            error();
        }
        sdata++;
    }
}


//
// error -
//
void
error(void)
{
    for (;;);
}

//
// spi_init -
//
void
spi_init()
{
    SpiaRegs.SPICCR.all =0x000F;	// Reset on, rising edge, 16-bit char bits

    //
    // Enable master mode, normal phase, enable talk, and SPI int disabled.
    //
    SpiaRegs.SPICTL.all =0x0006;
    SpiaRegs.SPIBRR =0x007F;
    SpiaRegs.SPICCR.all =0x009F;   // Relinquish SPI from Reset
    SpiaRegs.SPIPRI.bit.FREE = 1;  // Set so breakpoints don't disturb xmission
}

//
// spi_xmit -
//
void
spi_xmit(Uint16 a)
{
    SpiaRegs.SPITXBUF=a;
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
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;
}

//
// End of File
//

