#include "RV335_Project.h"     // Device Headerfile and Examples Include File

#include <stdio.h>
#include <math.h>
#include <stdint.h>


#define SIZE            100U
#define EPSILON         0.0001f
typedef float fsize_t;

fsize_t x[SIZE] = {
    1.6992375,  2.1354470,  0.1148661,  2.5421889,
    2.3518555,  0.3775787,  1.6494780,  1.0236365,
    1.7167215,  1.2531208,  1.3040543,  0.5678044,
    0.8023211,  0.0645150,  2.9018125,  2.0536588,
    2.9298919,  0.5136893,  2.8937124,  2.4964914,
    1.8139374,  1.3824126,  0.8093174,  2.3623093,
    0.7183864,  0.2016497,  2.4106368,  2.1086439,
    2.2469064,  2.0170936,  1.3164790,  1.2276153,
    2.5639798,  0.9972290,  2.5589522,  2.4789476,
    2.6774660,  1.5885043,  1.9969891,  2.9873229,
    1.3947545,  0.1885547,  2.7229751,  1.9829379,
    1.1154968,  3.1321782,  0.7042555,  2.0497355,
    1.9006342,  1.2165674,  0.4466941,  0.0789639,
    1.3229632,  0.5783681,  2.2800902,  1.1635287,
    2.6438390,  2.3066506,  1.7939307,  0.5556065,
    3.0077106,  0.8335338,  2.9046565,  0.7029955,
    1.1735853,  0.2748905,  2.0109854,  0.5674247,
    0.1415322,  2.2719165,  1.0915076,  2.0753890,
    1.2059588,  1.9708672,  0.0680149,  2.8606400,
    2.5150292,  2.3431490,  2.5544692,  1.2041923,
    1.9392399,  1.8079704,  1.6652065,  0.8641571,
    0.7810909,  1.4188650,  0.7153809,  2.5272529,
    3.0979378,  0.0942225,  1.6828387,  0.2735612,
    2.5198446,  3.1074904,  0.2103179,  2.9512070,
    0.0571064,  2.1483424,  2.4621808,  0.3141592,
};

// Ê¹ÓÃC++ sortº¯Êý¼ÆËãµÄ½á¹û
fsize_t x_golden[SIZE] = {
    0.0571064 , 0.0645150 , 0.0680149 , 0.0789639 , 0.0942225 ,
    0.1148661 , 0.1415322 , 0.1885547 , 0.2016497 , 0.2103179 ,
    0.2735612 , 0.2748905 , 0.3141592 , 0.3775787 , 0.4466941 ,
    0.5136893 , 0.5556065 , 0.5674247 , 0.5678044 , 0.5783681 ,
    0.7029955 , 0.7042555 , 0.7153809 , 0.7183864 , 0.7810909 ,
    0.8023211 , 0.8093174 , 0.8335338 , 0.8641571 , 0.9972290 ,
    1.0236365 , 1.0915076 , 1.1154968 , 1.1635287 , 1.1735853 ,
    1.2041923 , 1.2059588 , 1.2165674 , 1.2276154 , 1.2531208 ,
    1.3040543 , 1.3164790 , 1.3229632 , 1.3824126 , 1.3947545 ,
    1.4188650 , 1.5885043 , 1.6494780 , 1.6652066 , 1.6828387 ,
    1.6992375 , 1.7167215 , 1.7939306 , 1.8079704 , 1.8139374 ,
    1.9006342 , 1.9392399 , 1.9708672 , 1.9829379 , 1.9969891 ,
    2.0109854 , 2.0170937 , 2.0497355 , 2.0536587 , 2.0753889 ,
    2.1086440 , 2.1354470 , 2.1483424 , 2.2469065 , 2.2719164 ,
    2.2800901 , 2.3066506 , 2.3431489 , 2.3518555 , 2.3623092 ,
    2.4106369 , 2.4621809 , 2.4789476 , 2.4964914 , 2.5150292 ,
    2.5198445 , 2.5272529 , 2.5421889 , 2.5544691 , 2.5589521 ,
    2.5639799 , 2.6438389 , 2.6774659 , 2.7229750 , 2.8606400 ,
    2.8937123 , 2.9018126 , 2.9046564 , 2.9298918 , 2.9512069 ,
    2.9873228 , 3.0077107 , 3.0979378 , 3.1074903 , 3.1321783
};

uint16_t pass =0, fail = 0;

void qsort_SP_RV(void *base, const uint16_t nmemb);
void VECTOR_qsortRV(void *base, const uint16_t nmemb);

int  main()
{
      // Locals
    uint16_t i;
    //VECTOR_qsortRV(x, SIZE);  // c function

    __asm("CSRRWI t0, 0x808, 0x7; ");
    uint32_t start_time=0;
  uint32_t end_time=0;
  StartCpuTimer0();
  start_time=ReadCpuTimer0Counter();
   
    qsort_SP_RV(x, SIZE); // asm function
    end_time=ReadCpuTimer0Counter();
                   uint32_t count=start_time-end_time-14;

    // Check the output
      for(i = 0; i < SIZE; i++){

        if(x[i] == x_golden[i]){
            pass++;
        }else{
            fail++;
        }

    // printf("x[%d] = %f ,x_golden[%d] = %f \n",i ,x[i] , i , x_golden[i]);

    }


    return 0;
}


static inline void swap_item(fsize_t *src, fsize_t *dest)
{
    fsize_t temp;

    temp = *src;
    *src  = *dest;
    *dest = temp;

}

void VECTOR_qsortRV(void *base, const uint16_t nmemb)
{
    char   *basep  = base;              // POINTER TO ARRAY OF ELEMENTS
    uint16_t  i    = 0;                 // left scan index
    // right scan index
    uint16_t  j    = sizeof(fsize_t)*(nmemb - 1);
    // Want closest even value
    uint16_t pivot = (nmemb & 0xFFFE)*(sizeof(fsize_t) / 2);
    char   *pivp   = basep + pivot ;

    if (nmemb <= 1) return;

    while( i < j )
    {
        while(*(fsize_t*)(basep + i) < *(fsize_t*)pivp)
        {
            i=i+sizeof(fsize_t);
        }
        while(*(fsize_t*)(basep + j) > *(fsize_t*)pivp)
        {
            j=j-sizeof(fsize_t);
        }

        if( i < j )
        {
            swap_item((fsize_t*)(basep + i), (fsize_t*)(basep + j));
            if ( pivot == i )
            {
                pivot = j;
                pivp = basep + pivot;
            }
            else if( pivot == j )
            {
                pivot = i;
                pivp = basep + pivot;
            }
            i=i+sizeof(fsize_t);
            j=j-sizeof(fsize_t);
        }
        else if ( i == j )
        {
            i=i+sizeof(fsize_t);
            j=j-sizeof(fsize_t);
            break;
        }
    }

    if(j > 0)
    {
        VECTOR_qsortRV(basep, (j/sizeof(fsize_t) + 1U));
    }
    if((i/sizeof(fsize_t)) < (nmemb - 1))
    {
        VECTOR_qsortRV(basep + i, nmemb - (i/sizeof(fsize_t)));
    }

}



