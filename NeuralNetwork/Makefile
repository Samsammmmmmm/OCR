xor:
	gcc -c -Wall -Wextra layer.c
	gcc -c -Wall -Wextra neuron.c
	gcc -c -Wall -Wextra NeuralNetwork.c
	gcc -c -Wall -Wextra Load_Save.c
	gcc -c -Wall -Wextra Xor.c
	gcc -Wall -Wextra -o Xor.out Xor.o Load_Save.o NeuralNetwork.o layer.o neuron.o -lm
all:
	gcc -c -g -Wall -Wextra layer.c
	gcc -c -g -Wall -Wextra neuron.c
	gcc -c -g -Wall -Wextra NeuralNetwork.c
	gcc -c -g -Wall -Wextra Load_Save.c
	gcc -c -g -Wall -Wextra Sudoku.c
	gcc -Wall -Wextra Sudoku.o Load_Save.o NeuralNetwork.o layer.o neuron.o -lm -lSDL2 -lSDL2_image
clean:
	rm *.o Weights/weights_* *.out