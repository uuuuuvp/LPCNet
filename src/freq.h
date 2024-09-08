#include "kiss_fft.h"

#define PREEMPHASIS (0.85f)

#define FRAME_SIZE_5MS (2)
#define OVERLAP_SIZE_5MS (2)
#define TRAINING_OFFSET_5MS (1)

#define WINDOW_SIZE_5MS (FRAME_SIZE_5MS + OVERLAP_SIZE_5MS)

#define FRAME_SIZE (40*FRAME_SIZE_5MS)
#define OVERLAP_SIZE (40*OVERLAP_SIZE_5MS)
#define TRAINING_OFFSET (40*TRAINING_OFFSET_5MS)
#define WINDOW_SIZE (FRAME_SIZE + OVERLAP_SIZE)
#define FREQ_SIZE (WINDOW_SIZE/2 + 1)

#define NB_BANDS 14
#define NB_BANDS_1 (NB_BANDS - 1)

void compute_band_energy(float *bandE, const kiss_fft_cpx *X);
void compute_band_corr(float *bandE, const kiss_fft_cpx *X, const kiss_fft_cpx *P);

void apply_window(float *x);
void dct(float *out, const float *in);
void idct(float *out, const float *in);
void forward_transform(kiss_fft_cpx *out, const float *in);
void inverse_transform(float *out, const kiss_fft_cpx *in);
float lpc_from_bands(float *lpc, const float *Ex);
float lpc_from_cepstrum(float *lpc, const float *cepstrum);
void apply_window(float *x);

