#include "RV335_Project.h"     // Device Headerfile and Examples Include File
//#############################################################################
//! \file $FOLDER_TEST$/$DEVICE$_FixedPoint_IIR32/test_iir32d.c
//!
//! \brief  32-bit IIR Filter Example
//!
//! \date   Aug 12, 2014
//!
//! This examples shows how to implement a 32-bit IIR filter using the optimized
//! IIR library routines. The IIR structure is as follows:
//!
//! typedef struct {
//!   void (*init)(void *);
//!   void (*calc)(void *);
//!   long *coeff_ptr;
//!   long *dbuffer_ptr;
//!   long nbiq;
//!   long input;
//!   long isf;
//!   long output32;
//!   int output16;
//!   int qfmat;
//! }IIR5BIQ32;
//!
//! Users can also use default templates defined within filter.h to initialize
//! the IIR32 header structure.
//!
//! IIR32 iir= IIR32_DEFAULTS;
//!
//! The MATLAB Filter Design and Analysis Tool (FDATool) is a useful tool
//! for designing filters and generating fixed point coefficients. Refer
//! to the appendix of the Fixed Point DSP Libray User's Guide for more
//! information.
//! The eziir32.m MATLAB script ($FOLDER_EXAMPLES$/$DEVICE$_FixedPoint_IIR32/matlab/
//! eziir32.m) can be used to generate the filter coefficients in a way as to prevent
//! overflow of the output. Please refer to SPRA509 (Overflow Avoidance Techniques in
//! Cascaded IIR Filter Implementations on the TMS320 DSP's) for a better understanding
//! of what the process is. The script will generate a Q-format and Input Scaling Factor
//! for the coefficients, QFMAT and ISF respectively, that will be used in the IIR5BIQ32
//! object initialization.
//!
//! \b Watch \b Variables
//! - iir  IIR32 object
//!
//! In CCS, you can invoke the "SetupDebugEnv.js" script from the
//! Scripting console to have these variables set up in the Watch window
//! and create graphs to show the input/output vectors
//
//  Group:            RISCV
//  Target Family:    rv335
//
//#############################################################################
//
//
//$Copyright: Copyright (C) 2014-2023 FDMTEK -
//            http://www.ti.com/ ALL RIGHTS RESERVED $
//#############################################################################
#define XXSTRINGIZE(s)  XSTRINGIZE(s)  // calls xstringize(s), if s is a macro it
// will expand s first
#define XSTRINGIZE(s)   STRINGIZE(s)  // calls stringize(s), if s is a macro it
// will expand s first
#define STRINGIZE(s)    #s            // TREAT s as a literal, if s is a macro
// it wont be expanded
#define XCONCAT(x,y,z) CONCAT(x,y,z)  // expand x and y then call concat on the
// expanded macros x and y
#define CONCAT(x,y,z)   x ## y ## z   // concats x and y but doesnt expand
// x and y if they are macros
//*****************************************************************************
// includes
//*****************************************************************************
#include "filter.h"
#include "test_iir32d.h"

#if ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5))
#include XSTRINGIZE(XCONCAT(test,RUN_TEST,_coeffs.txt))
#endif
//*****************************************************************************
// defines
//*****************************************************************************

//*****************************************************************************
// globals
//*****************************************************************************
// Create an Instance of IIR5BIQD32 module and place the object in
// "iirfilt" section
#ifndef __cplusplus
#pragma DATA_SECTION(iir, "iirfilt");
#else
#pragma DATA_SECTION("iirfilt");
#endif
IIR5BIQ32  iir = IIR5BIQ32_DEFAULTS;

#if ((RUN_TEST == TEST_1) || (RUN_TEST == TEST_2))
int16_t temp[2*SIGNAL_LENGTH];
int32_t *dbuffer;
#elif ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5))
// Modify the delay buffer size to be commensurate with the number of biquads
// in the filter. Size of the Delay buffer = 2*nbiq i.e. two delay elements
// per biquad, each element being 32-bits wide.
// Define the Delay buffer for the cascaded 6 biquad IIR filter and place it
// in "iirfilt" section
#ifndef __cplusplus
#pragma DATA_SECTION(dbuffer, "iirldb");
#else
#pragma DATA_SECTION("iirldb");
#endif
int32_t dbuffer[2*IIR32_NBIQ + SIGNAL_LENGTH*2];

