#include "mc/myConsole.h"
#include "mt/myTerm.h"
#include "mbg/myBigChar.h"
#include "msc/mySimpleComputer.h"
#include "mk/myReadKey.h"
#include "sias/asm.h"
#include "sa/cu.h"
#include "sb/basic.h"
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

void sghandler(int signo) {
    if (signo == SIGUSR1) {
        sc_regSet(IGNORTACT, 1);
        instructionCounter = 0;
        pointer_mem = instructionCounter;
    } else if (signo == SIGALRM) {
        int tRegisterValue = 0;
        sc_regGet(IGNORTACT, &tRegisterValue);
        if (!tRegisterValue) {
            Cu();
            instructionCounter++;
            pointer_mem = instructionCounter;
        }
        if (instructionCounter > 99) {
            raise(SIGUSR1);
        }
    }
}

int main() {

	struct itimerval nval, oval;
    nval.it_interval.tv_sec = 0;
    nval.it_interval.tv_usec = (100000);
    nval.it_value.tv_sec = 1;
    nval.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &nval, &oval);
    
    signal(SIGALRM, sghandler);
    signal(SIGUSR1, sghandler);
    sc_regInit();
    sc_memoryInit();
    console();

    return 0;
}
