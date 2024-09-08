#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "freq.h"
#include "lpcnet_private.h"


static void single_interp(float *x, const float *left, const float *right, int id)
{
    int i;
    float ref[NB_BANDS];
    float pred[3*NB_BANDS];
    RNN_COPY(ref, x, NB_BANDS);
    for (i=0;i<NB_BANDS;i++) pred[i] = .5*(left[i] + right[i]);
    for (i=0;i<NB_BANDS;i++) pred[NB_BANDS+i] = left[i];
    for (i=0;i<NB_BANDS;i++) pred[2*NB_BANDS+i] = right[i];
    for (i=0;i<NB_BANDS;i++) {
      x[i] = pred[id*NB_BANDS + i];
    }
    if (0) {
        float err = 0;
        for (i=0;i<NB_BANDS;i++) {
            err += (x[i]-ref[i])*(x[i]-ref[i]);
        }
        printf("%f\n", sqrt(err/NB_BANDS));
    }
}

void perform_double_interp(float features[4][NB_TOTAL_FEATURES], const float *mem, int best_id) {
    int id0, id1;
    best_id += (best_id >= FORBIDDEN_INTERP);
    id0 = best_id / 3;
    id1 = best_id % 3;
    single_interp(features[0], mem, features[1], id0);
    single_interp(features[2], features[1], features[3], id1);
}
