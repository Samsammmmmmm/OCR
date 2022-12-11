all:
	gcc -g -c -Wall -Wextra Grid/seg.c
	gcc -g -c -Wall -Wextra Image_processing/filter_funct.c
	gcc -g -c -Wall -Wextra Image_processing/basics.c
	gcc -g -c -Wall -Wextra Rotation/rota.c
	gcc -g -c -Wall -Wextra Graphics/print_solu.c
	gcc -g -c -Wall -Wextra Solver/load_and_save.c
	gcc -g -c -Wall -Wextra Solver/sudokuSolver.c
	gcc -g -c -Wall -Wextra NeuralNetwork/layer.c
	gcc -g -c -Wall -Wextra NeuralNetwork/neuron.c
	gcc -g -c -Wall -Wextra NeuralNetwork/NeuralNetwork.c
	gcc -g -c -Wall -Wextra NeuralNetwork/Load_Save.c
	gcc -g -c -Wall -Wextra NeuralNetwork/Sudoku.c
	gcc -g -c `pkg-config --cflags gtk+-3.0` -Wall -Wextra UI/ui.c
	gcc -Wall -Wextra *.o -lm -lSDL2 -lSDL2_image `pkg-config --libs gtk+-3.0`

clean:
	rm BMP/*.bmp
	rm BMP/*.png
	rm to_solve.*
	rm *.o
	rm a.out
