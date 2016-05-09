import math
import random
import numpy as np
np.set_printoptions(threshold=np.nan)
import sys

''' read arguments '''

eps = None
eta = None
epochs = None
inputlayer = int(sys.argv[1])
hiddenlayer = int(sys.argv[2])
outputlayer = int(sys.argv[3])
eps = float(sys.argv[4])
eta = float(sys.argv[5])
epochs = 1
skipCount = 5
files = [sys.argv[6]]

		
def data_from_files(files, inputlayer, outputlayer):
	for file in files:
		with open(file, "rb") as f:
			input = []
			output = []
			skip = 0
			while True:
				chunk = f.read(1)
				if skip == 0:
					if chunk:
						if len(input) < inputlayer:
							input.append(int(chunk))
						elif len(output) < outputlayer:
							output.append(int(chunk))
						else:
							yield input, output
							skip = skipCount * (inputlayer +outputlayer)
							input = []
							output = []
				elif chunk:
					skip += -1
				else:
					break

''' Neural Network '''

def sig(x):
	return 1 / (1 + np.exp(-1 * x))

class NeuralNet:
	def __init__(self, inputlayer, hiddenlayer, outputlayer):
		self.n = hiddenlayer
		self.d = inputlayer
		self.c = outputlayer
		self.W1 = np.subtract ( np.random.rand(self.n, self.d+1), np.full((self.n, self.d+1), .5) )
		self.W2 = np.subtract ( np.random.rand(self.c, self.n+1), np.full((self.c, self.n+1), .5) )
		
	def feedForward(self, datum):
		A = np.array(datum + [1])
		B = np.append(sig(np.dot(self.W1, A)), 1)
		C = sig(np.dot(self.W2, B))
		
		result = C.tolist()
		return result, A, B, C
	
	def backPropagate(self, A, B, C, yhat, eta):
		dEdC = yhat - C
		
		dNet2 = dEdC * C * (1 - C)
		dW2 = eta * np.dot(dNet2[:, None], np.transpose(B[:, None]))
		
		dNet1 = B * (1 - B) * np.dot(np.transpose(self.W2), dNet2)
		B = np.delete(B, self.n, 0)
		dNet1 = np.delete(dNet1, self.n, 0)
		dW1 = eta * np.dot(dNet1[:, None], np.transpose(A[:, None]))

		self.W1 = np.add(self.W1, dW1)
		self.W2 = np.add(self.W2, dW2)
		
	def processData(self, datum, desired, eta):
		c = 0
		while True:
			result, A, B, C = self.feedForward(datum)
			E = np.linalg.norm(np.subtract(desired, result)) / 2
			c += 1
			if c % 1000 == 0:
				print >> sys.stderr, "\t", c, E
			if c > 10000:
				break
			if math.isnan(E):
				sys.exit(1)
			if E < eps:
				break
			else:
				self.backPropagate(A, B, C, desired, eta)
			
		
net = NeuralNet(inputlayer, hiddenlayer, outputlayer)

print >> sys.stderr, "Training..."
try:
	for i in range(epochs):
		print >> sys.stderr, "Run", i + 1, "..."
		j = 0
		for (input, output) in data_from_files(files, inputlayer, outputlayer):
			j += 1
			if j%100 == 0:
				print >> sys.stderr, j, "done"
			net.processData(input, output, eta)
		if j == 1000000:
			break
		print j
except KeyboardInterrupt:
	pass

print "NN", inputlayer, hiddenlayer, outputlayer	

print net.W1

print net.W2