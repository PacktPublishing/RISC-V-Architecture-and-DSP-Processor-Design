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

typedef   float    _iq;

// Typedef structure of the FREQCAL Object
typedef struct
{
	// Scaler converting 1/N cycles to a GLOBAL_Q freq (Q0) independently with global Q
	Uint32 	freqScaler_pr;
	// Scaler converting 1/N cycles to a GLOBAL_Q freq (Q0) independently with global Q
	_iq 	freqScaler_fr;
	Uint32 	BaseFreq;        	// Parameter : Maximum Freq
	_iq 	freq_pr;             // Output :  Freq in per-unit using capture unit
	_iq 	freqhz_pr;       	 // Output: Freq in Hz, measured using Capture unit
	Uint32 	oldpos;
	_iq 	freq_fr;            // Output : Freq in per-unit using position counter
	_iq 	freqhz_fr; 	    	// Output: Freq in Hz, measured using Capture unit
	void 	(*init)();     		// Pointer to the init function
	void 	(*calc)();    		// Pointer to the calc function
} FREQCAL;


// Define for a QEP_handle
typedef FREQCAL *FREQCAL_handle;

#define  CPU_FRQ_100MHZ  0
#define  CPU_FRQ_150MHZ  1  // 150 MHz CPU Freq (30 MHz input freq) by DEFAULT
#define  _IQ(A)      	(A)
#define  _IQdiv(A,B)    ((float)(A) / (float)(B))
#define  _IQmpy(A,B)    ((A) * (B))

#if (CPU_FRQ_150MHZ)
#define CPU_CLK   150e6
#endif
#if (CPU_FRQ_100MHZ)
#define CPU_CLK   100e6
#endif
#define 	PWM_CLK   5e3     		// If a diff frequency is desired, change freq here
#define 	SP        CPU_CLK/(2*PWM_CLK)
#define 	TBCTLVAL  0x200E       	// Up-down cnt, timebase = SYSCLKOUT


// Default initializer for the FREQCAL Object
#if (CPU_FRQ_150MHZ)
#define FREQCAL_DEFAULTS {\
		234,200,10000,0,0,\
		0,0,0,\
		(void (*)(long))FREQCAL_Init,\
		(void (*)(long))FREQCAL_Calc }
#endif
#if (CPU_FRQ_100MHZ)
#define FREQCAL_DEFAULTS {\
		313,200,10000,0,0,\
		0,0,0,\
		(void (*)(long))FREQCAL_Init,\
		(void (*)(long))FREQCAL_Calc }
#endif


// Function Prototypes
void FREQCAL_Calc(FREQCAL *p);
void FREQCAL_Init(void);
void EPwmSetup(void);
void prdTick(void);
void success();

// Globals
FREQCAL freq=FREQCAL_DEFAULTS;

int main(void)
{
	// Initialize trap
	 trap_init();
	 init_plic();//init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/

	// Only init the GPIO for EQep1 and EPwm1 in this case
	InitEQep1Gpio();
	InitEPwm1Gpio();

	IntVectTable.st.EPWM1_INT= &prdTick;
	enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM1_INT));

	// Example specific ePWM setup.
	EPwmSetup();

	// Initializes eQEP for frequency calculation in FREQCAL_Init(void)function
	freq.init(&freq);

	for(;;)
	{

	}

	return 0;
}


// FREQCAL_Init -
void FREQCAL_Init(void)
{
#if (CPU_FRQ_150MHZ)
	EQep1Regs.QUPRD=1500000;	// Unit Timer for 100Hz at 150MHz SYSCLKOUT
#endif
#if (CPU_FRQ_100MHZ)
	EQep1Regs.QUPRD=1000000;	// Unit Timer for 100Hz at 100MHz SYSCLKOUT
#endif

	EQep1Regs.QDECCTL.bit.QSRC=2;		// Up count mode (freq. measurement)
	EQep1Regs.QDECCTL.bit.XCR=0;

	EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
	EQep1Regs.QEPCTL.bit.PCRM=00;		// QPOSCNT reset on index evnt
	EQep1Regs.QEPCTL.bit.UTE=1; 		// Unit Timer Enable
	EQep1Regs.QEPCTL.bit.QCLM=1; 		// Latch on unit time out
	EQep1Regs.QPOSMAX=0xffffffff;
	EQep1Regs.QEPCTL.bit.QPEN=1; 		// QEP enable

#if (CPU_FRQ_150MHZ)
	EQep1Regs.QCAPCTL.bit.UPPS=2;  // 1/4 for unit position at 150MHz SYSCLKOUT
#endif
#if (CPU_FRQ_100MHZ)
	EQep1Regs.QCAPCTL.bit.UPPS=3;  // 1/8 for unit position at 100MHz SYSCLKOUT
#endif

	EQep1Regs.QCAPCTL.bit.CCPS=7;		// 1/128 for CAP clock
	EQep1Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable
}

