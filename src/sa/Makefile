#Makefile for sa

CC = gcc

CFLAGS = -Wall -Werror -std=c99 
FLAGS = -o
BUILD = build/

all: build $(BUILD)sa.a

$(BUILD)sa.a: $(BUILD)Cu.o
	ar cr $(BUILD)sa.a $(BUILD)Cu.o

$(BUILD)Cu.o: Cu.c Cu.h
	$(CC) $(CFLAGS) -c Cu.c -o $@

build:
	mkdir build

clear:
	-rm -rf build