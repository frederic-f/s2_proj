SRC=level3.c
HDR=bub.h
FCT=bub.c
BIN=main

$(BIN): $(SRC) $(HDR) $(FCT) ; gcc -Wall -g -std=c99 $(SRC) $(HDR) $(FCT) `sdl-config --cflags --libs` -o $(BIN) -lm

clean: rm -f $(BIN)