// Input/output buffer
#ifndef __cplusplus
#pragma DATA_SECTION(sigIn, "sigIn");
#else
#pragma DATA_SECTION("sigIn");
#endif
int32_t sigIn[SIGNAL_LENGTH] = {
#include XSTRINGIZE(XCONCAT(test,RUN_TEST,_data_input.txt))
};

#ifndef __cplusplus
#pragma DATA_SECTION(sigOut, "sigOut");
#else
#pragma DATA_SECTION("sigOut");
#endif
int32_t sigOut[SIGNAL_LENGTH];

const int32_t goldOut[SIGNAL_LENGTH] = {
#include XSTRINGIZE(XCONCAT(test,RUN_TEST,_data_output.txt))
};

// For each biquad there are 6 coefficients: a0, a1, a2, b0, b0, b2.
// a0 is always 1, therefore, we need 5 coefficients for each biquad
// xn------>(x)--->(+)--------------->yn
//      |    ^      ^             |
//      |    |      |q4           |
//      |    b(0)  (z)            |
//      |           ^             |
//      |           |q3           |
//      |-->(x)--->(+)<-----(x)---|
//      |    ^      ^        ^    |
//      |    |      |q2      |    |
//      |    b(1)  (z)       a(1) |
//      |           ^             |
//      |           |q1           |
//      --->(x)--->(+)<-----(x)----
//           ^               ^
//           |               |
//           b(2)            a(2)
// Size of the coefficient array = 5 * nbiq
const int32_t coeff[5*IIR32_NBIQ] = IIR32_COEFF;

// Error between computed output and MATLAB output
int32_t err[SIGNAL_LENGTH];
#endif

void FILTER_runIIR(IIR5BIQ32_Handle hndIIR32)
{
    uint16_t j ;
    int32_t xnn =  (hndIIR32->input);            //IIR_getInput(hnd);
    int32_t ynn = (int32_t)0.0;
    int32_t *coef = hndIIR32->coeff_ptr ;           // ϵ��
    int32_t *q = hndIIR32->dbuffer_ptr;   //!< Delay buffer Pointer
    uint16_t nbiq =  hndIIR32->nbiq;
    int32_t isf = hndIIR32->isf;
    uint16_t qfmat = hndIIR32->qfmat;

    xnn = ((int64_t)xnn * (int64_t)isf >> 32);
    for(j = 0; j < nbiq ; j++)
    {
        // a1 = coef[0] , a2 = coef[1] , b0 = coef[2] ,b1 = coef[3] , b2 = coef[4]
        volatile int a1 =coef[1];
        volatile int a2 =coef[0];
        volatile int a3 =q[1];
        volatile int a4 =q[2];
        xnn = xnn + (((int64_t)coef[1] * (int64_t)q[1] + (int64_t)coef[0] * (int64_t)q[2])>>32);
        q[0] =  xnn << (32-qfmat);
        ynn = 0;
        volatile int a =q[0];
        volatile int b =coef[4];
        ynn = ((int64_t)coef[4] * (int64_t)q[0]) + ((int64_t)coef[3]*(int64_t)q[1]) + ((int64_t)coef[2]*(int64_t)q[2])>>32;
       // d[2] = d[1];
        //q[2] = q[1];

       // d[1] = d[0];
        //q[1] = q[0];

        xnn  = (ynn) ;
        q = q - 3UL;
        coef = coef + 5UL;
    }
    hndIIR32->dbuffer_ptr--;
    // write the final output to the structure object
    hndIIR32->output32 = (ynn << (31-qfmat));
}
uint16_t pass = 0, fail = 0;

