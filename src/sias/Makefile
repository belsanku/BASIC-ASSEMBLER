#Makefile for sa

CC = gcc

CFLAGS = -Wall -Werror -std=c99 
FLAGS = -o
BUILD = build/

all: build $(BUILD)sias.a

$(BUILD)sias.a: $(BUILD)asm.o
	ar cr $(BUILD)sias.a $(BUILD)asm.o

$(BUILD)asm.o: asm.c asm.h
	$(CC) $(CFLAGS) -c asm.c -o $@

build:
	mkdir build

clear:
	-rm -rf build