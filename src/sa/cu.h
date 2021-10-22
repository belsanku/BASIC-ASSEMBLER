#ifndef CU_H
#define CU_H
#include "../mc/myConsole.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>



#define READ 0x10
#define WRITE 0x11
#define LOAD 0x20
#define STORE 0x21
#define ADD 0x30
#define SUB 0x31
#define DIVIDE 0x32
#define MUL 0x33
#define JUMP 0x40
#define JNEG 0x41
#define JZ 0x42
#define HALT 0x43
#define JNP 0x59
#define JNS 0x55
#define NOT 0x51

int instructionCounter;
int accumulator;
int coord;
int Cu();
int Alu(int command, int operand);
#endif