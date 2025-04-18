all: main.o
	cc -o starama -L./ -lSDL3 ./obj/main.o

main.o: ./src/main.c
	cc -o ./obj/main.o -I./include/ -c ./src/main.c
