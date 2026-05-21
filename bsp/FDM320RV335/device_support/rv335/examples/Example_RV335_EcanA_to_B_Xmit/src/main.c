#include "../include/RV335_Project.h"     // Device Headerfile and Examples Include File
#include "../include/RV335_SysCtrl.h"
#include "../routines/include/RV335_Device.h"
#include "../routines/include/RV335_Examples.h"

//
// Defines
//
#define TXCOUNT  100            // Transmission will take place (TXCOUNT) times

//
// Globals
//
long      i;
long 	  loopcount = 0;


int main(void)
{
	// Initialize trap
	trap_init();
	init_plic();//init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/

//
	// Create a shadow register structure for the CAN control registers.
	// This is needed, since only 32-bit access is allowed to these registers.
	// 16-bit access to these registers could potentially corrupt the register
	// contents or return false data.
	//
	struct ECAN_REGS ECanaShadow;

	//
	// Just initialize eCAN pins for this example
	// This function is in RV335_ECan.c
	//
	InitECanGpio();

	//
	// In this case just initialize eCAN-A and eCAN-B
	// This function is in RV335_ECan.c
	//
	InitECan();

	//
	// User specific code
	//

	//
	// Write to the MSGID field
	//
	ECanaMboxes.MBOX25.MSGID.all = 0x95555555;          // Extended Identifier

	//
	// Configure Mailbox under test as a Transmit mailbox
	//
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANMD.bit.MD25 = 0;
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

	//
	// Enable Mailbox under test
	//
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME25 = 1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;

	//
	// Write to DLC field in Master Control reg
	//
	ECanaMboxes.MBOX25.MSGCTRL.bit.DLC = 8;

	//
	// Write to the mailbox RAM field
	//
	ECanaMboxes.MBOX25.MDL.all = 0x55555555;
	ECanaMboxes.MBOX25.MDH.all = 0x55555555;

	//
	// Begin transmitting
	//
	for(i=0; i < TXCOUNT; i++)
	{
		ECanaShadow.CANTRS.all = 0;
		ECanaShadow.CANTRS.bit.TRS25 = 1;     // Set TRS for mailbox under test
		ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

		do
		{
			ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
		} while(ECanaShadow.CANTA.bit.TA25 == 0 );// Wait for TA5 bit to be set

		ECanaShadow.CANTA.all = 0;
		ECanaShadow.CANTA.bit.TA25 = 1;     	         // Clear TA5
		ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;

		loopcount ++;
	}

	for(;;){}
	return 0;
}

//
// End of File
//
