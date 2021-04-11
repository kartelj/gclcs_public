#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

#include "random.h"

static unsigned long int next;


unsigned int irand(void)
{
    step_next;
    return ((unsigned int) (next / 0x1000) & 0xFFFF);
}

unsigned long lrand(void)
{
    return ((((unsigned long) irand()) << 16) | irand());
}

unsigned long lrandp(double p)
{ int i;
  unsigned long int r=0;
  
for(i=0;i<32;i++)
  r |= Flip(p) << i;
return r;    
}

unsigned long lrandpq(double p, double q)
{
	int i;
    unsigned long int r=0;

    for(i=0;i<32;i+=2)
	{
		r |= (Flip(p) << (i));
	}

	for(i=0;i<32;i+=2)
	{
		r |= (Flip(q) << (i+1));		
	}

    
	return r;    


}


void isrand(unsigned int seed)
{
    next = seed;
}

void irandomize (void)
{
	next = (unsigned long)time(NULL);
}


void SetRandomSeed(unsigned int rnd)
{
  time_t t;

  if(rnd<=0) rnd = (unsigned)time(&t);
  isrand(rnd);
}

/***************************************************************\
 DoubleRoundToInt()

 Nalazi najblizi ceo broj za dati double broj
\***************************************************************/
int DoubleRoundToInt( double dblNum)
{
	int intLo, intHi, intMid;

	if ( dblNum >= INT_MAX )
      return INT_MAX;
	if ( dblNum <= INT_MIN )
      return INT_MIN;
	intLo = 0;
	intHi = INT_MAX/4;
	while ( intHi - intLo > 1 )
   {
		intMid = ( intLo + intHi ) / 2;
		if ( intMid - dblNum < 0 )
			intHi = intMid;
      else
			intLo = intMid;
   }
	if ( fabs( dblNum - intLo ) < fabs( dblNum - intHi ) )
		return intLo;
   else
		return intHi;
}  /* DoubleRoundToInt */

