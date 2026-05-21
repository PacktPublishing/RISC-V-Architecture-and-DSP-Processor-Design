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
#include "../routines/include/RV335_Dma_defines.h"
//
// Defines
// Sample buffer size
//
#define BUF_SIZE   1024  

//
// DMA Defines
//
#define CH1_TOTAL               DATA_POINTS_PER_CHANNEL
#define CH1_WORDS_PER_BURST     ADC_CHANNELS_TO_CONVERT

#pragma DATA_SECTION(DMABuf1,"DMARAML4");
#pragma DATA_SECTION(DMABuf2,"DMARAML5");

volatile Uint16 DMABuf1[1024];
volatile Uint16 DMABuf2[1024];

volatile Uint16 *DMADest;
volatile Uint16 *DMASource;

void local_DINTCH1_ISR(void);

void main(void)
{


    // Initialize trap
    trap_init();
    init_plic();
    //init_priority();


    // Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
   InitSysCtrl();

   IntVectTable.st.DINTCH1 = &local_DINTCH1_ISR;
   enable_interrupt( GET_INT_NUM(IntVectTable.st.DINTCH1));
   CpuTimer0Regs.TCR.bit.TSS  = 1;       //Stop Timer0 for now

    //
    // Initialize DMA
    //
    DMAInitialize();

    //
    // Initialize Tables
    //
    Uint16 i;
    for (i=0; i<BUF_SIZE; i++)
    {
        DMABuf1[i] = 0;
        DMABuf2[i] = i;
    }

    //
    // Configure DMA Channel
    //
    DMADest   = &DMABuf1[0];
    DMASource = &DMABuf2[0];
    DMACH1AddrConfig(DMADest,DMASource);
    
    //
    // Will set up to use 32-bit datasize, pointers are based on 16-bit words
    //
    DMACH1BurstConfig(31,2,2);
    
    //
    // so need to increment by 2 to grab the correct location
    //
    DMACH1TransferConfig(31,2,2);
    DMACH1WrapConfig(0xFFFF,0,0xFFFF,0);
    
    //
    // Use timer0 to start the x-fer.
    // Since this is a static copy use one shot mode, so only one trigger 
    // is needed. Also using 32-bit mode to decrease x-fer time
    //
    DMACH1ModeConfig(DMA_TINT0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_DISABLE,
                     SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,THIRTYTWO_BIT,
                     CHINT_END,CHINT_ENABLE);

    StartDMACH1();

    //
    // Init the timer 0
    //
    
    //
    // load low value so we can start the DMA quickly
    //
    CpuTimer0Regs.TIM.half.LSW = 512;
    CpuTimer0Regs.TCR.bit.SOFT = 1;      //Allow to free run even if halted
    CpuTimer0Regs.TCR.bit.FREE = 1;
    CpuTimer0Regs.TCR.bit.TIE  = 1;      //Enable the timer0 interrupt signal
    CpuTimer0Regs.TCR.bit.TSS  = 0;      //restart the timer 0

    for(;;)
    {
    }

}

//
// local_DINTCH1_ISR - INT7.1(DMA Channel 1)
//
void 
local_DINTCH1_ISR(void)
{
    //
    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    //
    __asm ("      EBREAK");
    for(;;);
}
