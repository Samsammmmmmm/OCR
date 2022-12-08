rota:
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`   -c -o rota.o rota.c -g
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`   -c -o basics.o ../Tools/basics.c -g
	gcc   rota.o basics.o  `pkg-config --libs sdl2 SDL2_image` -o rota -lm -g

all:
	gcc -Wall -Wextra -c *.c
	gcc *.o -o main -lSDL2 -lSDL2_image

clean:
	rm -rf *[.do] main rota *~
