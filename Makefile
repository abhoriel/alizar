CC=gcc
CFLAGS=-Iinclude -Iworldgen/include -std=c99 -Wall -Wextra -g#-Wno-sign-compare
LDFLAGS= -lSDL2 -lSDL2_image -lm
	
OBJ=main.o log.o sdl.o tile.o world.o object.o worldgen/terrain.o
BIN=alizarin

all: $(OBJ) $(BIN)
	
$(BIN): $(OBJ) 
	$(CC) $(LDFLAGS) $(OBJ) -o $@

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

