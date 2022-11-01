all:
	gcc -c layer.c
	gcc -c neuron.c
	gcc -c NeuralNetwork.c
	gcc -c Xor.c
	gcc -g -o Xor.out Xor.o NeuralNetwork.o layer.o neuron.o -lm
clean:
	rm *.o Xor.out