/**
   @file pitch.c
   @brief Pitch analysis
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pitch.h"
#include "common.h"
#include "celt_lpc.h"
#include "math.h"

void celt_pitch_xcorr(const opus_val16 *_x, const opus_val16 *_y,
      opus_val32 *xcorr, int len, int max_pitch)
{

#if 0 /* This is a simple version of the pitch correlation that should work
         well on DSPs like Blackfin and TI C5x/C6x */
   int i, j;
   for (i=0;i<max_pitch;i++)
   {
      opus_val32 sum = 0;
      for (j=0;j<len;j++)
         sum = MAC16_16(sum, _x[j], _y[i+j]);
      xcorr[i] = sum;
   }

#else /* Unrolled version of the pitch correlation -- runs faster on x86 and ARM */
   int i;
   /*The EDSP version requires that max_pitch is at least 1, and that _x is
      32-bit aligned.
     Since it's hard to put asserts in assembly, put them here.*/

   celt_assert(max_pitch>0);
   celt_assert((((unsigned char *)_x-(unsigned char *)NULL)&3)==0);
   for (i=0;i<max_pitch-3;i+=4)
   {
      opus_val32 sum[4]={0,0,0,0};
      xcorr_kernel(_x, _y+i, sum, len);
      xcorr[i]=sum[0];
      xcorr[i+1]=sum[1];
      xcorr[i+2]=sum[2];
      xcorr[i+3]=sum[3];
   }
   /* In case max_pitch isn't a multiple of 4, do non-unrolled version. */
   for (;i<max_pitch;i++)
   {
      opus_val32 sum;
      sum = celt_inner_prod(_x, _y+i, len);
      xcorr[i] = sum;
   }
#endif
}

