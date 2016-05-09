#include "Build Settings.h"

#ifndef NEURALNET
#define NEURALNET

class NeuralNet 
{
	public:
		NeuralNet(int _inputLayerSize, int _hiddenLayerSize, int _outputLayerSize, float _eps, float _eta);
		NeuralNet(std::ifstream& input);
		~NeuralNet();
		
		void Save(std::ostream& output);
		
		void FeedForward(float* input, float* output);
		void BackPropagate(float* desired);
		void Train(float* inputs, float* desired, int n);
	private:
		Matrix W1;
		Matrix W2;
		Matrix dW1;
		Matrix dW2;
		float* Ain;
		float* Aout;
		float* Bin;
		float* Bout;
		float* Cin;
		float eps;
		float eta;
		float* trainer;
		int inputLayerSize;
		int hiddenLayerSize;
		int outputLayerSize;
		float Sig(float a);
		float SigPrimeAssisted(float y);
};

#endif
