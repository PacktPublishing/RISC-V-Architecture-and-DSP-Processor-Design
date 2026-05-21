#include "RV335_Device.h"     // RV335 Headerfile Include File
#include "RV335_Examples.h"   // RV335 Examples Include File

#include"math.h"

#define IIRNUMBER 2
#define SIGNAL1F 1000
#define SIGNAL2F 4500
#define SAMPLEF  10000
#define PI 3.1415926

float InputWave();
float IIR();

float fBn[IIRNUMBER]={ 0.0,0.7757 };
float fAn[IIRNUMBER]={ 0.1122,0.1122 };
float fXn[IIRNUMBER]={ 0.0 };
float fYn[IIRNUMBER]={ 0.0 };
float fInput,fOutput;
float fSignal1,fSignal2;
float fStepSignal1,fStepSignal2;
float f2PI;
int i;
float fIn[256],fOut[256];
int nIn,nOut;

main(void)
{
   
   	nIn=0; nOut=0;
	f2PI=2*PI;
	fSignal1=0.0;
	fSignal2=PI*0.1;
//	fStepSignal1=2*PI/30;
//	fStepSignal2=2*PI*1.4;
	fStepSignal1=2*PI/50;
	fStepSignal2=2*PI/2.5;
	while ( 1 )
	{
		fInput=InputWave();
		fIn[nIn]=fInput;
		nIn++; nIn%=256;
		fOutput=IIR();
		fOut[nOut]=fOutput;
		nOut++;
		if ( nOut>=256 )
		{
			nOut=0;		/* 请在此句上设置软件断点 */
		}
	}
} 	


float InputWave()
{
	for ( i=IIRNUMBER-1;i>0;i-- )
	{
		fXn[i]=fXn[i-1];
		fYn[i]=fYn[i-1];
	}
	fXn[0]=sin(fSignal1)+cos(fSignal2)/6.0;
	fYn[0]=0.0;
	fSignal1+=fStepSignal1; 
	if ( fSignal1>=f2PI )	fSignal1-=f2PI;
	fSignal2+=fStepSignal2;
	if ( fSignal2>=f2PI )	fSignal2-=f2PI;
	return(fXn[0]);
}

float IIR()
{
	float fSum;
	fSum=0.0;
	for ( i=0;i<IIRNUMBER;i++ )
	{
		fSum+=(fXn[i]*fAn[i]);
		fSum+=(fYn[i]*fBn[i]);
	}
	return(fSum);
}

