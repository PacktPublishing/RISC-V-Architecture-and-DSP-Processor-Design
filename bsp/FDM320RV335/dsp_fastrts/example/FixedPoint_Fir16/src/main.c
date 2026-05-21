#include "RV335_Project.h"     // Device Headerfile and Examples Include File

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
#include "test_fir16d.h"
#include "filter.h"

//*****************************************************************************
// defines
//*****************************************************************************

//*****************************************************************************
// globals
//*****************************************************************************
// Create an Instance of FIR16 module and place the object in "firfilt" section
#ifndef __cplusplus
#pragma DATA_SECTION(fir, "firfilt");
#else
#pragma DATA_SECTION("firfilt");
#endif
FIR16 fir= FIR16_DEFAULTS;

#if ((RUN_TEST == TEST_1) || (RUN_TEST == TEST_2)  || (RUN_TEST == TEST_8))
int16_t temp[SIGNAL_LENGTH];
int32_t *dbuffer;

#elif ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5) || \
       (RUN_TEST == TEST_6) || (RUN_TEST == TEST_7))
// Define the Delay buffer for the "FIR_ORDER"th order filter
// and place it in "firldb" section. Since we define it as int32_t,
// The size of the buffer is:
// FIR_ORDER even -> (FIR_ORDER+1)/2+1 e.g. FIR_ORDER = 32, size = 17 dwords
//           odd  -> (FIR_ORDER+1)/2   e.g. FIR_ORDER = 31, size = 16 dwords
// The delay line buffer must be aligned to a 256 word boundary
#ifndef __cplusplus
#pragma DATA_SECTION(dbuffer,"firldb");
#else
#pragma DATA_SECTION("firfilt");
#endif
#if(FIR_ORDER & 0x01)   // odd
//int32_t dbuffer[(FIR_ORDER+1)/2];

int32_t dbuffer[FIR_ORDER + 2];
#else
int32_t dbuffer[(FIR_ORDER+ 3)/2];
#endif
// Input/output buffer
#ifndef __cplusplus
#pragma DATA_SECTION(sigIn, "sigIn");
#else
#pragma DATA_SECTION("sigIn");
#endif
int16_t sigIn[SIGNAL_LENGTH]= {
    #include XSTRINGIZE(XCONCAT(test,RUN_TEST,_data_input.txt))
};

#ifndef __cplusplus
#pragma DATA_SECTION(sigOut, "sigOut");
#else
#pragma DATA_SECTION("sigOut");
#endif
int16_t sigOut[SIGNAL_LENGTH];

const int16_t goldOut[SIGNAL_LENGTH] = {
    #include XSTRINGIZE(XCONCAT(test,RUN_TEST,_data_output.txt))
};

// Define Constant Coefficient Array  and place it in the "coefffilt"
// section. You can either reorder the coefficients at run time (done
// in this example) or just store them reordered.
// Index       LSW       MSW
//         +---------+---------+
//  0      | h(L-1)  | h(L/2-1)|
//  2      | h(L-2)  | h(L/2-2)|
//  4      | h(L-3)  | h(L/2-3)|
//  6      | h(L-4)  | h(L/2-4)|
//  ...    | ...     | ...     |
//  L/2-3  | h(L/2+2)| h(2)    |
//  L/2-2  | h(L/2+1)| h(1)    |
//  L/2-1  | h(L/2)  | h(0)    |
//         +---------+---------+
// The size of the array is:
// FIR_ORDER even -> (FIR_ORDER+1)+1 e.g. FIR_ORDER = 32, size = 34 words
//           odd  -> (FIR_ORDER+1)   e.g. FIR_ORDER = 31, size = 32 words
// The reason being that we use the DMAC operation which will multiply
// the coefficients in 32-bit chunks, we don't want an odd size coefficient
// buffer and risk corrupting the delay line

#pragma DATA_SECTION("coefffilt");
#if(FIR_ORDER & 0x01)   // odd
int16_t coeff[FIR_ORDER+1] = {
    #include XSTRINGIZE(XCONCAT(test,RUN_TEST,_coeffs.txt))
};
#else
int16_t coeff[FIR_ORDER+2] = {
    #include XSTRINGIZE(XCONCAT(test,RUN_TEST,_coeffs.txt))
};
#endif


#pragma DATA_SECTION("coefffilt");

int16_t revCoeff[FIR_ORDER+2];

int16_t err[SIGNAL_LENGTH];
#endif // ((RUN_TEST == TEST_2)||(RUN_TEST == TEST_3))

uint16_t pass = 0, fail = 0;
//*****************************************************************************
// function definitions
//*****************************************************************************


uint16_t FIXPT_DSP_runTest()
{
    // Locals
    int16_t i = 0;

#if(RUN_TEST == TEST_8)
    // Write dummy codes to temp
    memset(temp, 0xBAAD, sizeof(temp));

    dbuffer         = (int32_t *)&temp[FIR_ORDER]; // Set dbuffer to an even address
    fir.order       = FIR_ORDER;        // even
    fir.dbuffer_ptr = &dbuffer[0];
    fir.coeff_ptr   = 0;
    fir.init(&fir);
    fir.calc(&fir);

#elif ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5) || \
       (RUN_TEST == TEST_6) || (RUN_TEST == TEST_7))
    // zero out sigOut
    memset(sigOut, 0, sizeof(sigOut));

    // Load the coefficients in normal order. If
    // FIR_ORDER even -> number of taps are odd, zero out the last element of coeffs
    //           odd  -> number of taps are even, do nothing
#if ((FIR_ORDER & 0x01) == 0)
        coeff[FIR_ORDER + 1] = 0;

    // if the order of the coefficients is even, you will have
    // an odd number of taps; zero out the location right after the
    // last tap
        revCoeff[FIR_ORDER_REV-1] = 0;
#endif
    // Reorder the coefficients
    // FIR_ORDER even -> (FIR_ORDER+1)+1 e.g. FIR_ORDER = 32, size = 34 words
    //           odd  -> (FIR_ORDER+1)   e.g. FIR_ORDER = 31, size = 32 words
#if (FIR_ORDER & 0x01) //odd order
    for(i = 0; i < FIR_ORDER + 1; i = i + 2){
#else // even order
    for(i = 0; i < FIR_ORDER + 2; i = i + 2){
#endif
        revCoeff[FIR_ORDER_REV-i-1] = coeff[i/2+FIR_ORDER_REV/2];
        revCoeff[FIR_ORDER_REV-i-2] = coeff[i/2];
    }

    // Initialize FIR16 object
    fir.order       = FIR_ORDER;
//    fir.dbuffer_ptr = &dbuffer[1024];
    fir.dbuffer_ptr = &dbuffer[FIR_ORDER +1];
    fir.coeff_ptr   = (int32_t *)&revCoeff[0];
    fir.init(&fir);

    // FIR calculation
    for(i = 0; i < SIGNAL_LENGTH; i++){

        fir.input  = sigIn[i]; // Q15 format
        fir.calc(&fir);
        sigOut[i]  = fir.output;
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
#if ((RUN_TEST == TEST_3) || (RUN_TEST == TEST_4) || (RUN_TEST == TEST_5) || \
       (RUN_TEST == TEST_6) || (RUN_TEST == TEST_7))
    goldPass =  SIGNAL_LENGTH;
    // Check that bit reversal was done correctly
    for(i = 0; i < goldPass; i++ ){
        err[i] = abs(goldOut[i] - sigOut[i]);
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
