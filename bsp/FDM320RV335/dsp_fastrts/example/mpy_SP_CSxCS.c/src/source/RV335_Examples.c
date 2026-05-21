#include "RV335_Examples.h"

// rv implement delay, not accurate
void RV335_usDelay (int Count)
{
	// divide 4 for time accurate. by calculate
	Count /= 4;

	while(Count >= 0)
	{
		Count--;
	}
}
