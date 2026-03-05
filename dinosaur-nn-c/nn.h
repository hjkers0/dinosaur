#ifndef NN_H_
#define NN_H_

#include <math.h>
#include <stdlib.h>

typedef struct {
	float	*weights;
	int		size_w; 
	float bias;
} Neuron;

void n_init(Neuron *n, int size);
void n_free(Neuron *n);
float n_activate(Neuron *n, float inputs[]);
void n_mutate(Neuron *n, float rate);
void n_clone(Neuron *n);

#endif
