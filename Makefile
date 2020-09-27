INCLUDES = -I ./include
FLAGS = -g -Wall
LIBS = -L ./lib -lSDL2main -lSDL2
OBJECTS = ./build/memory.o ./build/stack.o ./build/keyboard.o ./build/chip8.o ./build/screen.o

all: ${OBJECTS}
	gcc ${INCLUDES} ${FLAGS} ${LIBS} ./src/main.c ${OBJECTS} -o ./bin/chip8

./build/memory.o:src/memory.c
	gcc ${INCLUDES} ${FLAGS} ./src/memory.c -c -o ./build/memory.o

./build/stack.o:src/stack.c
	gcc ${INCLUDES} ${FLAGS} ./src/stack.c -c -o ./build/stack.o

./build/keyboard.o:src/keyboard.c
	gcc ${INCLUDES} ${FLAGS} ./src/keyboard.c -c -o ./build/keyboard.o

./build/chip8.o:src/chip8.c
	gcc ${INCLUDES} ${FLAGS} ./src/chip8.c -c -o ./build/chip8.o

./build/screen.o:src/screen.c
	gcc ${INCLUDES} ${FLAGS} ./src/screen.c -c -o ./build/screen.o

clean:
	rm -rf ./build/*
	rm -rf ./bin/*