//*****************************************************************************
// function definitions
//*****************************************************************************
uint16_t FIXPT_DSP_runTest()
{
    //Locals
    int16_t i = 0;

#if(RUN_TEST == TEST_1)
    // Write dummy codes to temp
    memset(temp, 0xBAAD, sizeof(temp));
    // IIR Filter Initialization
    dbuffer          = (int32_t *)&temp[2*IIR32_NBIQ]; // Set dbuffer to an odd address
    iir.dbuffer_ptr  = dbuffer;
    iir.nbiq         = IIR32_NBIQ;
    iir.init(&iir);


#elif(RUN_TEST == TEST_2)
    // Write dummy codes to temp
    memset(temp, 0xBAAD, sizeof(temp));
    // IIR Filter Initialization
    dbuffer          = (int32_t *)&temp[2*IIR32_NBIQ+1]; // Set dbuffer to an odd address
    iir.dbuffer_ptr  = dbuffer;
    iir.nbiq         = IIR32_NBIQ;
    iir.init(&iir);

#elif ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5))
    // zero out sigOut
    memset(sigOut, 0, sizeof(sigOut));


    // IIR Filter Initialization
    iir.dbuffer_ptr  = (int32_t *)dbuffer+(2*IIR32_NBIQ + SIGNAL_LENGTH*2);
    iir.coeff_ptr    = (int32_t *)coeff;
    iir.qfmat        = IIR32_QFMAT;
    iir.nbiq         = IIR32_NBIQ;
    iir.isf          = IIR32_ISF;
    iir.init(&iir);

    for(i=0; i < SIGNAL_LENGTH; i++){
        iir.input = sigIn[i];
        iir.calc(&iir);
//        FILTER_runIIR(&iir);
        sigOut[i] = iir.output32;
    }

#endif

return(1);
}
uint16_t FIXPT_DSP_genReport()
{
    //Locals
    int16_t i;
    uint16_t goldPass = 0; // Each test must define an expected pass value
    pass = 0; fail = 0;
#if (RUN_TEST == TEST_1)
    goldPass = 4*IIR32_NBIQ;
    // Check above dbuffer
    for(i = 0; i < 2*IIR32_NBIQ; i++) {
        if(temp[i] == 0xBAAD){
            pass++;
        }else{
            fail++;
        }
    }
    // check dbuffer
    for(i = 2*IIR32_NBIQ; i < (2*IIR32_NBIQ + goldPass); i++) {
        if(temp[i] == 0){
            pass++;
        }else{
            fail++;
        }
    }
    // Check below dbuffer
    for((i = 2*IIR32_NBIQ + goldPass); i < 2*SIGNAL_LENGTH; i++) {
        if(temp[i] == 0xBAAD){
            pass++;
        }else{
            fail++;
        }
    }
#elif (RUN_TEST == TEST_2)
    goldPass = 4*IIR32_NBIQ;
    // Check above dbuffer
    for(i = 0; i < 2*IIR32_NBIQ + 1; i++) {
        if(temp[i] == 0xBAAD){
            pass++;
        }else{
            fail++;
        }
    }
    // check dbuffer
    for(i = 2*IIR32_NBIQ + 1; i < (2*IIR32_NBIQ + goldPass + 1); i++) {
        if(temp[i] == 0){
            pass++;
        }else{
            fail++;
        }
    }
    // Check below dbuffer
    for((i = 2*IIR32_NBIQ + goldPass + 1); i < 2*SIGNAL_LENGTH; i++) {
        if(temp[i] == 0xBAAD){
            pass++;
        }else{
            fail++;
        }
    }
#elif ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5))
    goldPass =  SIGNAL_LENGTH;
    // Check that bit reversal was done correctly
    for(i = 0; i < goldPass; i++ ){
        err[i] = labs(goldOut[i] - sigOut[i]);
        if(err[i] < TOLERANCE){
            pass++;
        }else{
            fail++;
        }
    }

#endif

    return(1);
}



int main(void)
{
	// Initialize trap
	trap_init();
	init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/
	FIXPT_DSP_runTest();
	    FIXPT_DSP_genReport();

	    for(;;);

	return 0;
}
