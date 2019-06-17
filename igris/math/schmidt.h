#ifndef IGRIS_SCHMIDT_H
#define IGRIS_SCHMIDT_H

#include <stdbool.h>

struct incremental_schmidt 
{
	int cur;
	int lo;
	int hi;
	int max;
	uint8_t state; 

	schmidt(int lo, int hi, int max) : 
		lo(lo), 
		hi(hi), 
		max(max), 
		cur(0), 
		state(0) 
	{}

	int check(int val) 
	{
		cur += val;

		if (cur < 0) cur = 0;
		if (cur > max) cur = max;

		if (state == 0) 
		{
			if (cur < hi) 
				return 0;

			else 
			{
				state = 1;
				return 1;
			}
		}
		else 
		{
			if (cur > lo) 
				return 1;

			else 
			{
				state = 0;
				return 0;
			}	
		}
	};
};

#endif