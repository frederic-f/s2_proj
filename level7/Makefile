SRC=main.c
SYSh=sys.h
SYSc=sys.c
BUBh=bub.h
BUBc=bub.c
GAMh=game.h
GAMc=game.c
CONh=data.h
BIN=main

$(BIN): $(SRC) $(CONh) $(SYSh) $(SYSc) $(BUBh) $(BUBc) $(GAMh) $(GAMc) ; gcc -Wall -g -std=c99 $(SRC) $(CONh) $(SYSh) $(SYSc) $(BUBh) $(BUBc) $(GAMh) $(GAMc) `sdl-config --cflags --libs` -o $(BIN) -lm

clean: rm -f $(BIN)

