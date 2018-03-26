SRC=level2.c
BIN=level2

$(BIN): $(SRC)
	gcc -Wall -g -std=c99 $(SRC) `sdl-config --cflags --libs` -o $(BIN) -lm

clean:
	rm -f $(BIN)
