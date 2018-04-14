SRC=level4.c
BUBh=bub.h
BUBc=bub.c
GAMh=game.h
GAMc=game.c
CONh=data.h
BIN=main

$(BIN): $(SRC) $(CONh) $(BUBh) $(BUBc) $(GAMh) $(GAMc) ; gcc -Wall -g -std=c99 $(SRC) $(CONh) $(BUBh) $(BUBc) $(GAMh) $(GAMc) `sdl-config --cflags --libs` -o $(BIN) -lm

clean: rm -f $(BIN)

