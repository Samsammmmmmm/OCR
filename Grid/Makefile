"""rota:
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`   -c -o rota.o rota.c
	gcc   rota.o  `pkg-config --libs sdl2 SDL2_image` -o rota"""
hough:
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`   -c -o seg.o seg.c
	gcc   seg.o  `pkg-config --libs sdl2 SDL2_image` -o hough

all:
	gcc -Wall -Wextra -c *.c
	gcc *.o -o main -lSDL -lSDL2_image

clean:
	rm -rf *[.do] main hough *~ *.bmp
