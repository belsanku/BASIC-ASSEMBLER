#ifndef MYCONSOLE_H
#define MYCONSOLE_H

#include "../mt/myTerm.h"
#include "../mbg/myBigChar.h"
#include "../msc/mySimpleComputer.h"
#include "../mk/myReadKey.h"
#include "../sias/asm.h"
#include "../sa/cu.h"
#include "../sb/basic.h"
#include <sys/time.h>
#include <unistd.h>
#define STD_X_MEM 1
#define STD_Y_MEM 1
#define STD_DX_MEM 12
#define STD_DY_MEM 62

#define STD_X_ACC 1
#define STD_Y_ACC 63

#define MEM 0
#define ACC 1
#define INSTR 2
#define OPER 3
#define KEYS 4
#define BC 5
#define IO 6
#define FLAGS 7

int coord;
int accumulator;
extern int instructionCounter;
int command;
int operand;

int pointer_mem;

int coord;
int access_inout;
int access_mem;
int access_oper;
int access_acc;
int access_flag;
int access_bc;
int access_count;
int access_menu;
int visual_access;
void kektest(int signo);
//Console part
void pa_keyRun();
void console();
void reset();
int setVisualNull();
char io_msg[1024];
//void log_console(const char *msg);
void read_console_value(int addr, int *value);
int write_console_value(int addr, int value);
int readFromConsole();
int writeFromConsole(int value);
void keySave();
void keyLoad();
int updateMemVisual();
int visualRegGet(int flag, int *value);
int visualRegSet(int flag, int value);

void setAcc();
int setInstrCounter();
void setPointer();

void messageBox(int x, int y, int dx, int dy);
void clrMessageBox(int x, int y);

//Visual part
void showAll();
void visualInOut();
void visualMemory();
void visualAccumulator();
int visualCounter();
void visualOperation();
void visualFlags();
void visualMenu();
void visualBigCharArea();

#endif