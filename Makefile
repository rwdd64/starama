TARGET=starama
CFLAGS=-ggdb -Wall -Wextra -Wpedantic
LDFLAGS=-L./ -lSDL3
CPPFLAGS=-I./include/

all: main.o
	$(CC) -o $(TARGET) $(LDFLAGS) ./obj/$?

main.o: ./src/main.c
	$(CC) -o ./obj/$@ -c $(CFLAGS) $(CPPFLAGS) $<
