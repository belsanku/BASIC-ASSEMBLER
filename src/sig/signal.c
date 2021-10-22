#include "signal.h"

void signalhangle(int signal)
{
    int val;
    sc_regGet(IGNORTACT, &val);
    if (val == 0)
    {
        Cu();
        setVisualNull();
        mt_gotoXY(24, 1);
        printf("%s", io_msg);
    }
}

void reset_signalhagle(int signal)
{
    accumulator = 0;
    instructionCounter = 0;
    mt_setbgcolor(BLUE);
    coord = 0;
    mt_setbgcolor(BLUE);
    sc_regInit();
    sc_regSet(IGNORTACT, 1);
    sc_memoryInit();
    setVisualNull();
}

void create_timer(double interval)
{
    struct itimerval nval;
    nval.it_value.tv_sec = (long)interval;
    nval.it_interval.tv_sec = (long)interval;
    nval.it_value.tv_usec = (long)((interval - (long)interval) * 100000);
    nval.it_interval.tv_usec = (long)((interval - (long)interval) * 100000);
    setitimer(ITIMER_REAL, &nval, NULL);
}
