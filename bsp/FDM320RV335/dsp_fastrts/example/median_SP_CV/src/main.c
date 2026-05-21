#include "RV335_Project.h"     // Device Headerfile and Examples Include File

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <float.h>


#define SIZE            256U
#define MEDIAN_VALUE    2.637057027747737f
#define EPSILON         0.000001f

typedef float fsize_t;

float x[SIZE] = {
 0.8074237,  0.8938309,  2.1144284,  0.4711467, 2.9926183,  2.3546213,  3.4797466,  3.4994392,
 3.1926538,  0.1680192,  0.3440305,  1.5979987, 2.6543214,  3.2722285,  2.0380960,  4.0999061,
 3.5917947,  4.8432467,  2.6566695,  1.6257284, 0.5281460,  3.0547933,  3.8940112,  2.1172646,
 0.4541164,  1.3323575,  0.7682836,  1.4050265, 2.2004257,  2.6357137,  2.2871218,  4.3768580,
 2.5902605,  4.7181131,  3.1885455,  4.7884697, 1.2035352,  3.3806115,  1.4453229,  3.3590408,
 3.4757025,  0.3399638,  1.2739508,  1.1202002, 3.3391636,  4.2219608,  1.7223121,  3.9025983,
 3.3766603,  0.0335766,  3.0108524,  1.9338560, 4.5799562,  0.0057553,  2.3122458,  2.1217452,
 2.3045818,  3.8507986,  1.6123590,  3.9236965, 2.3567858,  0.1788137,  0.8793721,  3.6087902,
 2.3674300,  0.7636060,  1.7056230,  3.0369461, 0.9587263,  3.6921342,  1.2142480,  4.5871217,
 1.3453079,  3.8275001,  0.9433099,  1.4374909, 0.4555673,  2.8810469,  3.4168162,  2.7329656,
 2.1286442,  3.2222139,  3.2380882,  3.3950838, 3.1789336,  4.7258706,  1.0446746,  3.5464085,
 1.1811529,  0.5969812,  3.0365197,  2.2506885, 2.2936275,  3.3097238,  3.8514276,  1.7510901,
 3.3100480,  2.0807929,  4.2096458,  4.1645841, 1.2822050,  3.0673037,  2.9112458,  2.7036967,
 4.3497052,  1.3238951,  1.5903704,  0.5960727, 4.6991474,  3.2277594,  2.3973161,  3.1965848,
 2.7235806,  3.2365574,  2.7194297,  3.6052331, 2.6124765,  4.9685231,  1.0933832,  0.5289914,
 0.5484873,  0.3179569,  2.0229000,  2.2418646, 1.8290809,  3.8175232,  3.1394819,  3.8599019,
 4.6642679,  4.8637043,  0.9601417,  0.6943710, 3.4813317,  0.4691001,  2.6270220,  2.6517211,
 4.3056991,  2.4242667,  1.9672818,  3.3571557, 3.7062897,  2.6002623,  1.7385634,  0.7499863,
 2.9304603,  1.3107266,  0.2222705,  3.7746663, 1.2139268,  2.2120116,  3.4389804,  1.7961411,
 3.6817004,  1.9735374,  3.4170793,  3.5202372, 2.2115271,  0.0978881,  1.6542894,  2.1215475,
 1.3513521,  0.9852690,  4.1086059,  2.1496070, 4.4388548,  1.9559150,  3.8455719,  1.9839576,
 4.0425705,  3.7753855,  1.8869777,  1.0800946, 3.9520361,  4.7465196,  1.6378272,  3.3563219,
 2.1932249,  4.1675030,  3.8442713,  0.8362677, 4.3099024,  4.9493608,  2.5721173,  4.4214051,
 2.9401303,  0.7737617,  0.9993141,  2.0347742, 3.7435286,  4.1279191,  3.9498151,  1.5926212,
 2.6703206,  0.4497534,  0.5585287,  0.6814627, 3.3932615,  2.4758851,  0.9485520,  2.4750291,
 0.7380411,  0.2748707,  4.2535634,  2.8027976, 4.6480443,  3.4833360,  2.9139548,  4.0769861,
 4.3950695,  4.9445581,  0.0026119,  4.3271930, 3.0628323,  4.9497510,  2.6384003,  2.3976169,
 4.0067380,  1.1392147,  2.4904715,  4.5042624, 2.8733061,  4.2258909,  3.6932015,  2.9299352,
 1.2336726,  3.3320811,  0.4174141,  3.1297989, 3.3047228,  3.6487593,  4.4537606,  4.9115161,
 3.8451454,  2.9072324,  4.6415653,  2.9004518, 0.0849147,  0.6042979,  4.3135536,  2.4214826,
 4.2242784,  1.0470254,  2.7614567,  3.1494169, 0.1599551,  3.0735671,  1.8120573,  0.2476629,
 2.4478499,  0.9625520,  0.6154187,  1.0274709, 0.7325746,  0.9453609,  0.2132621,  3.1759896,
};


float median = 0.0f;

uint16_t pass =0, fail = 0;
fsize_t VECTOR_medianRV(fsize_t *x, const uint16_t n);
fsize_t median_SP_RV(fsize_t *x, const uint16_t n);