//
// FREQCAL_Calc -
//
void FREQCAL_Calc(FREQCAL *p)
{
	float 	tmp;
	_iq 	newp,oldp;

	// Freq Calculation using QEP position counter
	// Check unit Time out-event for speed calculation

	// Unit Timer is configured for 100Hz in INIT function

	if(EQep1Regs.QFLG.bit.UTO==1)  // Unit Timeout event
	    {
	        //
	        // Differentiator
	        //
	        newp=EQep1Regs.QPOSLAT;         // Latched POSCNT value
	        oldp=p->oldpos;

	        if (newp>oldp)
	        {
	            tmp = newp - oldp;          // x2-x1 in v=(x2-x1)/T equation
	        }
	        else
	        {
	            tmp = (0xFFFFFFFF-oldp)+newp;
	        }

	        //
	        // p->freq_fr = (x2-x1)/(T*10KHz)
	        //
	        p->freq_fr = _IQdiv(tmp,p->freqScaler_fr);
	        tmp=p->freq_fr;

	        if (tmp>=_IQ(1))
	        {
	            p->freq_fr = _IQ(1);
	        }
	        else
	        {
	            p->freq_fr = tmp;
	        }

	        //
	        // Q0 = Q0*GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
	        // p->freqhz_fr = (p->freq_fr)*10kHz = (x2-x1)/T
	        //
	        p->freqhz_fr = _IQmpy(p->BaseFreq,p->freq_fr);

	        //
	        // Update position counter
	        //
	        p->oldpos = newp;

	        EQep1Regs.QCLR.bit.UTO=1;       // Clear interrupt flag
	    }

	    //
	    // Freq Calculation using QEP capture counter
	    //
	    if(EQep1Regs.QEPSTS.bit.UPEVNT==1)        // Unit Position Event
	    {
	        if(EQep1Regs.QEPSTS.bit.COEF==0)      // No Capture overflow
	        {
	            tmp=(unsigned long)EQep1Regs.QCPRDLAT;
	        }
	        else                // Capture overflow, saturate the result
	        {
	            tmp=0xFFFF;
	        }

	        //
	        // p->freq_pr = X/[(t2-t1)*10KHz]
	        //
	        p->freq_pr = _IQdiv(p->freqScaler_pr,tmp);
	        tmp=p->freq_pr;

	        if (tmp>_IQ(1))
	        {
	            p->freq_pr = _IQ(1);
	        }
	        else
	        {
	            p->freq_pr = tmp;
	        }

	        //
	        // Q0 = Q0*GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
	        // p->freqhz_pr =( p->freq_pr)*10kHz = X/(t2-t1)
	        //
	        p->freqhz_pr = _IQmpy(p->BaseFreq,p->freq_pr);

	        //
	        // Clear Unit position event flag
	        // Clear overflow error flag
	        //
	        EQep1Regs.QEPSTS.all=0x88;
	}


}


// prdTick - Interrupts once per ePWM period
void prdTick(void)
{
	// Checks for event and calculates frequency in FREQCAL_Calc(FREQCAL *p)
	freq.calc(&freq);

	// Acknowledge this interrupt to receive more interrupts from group 1
	//  PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
	EPwm1Regs.ETCLR.bit.INT=1;
}

// EPwmSetup
void  EPwmSetup()
{
//	InitEPwm1Gpio();
	EPwm1Regs.TBSTS.all=0;
	EPwm1Regs.TBPHS.half.TBPHS=0;
	EPwm1Regs.TBCTR=0;

	EPwm1Regs.CMPCTL.all=0x50;        // Immediate mode for CMPA and CMPB
	EPwm1Regs.CMPA.half.CMPA =SP/2;
	EPwm1Regs.CMPB=0;

	EPwm1Regs.AQCTLA.all=0x60;    // EPWMxA = 1 when CTR=CMPA and counter inc
	// EPWMxA = 0 when CTR=CMPA and counter dec
	EPwm1Regs.AQCTLB.all=0;
	EPwm1Regs.AQSFRC.all=0;
	EPwm1Regs.AQCSFRC.all=0;

	EPwm1Regs.DBCTL.all=0xb;      // EPWMxB is inverted
	EPwm1Regs.DBRED=0;
	EPwm1Regs.DBFED=0;

	EPwm1Regs.TZSEL.all=0;
	EPwm1Regs.TZCTL.all=0;
	EPwm1Regs.TZEINT.all=0;
	EPwm1Regs.TZFLG.all=0;
	EPwm1Regs.TZCLR.all=0;
	EPwm1Regs.TZFRC.all=0;

	EPwm1Regs.ETSEL.all=9;       // Interrupt when TBCTR = 0x0000
	EPwm1Regs.ETPS.all=1;	     // Interrupt on first event
	EPwm1Regs.ETFLG.all=0;
	EPwm1Regs.ETCLR.all=0;
	EPwm1Regs.ETFRC.all=0;

	EPwm1Regs.PCCTL.all=0;

	EPwm1Regs.TBCTL.all=0x0010+TBCTLVAL;			// Enable Timer
	EPwm1Regs.TBPRD=SP;
}

