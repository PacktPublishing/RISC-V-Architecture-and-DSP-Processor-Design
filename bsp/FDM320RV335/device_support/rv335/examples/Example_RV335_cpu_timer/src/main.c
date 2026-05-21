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

// Device Headerfile and Examples Include File
#include "../include/RV335_Project.h"
#include "../include/RV335_SysCtrl.h"
#include "../routines/include/RV335_Device.h"

void ISRTimer0(void);
void ISRTimer1(void);

Uint32 count=0,count1=0;
Uint32 SumTime=0, Time=0;

void main(void)
{


    // Initialize trap
    trap_init();
    init_plic();
    //init_priority();

    // Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2837xS_SysCtrl.c file.
   InitSysCtrl();

    // Clear all __interrupts and initialize PIE vector table:
    // Disable CPU __interrupts
   DINT;

   IntVectTable.st.TINT0 = &ISRTimer0;
   IntVectTable.st.TIMER1 = &ISRTimer1;

   enable_interrupt( GET_INT_NUM(IntVectTable.st.TINT0));
   enable_interrupt( GET_INT_NUM(IntVectTable.st.TIMER1));

   EDIS;    // This is needed to disable write to EALLOW protected registers
    
    // Initialize all the Device Peripherals:
    // Example specific timer setup.

   InitCpuTimers();
   ConfigCpuTimer(&CpuTimer0, 150, 10);
   ConfigCpuTimer(&CpuTimer1, 150, 50);


   CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
   CpuTimer1Regs.TCR.all = 0x4000;

   // Enable global Interrupts and higher priority real-time debug events:
   //
      EINT;   // Enable Global __interrupt INTM

    for(;;)
    {
    }

}

int count2 = 0;
void ISRTimer0(void)
{   
    CpuTimer0.InterruptCount++;

}


void ISRTimer1(void)
{
   CpuTimer1.InterruptCount++;

}

