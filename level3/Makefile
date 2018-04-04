SRC=level.c
BIN=level

$(BIN): $(SRC)
	gcc -Wall -g -std=c99 $(SRC) `sdl-config --cflags --libs` -o $(BIN) -lm

clean:
	rm -f $(BIN)
