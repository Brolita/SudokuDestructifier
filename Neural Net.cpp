#include "Build Settings.h"

NeuralNet::NeuralNet(int _inputLayerSize, int _hiddenLayerSize, int _outputLayerSize)
{
	inputLayerSize = _inputLayerSize;
	hiddenLayerSize = _hiddenLayerSize;
	outputLayerSize = _outputLayerSize;
	
	W1.Initalize(inputLayerSize + 1, hiddenLayerSize, 1.0f);
	W2.Initalize(hiddenLayerSize + 1, outputLayerSize, 1.0f);
	dW1.Initalize(inputLayerSize + 1, hiddenLayerSize, 1.0f);
	dW2.Initalize(hiddenLayerSize + 1, outputLayerSize, 1.0f);
	
	Ain = (float*)malloc((inputLayerSize+ 1)*sizeof(float));
	Aout = (float*)malloc(hiddenLayerSize*sizeof(float));
	Bin = (float*)malloc((hiddenLayerSize + 1)*sizeof(float));
	Bout = (float*)malloc(outputLayerSize*sizeof(float));
	Cin = (float*)malloc(outputLayerSize*sizeof(float));
	
	Ain[inputLayerSize] = 1;
	Bin[inputLayerSize] = 1;
}

NeuralNet::~NeuralNet()
{
	free(Ain);
	free(Aout);
	free(Bin);
	free(Bout);
	free(Cin);
}

void NeuralNet::FeedForward(float* input, float* output) 
{
	for(int i = 0; i < inputLayerSize; i++)
	{
		Ain[i] = input[i];
	}
	
	for(int i = 0; i < inputLayerSize + 1; i++)
	{
		for(int j = 0; j < hiddenLayerSize; j++)
		{
			Aout[j] += Ain[i] * W1[i][j];
		}
	}
	
	for(int i = 0; i < hiddenLayerSize; i++)
	{
		Bin[i] = Sig(Aout[i]);
	}
	
	for(int i = 0; i < hiddenLayerSize + 1; i++)
	{
		for(int j = 0; j < outputLayerSize; j++)
		{
			Bout[j] += Bin[i] * W2[i][j];
		}
	}
	
	for(int i = 0; i < outputLayerSize; i++)
	{
		Cin[i] = Sig(Bout[i]);
		output[i] = Cin[i];
	}
}

void NeuralNet::BackPropagate(float* desired)
{
	float dNet1[hiddenLayerSize + 1];
	float dNet2[outputLayerSize];
	
	for(int i = 0; i < outputLayerSize; i++)
	{
		dNet2[i] = (desired[i] - Cin[i]) * SigPrimeAssisted(Cin[i], Bout[i]);
		for(int j = 0; j < hiddenLayerSize + 1; j++)
		{
			dW2[j][i] = ETA * dNet2[i] * Bout[j];
		}
	}
	
	for(int i = 0; i < hiddenLayerSize + 1; i++)
	{
		dNet1[i] = 0;
		for(int j = 0; j < outputLayerSize; j++)
		{
			dNet1[i] += dNet2[j] * W2[i][j];
		}
		dNet1[i] *= SigPrimeAssisted(Bin[i], Aout[i]);
	}
	
	
	for(int i = 0; i < hiddenLayerSize; i++)
	{
		for(int j = 0; j < inputLayerSize + 1; j++)
		{
			dW1[j][i] = ETA * dNet1[i] * Aout[j];
		}
	}
	
	for(int i = 0; i < inputLayerSize + 1; i++)
	{
		for(int j = 0; j < hiddenLayerSize; j++)
		{
			W1[i][j] += dW1[i][j];
		}
	}
	
	for(int i = 0; i < hiddenLayerSize + 1; i++)
	{
		for(int j = 0; j < outputLayerSize; j++)
		{
			W2[i][j] += dW2[i][j];
		}
	}
}

float NeuralNet::Sig(float a) 
{
	return a / (1 + fabs(a));
}

float NeuralNet::SigPrimeAssisted(float y, float x)
{
	return y * y / (x * x);
}