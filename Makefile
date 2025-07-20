CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -g
LIBS = `pkg-config --libs gtk+-3.0` -ltag_c

SRC = main.c ui.c mp3_handler.c
OBJ = $(SRC:.c=.o)
BIN = mp3editor

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) *.o

run:
	./mp3editor