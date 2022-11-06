rota:
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`   -c -o rota.o rota.c
	gcc   rota.o  `pkg-config --libs sdl2 SDL2_image` -o rota

all:
	gcc -Wall -Wextra -c *.c
	gcc *.o -o main -lSDL2 -lSDL2_image

clean:
	rm -rf *[.do] main rota *~
