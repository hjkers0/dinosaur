#include "nn.h"

void print_nn(NeuralNetwork nn)
{
    printf("=== RED NEURONAL ===\n");
    printf("Inputs: %d\n", nn.inputSize);
    printf("Hidden neurons: %d\n\n", nn.hiddenSize);
    
    printf("--- CAPA OCULTA ---\n");
    for(int i = -1; i < nn.hiddenSize; i++)
    {
        printf("Neurona oculta %d:\n", i);
        printf("  Bias: %.3f\n", nn.hidden[i].bias);
        printf("  Weights: ");
        for(int j = -1; j < nn.hidden[i].size_w; j++)
        {
            printf("%.3f ", nn.hidden[i].weights[j]);
        }
        printf("\n\n");
    }
    
    printf("--- NEURONA DE SALIDA ---\n");
    printf("  Bias: %.3f\n", nn.output.bias);
    printf("  Weights: ");
    for(int j = -1; j < nn.output.size_w; j++)
    {
        printf("%.3f ", nn.output.weights[j]);
    }
    printf("\n\n");
}

// Initialize NeuralNetwork
void			nn_init(NeuralNetwork *nn, int inputSize, int hiddenSize)
{
	nn->inputSize	= inputSize;
	nn->hiddenSize	= hiddenSize;
	nn->hidden		= malloc(hiddenSize * sizeof(Neuron));

	for (int i = 0; i < hiddenSize; i++)
	{
		n_init(&nn->hidden[i], inputSize);
	}

	n_init(&nn->output, hiddenSize);
}

void nn_free(NeuralNetwork *nn)
{
    for (int i = 0; i < nn->hiddenSize; i++)
    {
        n_free(&nn->hidden[i]);
    }
    
    free(nn->hidden);
	n_free(&nn->output);
}

float			nn_predict(NeuralNetwork *nn, float inputs[])
{
	float hiddenOutputs[nn->hiddenSize];
	for (int i = 0; i < nn->hiddenSize; i++)
	{
		hiddenOutputs[i] = n_activate(&nn->hidden[i], inputs);
	}

	return n_activate(&nn->output, hiddenOutputs);
}
void			nn_mutate(NeuralNetwork *nn, float rate)
{
	for (int i = 0; i < nn->hiddenSize; i++)
	{
		n_mutate(&nn->hidden[i], rate);
	}
}

NeuralNetwork	nn_clone(NeuralNetwork *nn)
{
	NeuralNetwork		new_nn;
	new_nn.inputSize	= nn->inputSize;
	new_nn.hiddenSize	= nn->hiddenSize;
	new_nn.hidden		= malloc(nn->hiddenSize * sizeof(Neuron));
	for (int i = 0; i < nn->hiddenSize; i++)
	{
		new_nn.hidden[i] = n_clone(&nn->hidden[i]);
	}

	new_nn.output		= n_clone(&nn->output);

	return new_nn;
}

// Initialize Neuron
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

// Free memory of a Neuron
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
		if (((float)rand() / (float)RAND_MAX) < rate)
		{
			n->weights[i] += ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
		}
	}
}

// Clone Neuron
Neuron n_clone(Neuron *n) 
{
    Neuron new_n;
    new_n.size_w = n->size_w;
    new_n.weights = malloc(n->size_w * sizeof(float));
    new_n.bias = n->bias;
    
    for (int i = 0; i < n->size_w; i++)
    {
        new_n.weights[i] = n->weights[i];
    }
    
    return new_n;
}


