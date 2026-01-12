CC=cc
CFLAGS=-Wall -Werror
LIBS=-lm -lGL -lglfw

.PHONY: all
all: drawer

drawer: drawer.c
	$(CC) $(CFLAGS) -o drawer drawer.c $(LIBS)

install:
	install -m 755 drawer /usr/bin

