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

//
// Function Prototype statements
//
void local_delay_loop(void);
void LocalDMAInitialize(void);
void DMACH1Config(void);
void DMACH2Config(void);
void DMACH3Config(void);
void ConfigAdc(void);
void config_ePWM1_to_generate_ADCSOCA(void);
void config_ePWM2_to_generate_ADCSOCB(void);
void local_DINTCH1_ISR(void);

//
// Globals
//
#pragma DATA_SECTION(ADCbuffer,"DMARAML4");
volatile Uint32 ADCbuffer[3];

Uint16 VarA;
Uint32 VarB;

volatile Uint16 *MAPCNF = (Uint16 *)0x00702E;

Uint16 InterruptCount;

//
// Main
//
void main(void)
{
    Uint16 i;

    // Initialize trap
    trap_init();
    init_plic();
    //init_priority();

    // Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks.
    InitSysCtrl();

    //
    // Init IntVectTable inpterrupt sources
    //
    IntVectTable.st.DINTCH1 = &local_DINTCH1_ISR;	// Point to DMA CH1 ISR

    //
    // Enable interrupt sources
    //
    enable_interrupt( GET_INT_NUM(IntVectTable.st.DINTCH1));
    enable_interrupt( GET_INT_NUM(IntVectTable.st.DINTCH3));
    //
    // User specific code:
    //
    InterruptCount = 0;

    GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;    // All outputs
//    SysCtrlRegs.MAPCNF.bit.MAPEPWM = 1;      // Remap ePWMs for DMA access
//
    GpioDataRegs.GPASET.all = 0xFFFFFFFF;
    delay_loop();
    GpioDataRegs.GPACLEAR.all = 0x00000002;

    for(i=0; i<3; i++)
    {
        ADCbuffer[i] = ((Uint32)i*0x00011000) + 0x00044000;
    }

    VarA = 75;
    VarB = 0x652000;

    //
    // Enable and configure clocks to peripherals:
    //
    SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;  // Enable SYSCLK to DMA

    DMAInitialize();
    DMACH1Config();
    DMACH2Config();
    DMACH3Config();

    //
    // configure adc:
    //
    InitAdc();
    ConfigAdc();

    config_ePWM1_to_generate_ADCSOCA();
    config_ePWM2_to_generate_ADCSOCB();

    DmaRegs.CH1.CONTROL.bit.RUN = 1;
    DmaRegs.CH2.CONTROL.bit.RUN = 1;
    DmaRegs.CH3.CONTROL.bit.RUN = 1;
    __asm("   NOP");
    EPwm1Regs.TBCTL.bit.CTRMODE = 0;						// Up count mode
    EPwm2Regs.TBCTL.bit.CTRMODE = 0;						// Up count mode

    for(;;) 
    {
        
    }
}

//
// DMA Functions
//
void 
LocalDMAInitialize(void)
{
    //
	// Perform a hard reset on DMA
    //
	DmaRegs.DMACTRL.bit.HARDRESET = 1;

    //
	// always perform one NOP after a HARDRESET
    //
	__asm("     NOP");

    //
	// Stop DMA on emulation suspend
    //
	DmaRegs.DEBUGCTRL.bit.FREE = 0;

}

