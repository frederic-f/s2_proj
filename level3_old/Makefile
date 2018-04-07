# compiler definition
CC=gcc

# compilation options to produce .o file
FLAGS = -Wall -g -std=c99 `sdl-config --cflags --libs` -lm

level: level.o bub.o ; $(CC) -o level level.o bub.o

level.o: level.c bub.h ; $(CC) $(FLAGS) level.c

bub.o: bub.c bub.h ; $(CC) $(FLAGS) bub.c