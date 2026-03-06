#ifndef NN_H_
#define NN_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	float	*weights;
	int		size_w; 
	float	bias;
} Neuron;

typedef struct {
	int		inputSize;
	int		hiddenSize;
	Neuron	*hidden;
	Neuron	output;
} NeuralNetwork;

void			print_nn(NeuralNetwork nn);

void			nn_init(NeuralNetwork *nn, int inputSize, int hiddenSize);
void			nn_free(NeuralNetwork *nn);
float			nn_predict(NeuralNetwork *nn, float inputs[]);
void			nn_mutate(NeuralNetwork *nn, float rate);
NeuralNetwork	nn_clone(NeuralNetwork *nn);

void	n_init(Neuron *n, int size);
void	n_free(Neuron *n);
float	n_activate(Neuron *n, float inputs[]);
void	n_mutate(Neuron *n, float rate);
Neuron	n_clone(Neuron *n);

#endif