//
// DMACH1Config -
//
void 
DMACH1Config(void)
{
    //
    // Configure CH1:
    //
    
    //
    // Reset selected channel via CONTROL Register:
    //
    
    //
    // Perform SOFT reset on channel (clears all counters)
    //
    //DmaRegs.CH1.CONTROL.bit.SOFTRESET = 1;

    //
    // Set up MODE Register:
    //
    
    //
    // ePWM1 SOCA as peripheral interrupt source
    //
    DmaRegs.CH1.MODE.bit.PERINTSEL = 18;
    
    DmaRegs.CH1.MODE.bit.PERINTE = 1;       // Peripheral interrupt enabled
    DmaRegs.CH1.MODE.bit.ONESHOT = 0;       // 1 burst per SW interrupt
    DmaRegs.CH1.MODE.bit.CONTINUOUS = 1;    // Do not stop after each transfer
    DmaRegs.CH1.MODE.bit.SYNCE = 0;         // No sync signal
    DmaRegs.CH1.MODE.bit.SYNCSEL = 0;       // No sync signal
    DmaRegs.CH1.MODE.bit.DATASIZE = 0;		// 16-bit data size transfers
    
    //
    // Generate interrupt to CPU at the beg of transfer
    //
    DmaRegs.CH1.MODE.bit.CHINTMODE = 0;
    
    DmaRegs.CH1.MODE.bit.CHINTE = 1;        // Channel Interrupt to CPU enabled

    //
    // Set up BURST registers
    //
    
    //
    // Number (N-1) of 16-bit words transferred in a burst
    //
    DmaRegs.CH1.BURST_SIZE.all = 0;
    DmaRegs.CH1.SRC_BURST_STEP = 0;			// Not needed since BURST_SIZE = 0
    DmaRegs.CH1.DST_BURST_STEP = 0;			// Not needed since BURST_SIZE = 0

    //
    // Set up TRANSFER registers
    //
    DmaRegs.CH1.TRANSFER_SIZE = 0;		// Bursts (N-1) per transfer
    DmaRegs.CH1.SRC_TRANSFER_STEP = 0;	// Not needed since TRANSFER_SIZE = 0
    DmaRegs.CH1.DST_TRANSFER_STEP = 0;  // Not needed since TRANSFER_SIZE = 0

    //
    // Set up WRAP registers
    //
    DmaRegs.CH1.SRC_WRAP_SIZE = 0xFFFF;	// No source wrap-around
    DmaRegs.CH1.DST_WRAP_SIZE = 0xFFFF;	// No destination wrap-around
    DmaRegs.CH1.SRC_WRAP_STEP = 0;
    DmaRegs.CH1.DST_WRAP_STEP = 0;

    //
    // Set up SOURCE address
    //
    DmaRegs.CH1.SRC_ADDR_SHADOW = (Uint32) &VarA;	// Point to variable in RAM

    //
    // Set up DESTINATION address
    //
    
    //
    // Point to ePWM1 TBPRD register remapped for DMA need to make sure 
    // .cmd file has ePWMs remapped
    //
    DmaRegs.CH1.DST_ADDR_SHADOW = (Uint32) &EPwm1Regs.TBPRD;
    
    //
    // Clear any spurious flags
    //
    DmaRegs.CH1.CONTROL.bit.PERINTCLR = 1; //Clear any spurious interrupt flags
    DmaRegs.CH1.CONTROL.bit.SYNCCLR = 1;    // Clear any spurious sync flags
    DmaRegs.CH1.CONTROL.bit.ERRCLR = 1;   //Clear any spurious sync error flags

}

