OBJS = $(wildcard */*.c)
CC = gcc -g -v
INCLUDE_PATHS = -I/usr/local/include
INCLUDE = -I ./include
LIBRARY_PATHS = -L/usr/local/lib
LINKER_FLAGS = -lSDL2 -lSDL2main
OBJ_NAME = main

all:
	$(CC) -o $(OBJ_NAME) ${INCLUDE} $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(OBJS)