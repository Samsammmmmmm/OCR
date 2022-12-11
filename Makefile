all:
	gcc -c -Wall -Wextra Grid/seg.c
	gcc -c -Wall -Wextra Image_processing/filter_funct.c
	gcc -c -Wall -Wextra Image_processing/basics.c
	gcc -c -Wall -Wextra Rotation/rota.c
	gcc -c -Wall -Wextra Graphics/print_solu.c
	gcc -c -Wall -Wextra Solver/load_and_save.c
	gcc -c -Wall -Wextra Solver/sudokuSolver.c
	gcc -c -Wall -Wextra NeuralNetwork/layer.c
	gcc -c -Wall -Wextra NeuralNetwork/neuron.c
	gcc -c -Wall -Wextra NeuralNetwork/NeuralNetwork.c
	gcc -c -Wall -Wextra NeuralNetwork/Load_Save.c
	gcc -c -Wall -Wextra NeuralNetwork/Sudoku.c
	gcc -c `pkg-config --cflags gtk+-3.0` -Wall -Wextra UI/ui.c
	gcc -Wall -Wextra *.o -O3 -lm -lSDL2 -lSDL2_image `pkg-config --libs gtk+-3.0`

clean:
	rm BMP/*.bmp
	rm BMP/*.png
	rm to_solve.*
	rm *.o
	rm a.out
