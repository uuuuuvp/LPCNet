#ifndef _NNET_H_
#define _NNET_H_

#include "vec.h"

#define ACTIVATION_LINEAR  0
#define ACTIVATION_SIGMOID 1
#define ACTIVATION_TANH    2
#define ACTIVATION_RELU    3
#define ACTIVATION_SOFTMAX 4

typedef struct {
  const float *bias;
  const float *input_weights;
  int nb_inputs;
  int nb_neurons;
  int activation;
} DenseLayer;

typedef struct {
  const float *bias;
  const float *input_weights;
  const float *factor;
  int nb_inputs;
  int nb_neurons;
  int nb_channels;
  int activation;
} MDenseLayer;

typedef struct {
  const float *bias;
  const float *subias;
  const qweight *input_weights;
  const int *input_weights_idx;
  const qweight *recurrent_weights;
  int nb_inputs;
  int nb_neurons;
  int activation;
  int reset_after;
} GRULayer;

typedef struct {
  const float *bias;
  const float *subias;
  const float *diag_weights;
  const qweight *recurrent_weights;
  const int *idx;
  int nb_neurons;
  int activation;
  int reset_after;
} SparseGRULayer;

typedef struct {
  const float *bias;
  const float *input_weights;
  int nb_inputs;
  int kernel_size;
  int nb_neurons;
  int activation;
} Conv1DLayer;

typedef struct {
  const float *embedding_weights;
  int nb_inputs;
  int dim;
} EmbeddingLayer;

void compute_activation(float *output, const float *input, int N, int activation);

void compute_dense(const DenseLayer *layer, float *output, const float *input);

void compute_mdense(const MDenseLayer *layer, float *output, const float *input);

int sample_mdense(const MDenseLayer *layer,  const float *input, const float *sampling_logit_table);

void compute_gru(const GRULayer *gru, float *state, const float *input);

void compute_gru2(const GRULayer *gru, float *state, const float *input);

void compute_gruB(const GRULayer *gru, const float* gru_b_condition, float *state, const float *input);

void compute_gru3(const GRULayer *gru, float *state, const float *input);

void compute_sparse_gru(const SparseGRULayer *gru, float *state, const float *input);

void compute_conv1d(const Conv1DLayer *layer, float *output, float *mem, const float *input);

void compute_embedding(const EmbeddingLayer *layer, float *output, int input);

void accum_embedding(const EmbeddingLayer *layer, float *output, int input);

void compute_gru_a_input(float *output, const float *input, int N, const EmbeddingLayer *layer1, int val1, const EmbeddingLayer *layer2, int val2, const EmbeddingLayer *layer3, int val3);

int sample_from_pdf(const float *pdf, int N, float exp_boost, float pdf_floor);

#endif /* _MLP_H_ */
