#ifndef SIGNAL
#define SIGNAL
#pragma once

#include <stdio.h>
#include "../mc/myConsole.h"
#include <signal.h>
#include <sys/time.h>
#include "../msc/mySimpleComputer.h"

void signalhangle(int signal);
void reset_signalhagle(int signal);
void create_timer(double interval);

#endif