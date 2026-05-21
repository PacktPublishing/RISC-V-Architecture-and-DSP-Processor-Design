//###########################################################################
//
// FILE:   RV335_DefaultIsr.c
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
// INT13_ISR - Connected to INT13 of CPU (use MINT13 mask):
// Note CPU-Timer1 is reserved for TI use, however XINT13
// ISR can be used by the user.
//
interrupt void
INT13_ISR(void)     // INT13 or CPU-Timer1
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// INT14_ISR - Note CPU-Timer2 is reserved for TI use.
//
interrupt void
INT14_ISR(void)     // CPU-Timer2
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// DATALOG_ISR - Datalogging interrupt 
//
interrupt void
DATALOG_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// RTOSINT_ISR - RTOS interrupt
//
interrupt void
RTOSINT_ISR(void)   
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// EMUINT_ISR - Emulation interrupt
//
interrupt void 
EMUINT_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// NMI_ISR - Non-maskable interrupt
//
interrupt void 
NMI_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// ILLEGAL_ISR - Illegal operation TRAP
//
interrupt void 
ILLEGAL_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// USER1_ISR - User Defined trap 1
//
interrupt void 
USER1_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// USER2_ISR - User Defined trap 2
//
interrupt void 
USER2_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// USER3_ISR - User Defined trap 3
//
interrupt void 
USER3_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

	ESTOP0;

    for(;;);
}

//
// USER4_ISR - User Defined trap 4
//
interrupt void 
USER4_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

	ESTOP0;

    for(;;);
}

//
// USER5_ISR - User Defined trap 5
//
interrupt void 
USER5_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //

    ESTOP0;

    for(;;);
}

//
// USER6_ISR - User Defined trap 6
//
interrupt void 
USER6_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //


    ESTOP0;



    for(;;);
}

//
// USER7_ISR - User Defined trap 7
//
interrupt void 
USER7_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //


    ESTOP0;



    for(;;);
}

//
// USER8_ISR - User Defined trap 8
//
interrupt void 
USER8_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //


    ESTOP0;



    for(;;);
}

//
// USER9_ISR - User Defined trap 9
//
interrupt void 
USER9_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //


    ESTOP0;



    for(;;);
}

//
// USER10_ISR - User Defined trap 10
//
interrupt void 
USER10_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //


    ESTOP0;



    for(;;);
}

//
// USER11_ISR - User Defined trap 11
//
interrupt void 
USER11_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //


    ESTOP0;



    for(;;);
}

//
// USER12_ISR - User Defined trap 12
//
interrupt void 
USER12_ISR(void)
{
    //
    // Insert ISR Code here
    //

    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //


    ESTOP0;



    for(;;);
}


//
// rsvd_ISR - For test
//
interrupt void 
rsvd_ISR(void)
{
	ESTOP0;

    for(;;);
}

//
// End of file
//

