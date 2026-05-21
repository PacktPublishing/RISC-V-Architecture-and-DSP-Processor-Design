#include "RV335_Project.h"     // Device Headerfile and Examples Include File
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "fft_hamming_Q31.h"
#include "fft.h"
#include "test_cfft.h"

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

#ifndef __cplusplus
#pragma DATA_SECTION(ipcbsrc, "FFTipcbsrc");
#else
#pragma DATA_SECTION("FFTipcbsrc");
#endif

#if (RUN_TEST == TEST_2)

int32_t ipcbsrc[8*FFT_SIZE]= {

    #include XSTRINGIZE(XCONCAT(test,RUN_TEST,_data_input.txt))
};

#else
int32_t ipcbsrc[2 * FFT_SIZE]= {

    #include XSTRINGIZE(XCONCAT(test,RUN_TEST,_data_input.txt))
};
#endif
#ifndef __cplusplus
#pragma DATA_SECTION(ipcb, "FFTipcb");
#else
#pragma DATA_SECTION("FFTipcb");
#endif
#if ((RUN_TEST == TEST_1) || (RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5) ||\
     (RUN_TEST == TEST_6) || (RUN_TEST == TEST_7) || (RUN_TEST == TEST_8) || (RUN_TEST == TEST_9) ||\
     (RUN_TEST == TEST_10)|| (RUN_TEST == TEST_11)|| (RUN_TEST == TEST_12) || (RUN_TEST == TEST_13) ||\
     (RUN_TEST == TEST_14)|| (RUN_TEST == TEST_15)|| (RUN_TEST == TEST_16) || (RUN_TEST == TEST_17) ||\
     (RUN_TEST == TEST_18))
int32_t ipcb[2*FFT_SIZE];
#elif RUN_TEST == TEST_2
int32_t ipcb[8*FFT_SIZE];
#endif


#if RUN_TEST == TEST_1
const int32_t goldOut[2*FFT_SIZE] = {
    #include "test1_data_bitreversed.txt"
};
#elif RUN_TEST == TEST_2
const int32_t goldOut[8*FFT_SIZE] = {
    #include "test2_data_bitreversed.txt"
};
#elif ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5) ||\
       (RUN_TEST == TEST_6) || (RUN_TEST == TEST_7) || (RUN_TEST == TEST_8) || (RUN_TEST == TEST_9) ||\
       (RUN_TEST == TEST_10)|| (RUN_TEST == TEST_11)|| (RUN_TEST == TEST_12) || (RUN_TEST == TEST_13) ||\
       (RUN_TEST == TEST_14)|| (RUN_TEST == TEST_15)|| (RUN_TEST == TEST_16) || (RUN_TEST == TEST_17) ||\
       (RUN_TEST == TEST_18))
int32_t goldOut[2*FFT_SIZE] = {
    #include XSTRINGIZE(XCONCAT(test,RUN_TEST,_data_output.txt))
};
#endif

#if ((RUN_TEST == TEST_1) || (RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5) ||\
     (RUN_TEST == TEST_6) || (RUN_TEST == TEST_7) || (RUN_TEST == TEST_8) || (RUN_TEST == TEST_9) ||\
     (RUN_TEST == TEST_10)|| (RUN_TEST == TEST_11)|| (RUN_TEST == TEST_12) || (RUN_TEST == TEST_13) ||\
     (RUN_TEST == TEST_14)|| (RUN_TEST == TEST_15)|| (RUN_TEST == TEST_16) || (RUN_TEST == TEST_17) ||\
     (RUN_TEST == TEST_18))
int32_t err[2*FFT_SIZE];
#elif RUN_TEST == TEST_2
int32_t err[8*FFT_SIZE];
#endif

// Declare and initialize the structure object.
// Use the CFFT32_<n>P_DEFUALTS in the FFT header file if
// unsure as to what values to program the object with.
#if ((RUN_TEST == TEST_1) || (RUN_TEST == TEST_2))
CFFT32  cfft=CFFT32_1024P_DEFAULTS;
#elif ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_11))
CFFT32  cfft=CFFT32_32P_DEFAULTS;
#elif ((RUN_TEST == TEST_4) || (RUN_TEST == TEST_12))
CFFT32  cfft=CFFT32_64P_DEFAULTS;
#elif ((RUN_TEST == TEST_5) || (RUN_TEST == TEST_13))
CFFT32  cfft=CFFT32_128P_DEFAULTS;
#elif ((RUN_TEST == TEST_6) || (RUN_TEST == TEST_14))
CFFT32  cfft=CFFT32_256P_DEFAULTS;
#elif ((RUN_TEST == TEST_7) || (RUN_TEST == TEST_15))
CFFT32  cfft=CFFT32_512P_DEFAULTS;
#elif ((RUN_TEST == TEST_8) || (RUN_TEST == TEST_16))
CFFT32  cfft=CFFT32_1024P_DEFAULTS;
#elif ((RUN_TEST == TEST_9) || (RUN_TEST == TEST_17))
CFFT32  cfft=CFFT32_2048P_DEFAULTS;
#elif ((RUN_TEST == TEST_10) || (RUN_TEST == TEST_18))
CFFT32  cfft=CFFT32_4096P_DEFAULTS;
#endif

