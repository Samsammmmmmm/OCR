all:
	gcc -c -Wall -Wextra layer.c
	gcc -c -Wall -Wextra neuron.c
	gcc -c -Wall -Wextra NeuralNetwork.c
	gcc -c -Wall -Wextra Load_Save.c
	gcc -c -Wall -Wextra Xor.c
	gcc -Wall -Wextra -o Xor.out Xor.o Load_Save.o NeuralNetwork.o layer.o neuron.o -lm
clean:
	rm *.o weights.txt Xor.out