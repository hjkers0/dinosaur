#include "nn.h"

void n_init(Neuron *n, int size)
{
    n->size_w = size;
    n->weights = malloc(size * sizeof(float)); 
    
    for (int i = 0; i < size; i++)
    {
        n->weights[i] = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
    }
    n->bias = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
}


void n_free(Neuron *n)
{
	free(n->weights);
	n->weights = NULL;
	n->bias = 0;
	n->size_w = 0;
}

// Activate function for Neuron
float n_activate(Neuron *n, float inputs[])
{
	float sum = n->bias;
	for (int i = 0; i < n->size_w ; i++)
	{
		sum += inputs[i] * n->weights[i];
	}
	return tanhf(sum); 
}

// Mutate function for Neuron
void n_mutate(Neuron *n, float rate)
{
	for (int i = 0; i < n->size_w ; i++)
	{
		if (rand() < rate)
		{
			n->weights[i] += ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
		}
	}
}

// Clone Neuron
void n_clone(Neuron *n)
{
	Neuron new_n = { malloc(n->size_w * sizeof(float)), n->size_w, n->bias };
}


