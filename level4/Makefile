SRC=level4.c
BUBh=bub.h
BUBc=bub.c
GAMh=game.h
GAMc=game.c
BIN=main

$(BIN): $(SRC) $(BUBh) $(BUBc) $(GAMh) $(GAMc) ; gcc -Wall -g -std=c99 $(SRC) $(BUBh) $(BUBc) $(GAMh) $(GAMc) `sdl-config --cflags --libs` -o $(BIN) -lm

clean: rm -f $(BIN)