//
// DMACH2Config -
//
void 
DMACH2Config(void)
{

    //
    // Configure CH2
    //
    
    //
    // Reset selected channel via CONTROL Register
    //
    
    //
    // Perform SOFT reset on channel (clears all counters)
    //
    //DmaRegs.CH2.CONTROL.bit.SOFTRESET = 1;

    //
    // Set up MODE Register
    //
    
    //
    // ePWM2 SOCB as peripheral interrupt source
    //
    DmaRegs.CH2.MODE.bit.PERINTSEL = 21;
    
    DmaRegs.CH2.MODE.bit.PERINTE = 1;    	// Peripheral interrupt enabled
    DmaRegs.CH2.MODE.bit.ONESHOT = 0;    	// 1 burst per SW interrupt
    DmaRegs.CH2.MODE.bit.CONTINUOUS = 1; 	// Do not stop after each transfer
    DmaRegs.CH2.MODE.bit.SYNCE = 0;      	// No sync signal
    DmaRegs.CH2.MODE.bit.SYNCSEL = 0;    	// No sync signal
    DmaRegs.CH2.MODE.bit.DATASIZE = 1;		// 32-bit data size transfers
    DmaRegs.CH2.MODE.bit.CHINTMODE = 0;
    DmaRegs.CH2.MODE.bit.CHINTE = 0;    // Channel Interrupt to CPU disabled

    //
    // Set up BURST registers
    //
    
    //
    // Number (N-1) of 16-bit words transferred in a burst
    //
    DmaRegs.CH2.BURST_SIZE.all = 1;
    
    //
    // Not needed since only 1 32-bit move per burst
    //
    DmaRegs.CH2.SRC_BURST_STEP = 0x0000;
    
    //
    // Not needed since only 1 32-bit move per burst
    //
    DmaRegs.CH2.DST_BURST_STEP = 0x0000;

    //
    // Set up TRANSFER registers
    //
    DmaRegs.CH2.TRANSFER_SIZE = 0;		// Bursts (N-1) per transfer
    DmaRegs.CH2.SRC_TRANSFER_STEP = 0;	// Not needed since TRANSFER_SIZE = 0
    DmaRegs.CH2.DST_TRANSFER_STEP = 0; 	// Not needed since TRANSFER_SIZE = 0

    //
    // Set up WRAP registers
    //
    DmaRegs.CH2.SRC_WRAP_SIZE = 0xFFFF;	// No source wrap-around
    DmaRegs.CH2.DST_WRAP_SIZE = 0xFFFF;	// No destination wrap-around
    DmaRegs.CH2.SRC_WRAP_STEP = 0;
    DmaRegs.CH2.DST_WRAP_STEP = 0;

    //
    // Set up SOURCE address
    //
    DmaRegs.CH2.SRC_ADDR_SHADOW = (Uint32) &VarB;	// Point to variable in RAM

    //
    // Set up DESTINATION address
    //
    
    //
    // Point to ePWM1 CMPAHR/CMPA registers
    //
    DmaRegs.CH2.DST_ADDR_SHADOW = (Uint32) &EPwm1Regs.CMPA.all;

    //
    // Clear any spurious flags
    //
    DmaRegs.CH2.CONTROL.bit.PERINTCLR = 1; //Clear any spurious interrupt flags
    DmaRegs.CH2.CONTROL.bit.SYNCCLR = 1; 	//Clear any spurious sync flags
    DmaRegs.CH2.CONTROL.bit.ERRCLR = 1;   //Clear any spurious sync error flags

}

void DMACH3Config(void)
{

    //
    // Configure CH3
    //

    //
    // Set up MODE Register
    //

    //
    // ADC SEQ1INT as peripheral interrupt source
    //
    DmaRegs.CH3.MODE.bit.PERINTSEL = 1;
    
    DmaRegs.CH3.MODE.bit.PERINTE = 1;      // Peripheral interrupt enabled
    DmaRegs.CH3.MODE.bit.ONESHOT = 0;      // 1 burst per SW interrupt
    DmaRegs.CH3.MODE.bit.CONTINUOUS = 1;   // Do not stop after each transfer
    DmaRegs.CH3.MODE.bit.SYNCE = 0;        // No sync signal
    DmaRegs.CH3.MODE.bit.SYNCSEL = 0;      // No sync signal
    DmaRegs.CH3.MODE.bit.DATASIZE = 1;	   // 32-bit data size transfers
    DmaRegs.CH3.MODE.bit.CHINTMODE = 0;
    DmaRegs.CH3.MODE.bit.CHINTE = 0;       // Channel Interrupt to CPU disabled

    //
    // Set up BURST registers
    //
    
    //
    // Number (N-1) of 16-bit words transferred in a burst
    //
    DmaRegs.CH3.BURST_SIZE.all = 5;
    
    //
    // Increment source burst address by 2 (32-bit)
    //
    DmaRegs.CH3.SRC_BURST_STEP = 2;
    
    //
    // Increment destination burst address by 2 (32-bit)
    //
    DmaRegs.CH3.DST_BURST_STEP = 2;

    //
    // Set up TRANSFER registers
    //
    DmaRegs.CH3.TRANSFER_SIZE = 0;		// Bursts (N-1) per transfer
    DmaRegs.CH3.SRC_TRANSFER_STEP = 0;	// Not needed since TRANSFER_SIZE = 0
    DmaRegs.CH3.DST_TRANSFER_STEP = 0; 	// Not needed since TRANSFER_SIZE = 0

    //
    // Set up WRAP registers
    //
    DmaRegs.CH3.SRC_WRAP_SIZE = 0xFFFF;		// No source wrap-around
    DmaRegs.CH3.DST_WRAP_SIZE = 0xFFFF;		// No destination wrap-around
    DmaRegs.CH3.SRC_WRAP_STEP = 0;
    DmaRegs.CH3.DST_WRAP_STEP = 0;

    //
    // Set up SOURCE address
    //
    
    //
    // Point to first RESULT reg
    //
    DmaRegs.CH3.SRC_ADDR_SHADOW = (Uint32) &AdcMirror.ADCRESULT0;

    //
    // Set up DESTINATION address
    //
    
    //
    // Point to beginning of ADCbuffer
    //
    DmaRegs.CH3.DST_ADDR_SHADOW = (Uint32) &ADCbuffer[0];

    //
    // Clear any spurious flags
    //
    DmaRegs.CH3.CONTROL.bit.PERINTCLR = 1; //Clear any spurious interrupt flags
    DmaRegs.CH3.CONTROL.bit.SYNCCLR = 1;   //Clear any spurious sync flags
    DmaRegs.CH3.CONTROL.bit.ERRCLR = 1;   //Clear any spurious sync error flags

}

