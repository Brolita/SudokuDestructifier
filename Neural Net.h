#include "Build Settings.h"

#ifndef NEURALNET
#define NEURALNET

class NeuralNet 
{
	public:
		NeuralNet(int _inputLayerSize, int _hiddenLayerSize, int _outputLayerSize, double _eps, double _eta);
		NeuralNet(std::ifstream& input);
		~NeuralNet();
		
		void Save(std::ostream& output);
		
		void FeedForward(double* input, double* output);
		void BackPropagate(double* desired);
		void Train(double* inputs, double* desired, int n);
	private:
		Matrix W1;
		Matrix W2;
		Matrix dW1;
		Matrix dW2;
		double* Ain;
		double* Aout;
		double* Bin;
		double* Bout;
		double* Cin;
		double eps;
		double eta;
		double* trainer;
		int inputLayerSize;
		int hiddenLayerSize;
		int outputLayerSize;
		double Sig(double a);
		double SigPrimeAssisted(double y);
};

#endif
