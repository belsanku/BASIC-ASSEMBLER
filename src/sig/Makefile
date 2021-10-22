#Makefile for sa

CC = gcc

CFLAGS = -Wall -Werror -std=c99 
FLAGS = -o
BUILD = build/

all: build $(BUILD)sig.a

$(BUILD)sig.a: $(BUILD)signal.o
	ar cr $(BUILD)sig.a $(BUILD)signal.o

$(BUILD)signal.o: signal.c signal.h
	$(CC) $(CFLAGS) -c signal.c -o $@

build:
	mkdir build

clear:
	-rm -rf build