#makefile pour tuto pthread

CC=gcc
LK=gcc
CCFLAGS=-W -Wall -g -ansi -pedantic -D_REENTRANT
LKFLAGS=-g
LIBS=-lpthread

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

EXE=app_thread.exe

all:$(EXE)

$(EXE):$(OBJ)
	@($(LK) -o $@ $^ $(LKFLAGS) $(LIBS))

%.o:%.c
	@($(CC) -c $< $(CCFLAGS))

.PHONY: clean mrproper

clean:
	@(rm -rf *.o)

mrproper:clean
	@(rm $(EXE))


