#include "Build Settings.h"

class NeuralNet 
{
	public:
		NeuralNet(int _inputLayerSize, int _hiddenLayerSize, int _outputLayerSize);
		NeuralNet(std::ifstream& input);
		~NeuralNet();
		
		void Save(std::ostream& output);
		
		void FeedForward(float* input, float* output);
		void BackPropagate(float* desired);
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
		int inputLayerSize;
		int hiddenLayerSize;
		int outputLayerSize;
		float Sig(float a);
		float SigPrimeAssisted(float y);
};