CFFT32_Handle handleCFFT32;
int32_t twi[769];
uint16_t pass = 0, fail = 0;
//*****************************************************************************
// function definitions
//*****************************************************************************
uint16_t FIXPT_DSP_runTest()
{
    // Locals

    //Initialize CFFT object
    cfft.ipcbptr = ipcb;                          // FFT computation buffer
    cfft.magptr  = (int32_t *)ipcbsrc;                       // Magnitude output buffer
    cfft.tfptr = twi;
//  cfft.winptr  = (long *)win;                   // Window coefficient array

    // Initialize the handle
    handleCFFT32 = (CFFT32_Handle)&cfft;

#if RUN_TEST == TEST_1
    // Basic test of bit reversal. input/output vectors generated in MATLAB

    // zero out ipcb
    memset(ipcb, 0, sizeof(ipcb));

    // Step 3: Bit reverse order the input
    FFT32_brev(ipcbsrc, ipcb, FFT_SIZE);         // Real part bit-reversing
    FFT32_brev(&ipcbsrc[1], &ipcb[1], FFT_SIZE);
//    FFT32_brev(&ipcbsrc[1], &ipcb[1], FFT_SIZE); // Imaginary part bit-reversing
#elif RUN_TEST == TEST_2
    // Check if bit reversal writes to memory outside its bounds, we check FFT_SIZE*4 word locations
    // above and 2*FFT_SIZE*4 word locations below for memory corruption

    // zero out ipcb
    memset(ipcb, 0x0, sizeof(ipcb));

    // Step 3: Bit reverse order the input
    FFT32_brev(&ipcbsrc[2*FFT_SIZE], &ipcb[2*FFT_SIZE], FFT_SIZE);         // Real part bit-reversing
    FFT32_brev(&ipcbsrc[2*FFT_SIZE+1], &ipcb[2*FFT_SIZE+1], FFT_SIZE);
//    CFFT32_brev(&ipcbsrc[2*FFT_SIZE + 1], &ipcb[2*FFT_SIZE + 1], FFT_SIZE); // Imaginary part bit-reversing

#elif ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5) ||\
       (RUN_TEST == TEST_6) || (RUN_TEST == TEST_7) || (RUN_TEST == TEST_8) || (RUN_TEST == TEST_9) ||\
       (RUN_TEST == TEST_10))
    // 32-pt FFT
    // zero out ipcb
    memset(ipcb, 0, sizeof(ipcb));
    // Bit reverse order the input
    FFT32_brev(ipcbsrc, ipcb, FFT_SIZE);         // Real part bit-reversing
    FFT32_brev(&ipcbsrc[1], &ipcb[1], FFT_SIZE); // Imaginary part bit-reversing
    // Initialize and run the FFT routines
    cfft.init(handleCFFT32);                             // Twiddle factor pointer initialization
    cfft.calc(handleCFFT32);                              // Compute the FFT

#elif ((RUN_TEST == TEST_11)|| (RUN_TEST == TEST_12) || (RUN_TEST == TEST_13) ||\
       (RUN_TEST == TEST_14)|| (RUN_TEST == TEST_15) || (RUN_TEST == TEST_16) || (RUN_TEST == TEST_17) ||\
       (RUN_TEST == TEST_18))
    // 32-pt FFT
    // zero out ipcb
    memset(ipcb, 0, sizeof(ipcb));
    // Bit reverse order the input
    FFT32_brev((int32_t *)ipcbsrc, ipcb, FFT_SIZE);         // Real part bit-reversing
    FFT32_brev((int32_t *)&ipcbsrc[1], &ipcb[1], FFT_SIZE); // Imaginary part bit-reversing
    // Initialize and run the FFT routines
    cfft.init(handleCFFT32);                             // Twiddle factor pointer initialization
    cfft.calc(handleCFFT32);                              // Compute the FFT
    // Step 6: Calculate the magnitude
    cfft.mag(handleCFFT32);                               // Q30 format (abs(ipcbsrc)/2^16).^2
#endif
    return(1);
}

uint16_t FIXPT_DSP_genReport()
{
    //Locals
    int16_t i;
    uint16_t goldPass = 0; // Each test must define an expected pass value
    pass =0; fail = 0;
#if RUN_TEST == TEST_1
    goldPass =  2*FFT_SIZE;
    // Check that bit reversal was done correctly
    for(i = 0; i < goldPass; i++ ){
        err[i] = labs(goldOut[i] - ipcb[i]);
        if(err[i] == 0){
            pass++;
        }else{
            fail++;
        }
    }

#elif RUN_TEST == TEST_2
    goldPass =  8*FFT_SIZE;
    // Check that bit reversal was done correctly
    for(i = 0; i < goldPass; i++ ){
        err[i] = labs(goldOut[i] - ipcb[i]);
        if(err[i] == 0){
            pass++;
        }else{
            fail++;
        }
    }
#elif  ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5) ||\
       (RUN_TEST == TEST_6) || (RUN_TEST == TEST_7) || (RUN_TEST == TEST_8) || (RUN_TEST == TEST_9) ||\
       (RUN_TEST == TEST_10))
    goldPass =  2*FFT_SIZE;
    // Check that bit reversal was done correctly
    for(i = 0; i < goldPass; i++ ){
        err[i] = labs(goldOut[i] - ipcb[i]);
        if(err[i] < TOLERANCE){
            pass++;
        }else{
            fail++;
        }
    }
#elif  ((RUN_TEST == TEST_11) || (RUN_TEST == TEST_12) || (RUN_TEST == TEST_13) ||\
       (RUN_TEST == TEST_14) || (RUN_TEST == TEST_15) || (RUN_TEST == TEST_16) || (RUN_TEST == TEST_17) ||\
       (RUN_TEST == TEST_18))
    goldPass =  FFT_SIZE;
    // Check that bit reversal was done correctly
    for(i = 0; i < goldPass; i++ ){
        err[i] = labs(goldOut[i] - ipcbsrc[i]);
        if(err[i] < TOLERANCE){
            pass++;
        }else{
            fail++;
        }
    }
#endif
    if((pass == goldPass) && (fail == 0)){
        return(1);
    }
    return(0);
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

    for(;;){}
    return 0;
}
