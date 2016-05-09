#include "Build Settings.h"

NeuralNet::NeuralNet(int _inputLayerSize, int _hiddenLayerSize, int _outputLayerSize, double _eps = 0, double _eta = 0)
{
	inputLayerSize = _inputLayerSize;
	hiddenLayerSize = _hiddenLayerSize;
	outputLayerSize = _outputLayerSize;
	eps = _eps;
	eta = _eta;
	
	W1.Initalize(inputLayerSize + 1, hiddenLayerSize, 0.1f);
	W2.Initalize(hiddenLayerSize + 1, outputLayerSize, -0.1f);
	dW1.Initalize(inputLayerSize + 1, hiddenLayerSize, 0.f);
	dW2.Initalize(hiddenLayerSize + 1, outputLayerSize, 0.f);
	
	Ain = (double*)malloc((inputLayerSize+ 1)*sizeof(double));
	Aout = (double*)malloc(hiddenLayerSize*sizeof(double));
	Bin = (double*)malloc((hiddenLayerSize + 1)*sizeof(double));
	Bout = (double*)malloc(outputLayerSize*sizeof(double));
	Cin = (double*)malloc(outputLayerSize*sizeof(double));
	trainer = (double*)malloc(outputLayerSize*sizeof(double));
	
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
	free(trainer);
}

void NeuralNet::FeedForward(double* input, double* output) 
{
	for(int i = 0; i < inputLayerSize; i++)
	{
		Ain[i] = input[i];
	}
	
	for(int j = 0; j < hiddenLayerSize; j++)
	{
		Aout[j] = 0.f;
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

	for(int j = 0; j < outputLayerSize; j++)
	{
		Bout[j] = 0.f;
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

void NeuralNet::BackPropagate(double* desired)
{
	double dNet1[hiddenLayerSize + 1];
	double dNet2[outputLayerSize];
	
	for(int i = 0; i < outputLayerSize; i++)
	{
		dNet2[i] = (Cin[i] - desired[i]) * SigPrimeAssisted(Cin[i]);
	}
	
	for(int i = 0; i < outputLayerSize; i++)
	{
		for(int j = 0; j < hiddenLayerSize + 1; j++)
		{
			dW2[j][i] = eta * dNet2[i] * Bout[j];
		}
	}
	
	for(int i = 0; i < hiddenLayerSize + 1; i++)
	{
		dNet1[i] = 0;
		for(int j = 0; j < outputLayerSize; j++)
		{
			dNet1[i] += dNet2[j] * W2[i][j];
		}
		dNet1[i] *= SigPrimeAssisted(Bin[i]);
	}
	
	
	for(int i = 0; i < hiddenLayerSize; i++)
	{
		for(int j = 0; j < inputLayerSize + 1; j++)
		{
			dW1[j][i] = eta * dNet1[i] * Aout[j];
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

double NeuralNet::Sig(double a) 
{
	return 1.0f / (1.0f + exp(-1.0f * a));
}

double NeuralNet::SigPrimeAssisted(double y)
{
	return y * (1.0f - y);
}

NeuralNet::NeuralNet(std::ifstream& input)
{
	std::string readNN;
	input >> readNN;
	if(readNN != "NN")
		throw "Not a net serialization";
	
	input >> inputLayerSize 
		  >> hiddenLayerSize
		  >> outputLayerSize;
		  
	eps = 0;
	eta = 0;
		  
	W1.Initalize(inputLayerSize + 1, hiddenLayerSize, 1.0f);
	W2.Initalize(hiddenLayerSize + 1, outputLayerSize, 1.0f);
	dW1.Initalize(inputLayerSize + 1, hiddenLayerSize, 1.0f);
	dW2.Initalize(hiddenLayerSize + 1, outputLayerSize, 1.0f);
	
	Ain = (double*)malloc((inputLayerSize+ 1)*sizeof(double));
	Aout = (double*)malloc(hiddenLayerSize*sizeof(double));
	Bin = (double*)malloc((hiddenLayerSize + 1)*sizeof(double));
	Bout = (double*)malloc(outputLayerSize*sizeof(double));
	Cin = (double*)malloc(outputLayerSize*sizeof(double));
	
	Ain[inputLayerSize] = 1;
	Bin[inputLayerSize] = 1;
		  
	for(int i = 0; i < inputLayerSize + 1; i++)
	{
		for(int j = 0; j < hiddenLayerSize; j++)
		{
			input >> W1[i][j];
		}
	}
	
	for(int i = 0; i < hiddenLayerSize + 1; i++)
	{
		for(int j = 0; j < outputLayerSize; j++)
		{
			input >> W2[i][j];
		}
	}
}

void NeuralNet::Save(std::ostream& out)
{
	out << "NN " << inputLayerSize << " " 
			<< hiddenLayerSize << " "
			<< outputLayerSize << "\n";
	for(int i = 0; i < inputLayerSize + 1; i++)
	{
		for(int j = 0; j < hiddenLayerSize; j++)
		{
			out << W1[i][j] << " ";
		}
		out << "\n";
	}
	out << "\n";
	
	for(int i = 0; i < hiddenLayerSize + 1; i++)
	{
		for(int j = 0; j < outputLayerSize; j++)
		{
			out << W2[i][j] << " ";
		}
		out << "\n";
	}
}

double Distance(double* a, double* b, int n)
{
	double out = 0;
	for(int i = 0; i < n; i++)
	{
		//std::cout << "trainer: " << a[i] << " desired " << b[i] << std::endl;
		out += (a[i] - b[i]) * (a[i] - b[i]);
	}
	
	return sqrtf(out);
}

void NeuralNet::Train(double* inputs, double* desired, int n)
{
	std::cout << "starting training" << std::endl;
	for(int i = 0; i < n; i++)
	{
		int a = i * inputLayerSize;
		FeedForward(&inputs[a], (double*)trainer);
		while(Distance(trainer, &desired[a], outputLayerSize) > eps)
		{
			std::cout << Distance(trainer, &desired[a], outputLayerSize) << std::endl;
			BackPropagate(&desired[a]);
			FeedForward(&inputs[a], trainer);
		}
	}
}

