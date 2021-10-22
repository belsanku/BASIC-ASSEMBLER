#Makefile for Static Library
#СС указатель на компилятор для сборки
CC=gcc
#FLAGS Флажки для компиляции
CFLAGS=-Wall -Werror

.PHONY: clean

all: myComputer

myComputer: project src/main.o src/mc/build/mc.a  src/msc/build/msc.a src/mt/build/mt.a src/mbg/build/mbg.a src/mk/build/mk.a src/sa/build/sa.a src/sias/build/sias.a src/sb/build/sb.a
	$(CC) src/main.o -L. src/mc/build/mc.a src/msc/build/msc.a src/mt/build/mt.a src/mbg/build/mbg.a  src/mk/build/mk.a src/sa/build/sa.a src/sias/build/sias.a src/sb/build/sb.a -o $@

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o $@

project: 
	make msca
	make mta
	make mbga
	make mka
	make mca
	make sa
	make sias
	make sb
msca: 
	cd src/msc/ && make all
mta: 
	cd src/mt/ && make all
mbga: 
	cd src/mbg/ && make all
mka: 
	cd src/mk/ && make all
mca: 
	cd src/mc/ && make all
sa:
	cd src/sa/ && make all
sias:
	cd src/sias/ && make all
sb:
	cd src/sb/ && make all


clear:
	-rm src/main.o
	cd src/msc/ && make clear
	cd src/mt/ && make clear
	cd src/mbg/ && make clear
	cd src/mk/ && make clear
	cd src/mc/ && make clear
	cd src/sa/ && make clear
	cd src/sias/ && make clear
	cd src/sb/ && make clear
	-rm myComputer