int main(void)
{
    // Initialize trap
    trap_init();
    init_priority();
    // Initialize System Control
    InitSysCtrl();
    /* --- add your code below ---*/
    // Locals
    uint16_t i;
   __asm("CSRRWI t0, 0x808, 0x7; ");
    uint32_t start_time=0;
  uint32_t end_time=0;
  StartCpuTimer0();
  start_time=ReadCpuTimer0Counter();
   
   median = median_SP_RV(x, SIZE);
   end_time=ReadCpuTimer0Counter();
                   uint32_t count=start_time-end_time-14;

//  median = VECTOR_medianRV(x, SIZE);
    __asm(" NOP");

    //Check the output
    if(fabs(median - MEDIAN_VALUE) < EPSILON){
        pass++;
    }else{
        fail++;
    }



    return 0;
}

//************************************c ���� ********************************

#define ELEM_SWAP(a,b) { fsize_t t=(a);                      \
                         (a)=(b);                            \
                         (b)=t;}

static fsize_t _median_find_average_RV(fsize_t *x, fsize_t median_low,
                                       uint16_t n);

fsize_t VECTOR_medianRV(fsize_t *x, const uint16_t n)
{
    fsize_t* p;
    uint16_t low, high;
    uint16_t median;
    uint16_t middle, ll, hh;

    //--- Initialization
    low = 0;
    high = n-1;
    median = (low + high)/2;

    //--- Main loop
    for(;;)
    {

        if (high <= low)                            // One element only
        {
            if(n%2 == 0)                            // Array length is even
            {
                return(_median_find_average_RV(x, x[median], n));
            }
            else                                    // Array length is odd
            {
                return x[median];
            }
        }
        if (high == low + 1)                        // Two elements only
        {
            if (x[low] > x[high]) ELEM_SWAP(x[low], x[high]) ;
            {
                if(n%2 == 0)                        // Array length is even
                {
                    return(_median_find_average_RV(x, x[median], n));
                }
                else                                // Array length is odd
                {
                    return x[median];
                }
            }
        }
        //--- Find median of low, middle and high items; swap into position low
        middle = (low + high) / 2;
        if (x[middle] > x[high])    ELEM_SWAP(x[middle], x[high]) ;
        if (x[low] > x[high])        ELEM_SWAP(x[low], x[high]) ;
        if (x[middle] > x[low])        ELEM_SWAP(x[middle], x[low]) ;
        //--- Swap low item (now in position middle) into position (low+1)
        ELEM_SWAP(x[middle], x[low+1]) ;

        //--- Nibble from each end towards middle, swapping items when stuck
        ll = low + 1;
        hh = high;
        for(;;)
        {
            p = &x[ll]; *p++;                            // Setup the pointer
            do ll++; while (x[low] > *p++) ;
            p = &x[hh]; *p--;                            // Setup the pointer
            do hh--; while (*p-- > x[low]) ;

            if (hh < ll) break;
            ELEM_SWAP(x[ll], x[hh]) ;
        }

        //--- Swap middle item (in position low) back into correct position
        ELEM_SWAP(x[low], x[hh]) ;

        //--- Re-set active partition
        if (hh <= median) low = ll;
        if (hh >= median) high = hh - 1;

    } // end of for(;;)

}

fsize_t _median_find_average_RV(fsize_t *x, fsize_t median_low, uint16_t n)
{
    fsize_t delta;         // Distance from median_low to current value
    fsize_t median_delta;  // Distance from median_low to median_high
    fsize_t median_high;   // Upper median value
    uint16_t higher_count; // Number of values strictly greater than median_low
    uint16_t i;            // General purpose

    //--- Assign initial values
    median_high = FLT_MAX;                   // Largest fsize_t value.
                                             // Defined in math.h.
    median_delta = median_high - median_low; // Compute initial value
    higher_count = 0;                        // Initialize to zero

    //--- Sequence through the array.  Find how many values are strictly
    //    greater than median_low.
    //--- Also find the closest strictly higher value to median_low.
    //--- Unroll the loop once.
    for(i=0; i<n/2; i++)
    {
        //--- Unrolled loop iteration #1
        if(*x > median_low)
        {
            higher_count++;             // Increment the counter
            delta = *x - median_low;    // Compute delta
            if(delta < median_delta)    // Current value is closer to
                                        // median_low than current median_high
            {
                median_high = *x;       // New median_high is current array
                                        // value
                median_delta = delta;   // New median_high_low_delta is current
                                        // delta
            }
        }
        x++;                            // Increment the pointer

        //--- Unrolled loop iteration #2
        if(*x > median_low)
        {
            higher_count++;             // Increment the counter
            delta = *x - median_low;    // Compute delta
            if(delta < median_delta)    // Current value is closer to
                                        // median_low than current median_high
            {
                median_high = *x;       // New median_high is current array
                                        // value
                median_delta = delta;   // New median_high_low_delta is current
                                        // delta
            }
        }
        x++;                            // Increment the pointer

    } // end for()

    //--- Now determine if array is case (a), (b), or (c)
    if(higher_count < n/2)    // It is case (c)
    {
        return(median_low);   // median_high=median_low, so median_ave also
                              // equals median_low
    }
    else                      // It is case (a) or (b)
    {
        return( (median_low + median_high)/2 );
    }

}