//
// local_DINTCH1_ISR - 
//
void 
local_DINTCH1_ISR(void)	// DMA INT7.1
{
    GpioDataRegs.GPATOGGLE.all = 0x00000001;		// Toggle GPIOA0

    InterruptCount++;

    if((DmaRegs.CH1.CONTROL.bit.OVRFLG == 1) ||
    (DmaRegs.CH2.CONTROL.bit.OVRFLG == 1) ||
    (DmaRegs.CH3.CONTROL.bit.OVRFLG == 1))
    {
        __asm("     ebreak");
    }
}

//
// ConfigAdc - 
//
void 
ConfigAdc(void)
{
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 7;
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0;		// ADCINA0
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1;		// ADCINA1
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2;		// ADCINA2
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3;		// ADCINA3
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4;		// ADCINA4
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 5;		// ADCINA5
    
    //
    // Enable ADC to accept ePWM_SOCA trigger
    //
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;
    
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;			// Clear interrupt flag
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;		// Enable SEQ1 interrupt
}

//
// config_ePWM1_to_generate_ADCSOCA -
//
void 
config_ePWM1_to_generate_ADCSOCA(void)
{
    //
    // Configure ePWM1 Timer
    // Interrupt triggers ADCSOCA
    //
    
    //
    // Setup period (one off so DMA transfer will be obvious)
    //
    EPwm1Regs.TBPRD = 74;
    
    EPwm1Regs.CMPA.all = 0x501000;
    EPwm1Regs.ETSEL.bit.SOCASEL = 2;			// ADCSOCA on TBCTR=TBPRD
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;				// Generate SOCA on 1st event
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;				// Enable SOCA generation
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;			// /1 clock mode
}

//
// config_ePWM2_to_generate_ADCSOCB -
//
void 
config_ePWM2_to_generate_ADCSOCB(void)
{
    //
    // Configure ePWM2 Timer
    // Interrupt triggers ADCSOCB
    //

    EPwm2Regs.TBPRD = 150;						// Setup periodSetup period
    EPwm2Regs.CMPA.all = 0x200000;
    EPwm2Regs.ETSEL.bit.SOCBSEL = 2;			// ADCSOCB on TBCTR=TBPRD
    EPwm2Regs.ETPS.bit.SOCBPRD = 1;				// Generate SOCB on 1st event
    EPwm2Regs.ETSEL.bit.SOCBEN = 1;				// Enable SOCB generation
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;			// /1 clock mode

}

void local_delay_loop()
{
    short      i;
    for (i = 0; i < 1000; i++) 
    {
        
    }
}

//
// End of File
//

