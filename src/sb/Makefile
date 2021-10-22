#Makefile for sb

CC = gcc

CFLAGS = -Wall -Werror -std=c99 
FLAGS = -o
BUILD = build/

all: build $(BUILD)sb.a

$(BUILD)sb.a: $(BUILD)basic.o
	ar cr $(BUILD)sb.a $(BUILD)basic.o

$(BUILD)basic.o: basic.c basic.h
	$(CC) $(CFLAGS) -c basic.c -o $@

build:
	mkdir build

clear:
	-rm -rf build