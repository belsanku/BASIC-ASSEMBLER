#include "cu.h"
struct termios new_term_state;
struct termios termState;
struct termios origin;

#define max(a,b) (((a)>(b))?(a):(b))

int Cu()
{
	int value = 0;
    int temp = 0;
    sc_memoryGet(instructionCounter, &value);
    int command;
    int operand;
    if (sc_commandDecode(value, &command, &operand)) //первое число команда, второе операнд
    {
        sc_regSet(ERRORCOMS, 0);
        return 1;
    }
    if (operand >= N || operand < 0)
    {
        sc_regSet(ERRORFILE, 1);
        sc_regSet(IGNORTACT, 1);
        return 1;
    }
    if (command >= 0x30 && command <= 0x33)
    {
        Alu(command, operand);
    }
    else
    {
        switch (command)
        {
        case READ:
            rk_myTermRegime(&new_term_state, 1, 0, 1, 0, 0);
            mt_gotoXY(36, 45);
			mt_setfgcolor(WHITE);
			mt_setbgcolor(BLACK);
            read_console_value(operand, &value);
            rk_myTermRegime(&new_term_state, 1, 0, 1, 1, 0);
            if (value > 0x3FFF)
            {
                sc_regSet(ERRORADD, 1);
                break;
            }
            else
            {
                sc_memorySet(operand, value);
                mt_gotoXY(24, 1);
                break;
            }
        case WRITE:
            temp = write_console_value(operand, value);
            mt_setfgcolor(WHITE);
			mt_setbgcolor(BLACK);
            printf("%d == %x", temp, temp);
            break;
        case LOAD:
            sc_memoryGet(operand, &accumulator);
            break;
        case STORE:
            sc_memorySet(operand, accumulator);
            break;
        case JUMP:
            instructionCounter = --operand;
            break;
        case JNEG:
            if (accumulator < 0)
            {
                instructionCounter = --operand;
            }
            break;
        case JNS:
            if (accumulator > 0)
            {
                instructionCounter = --operand;
            }
            break;
        case JZ:
            if (accumulator == 0)
            {
                instructionCounter = --operand;
            }
            break;
        case HALT:
            sc_regSet(IGNORTACT, 1);
            return 0;
            break;
        case JNP:
            if ((accumulator % 2) != 0)
            {
                instructionCounter = --operand;
            }
            break;
        }        
    }
    return 0;
}

int Alu(int command, int operand)
{
    int value = 0;
    sc_memoryGet(operand, &value);
    switch (command)
    {
    case ADD:
        if (accumulator + value > 0x3FFF)
        {
            sc_regSet(ERRORADD, 1);
            break;
        }
        accumulator += value;
        break;
    case SUB:
        if (accumulator - value < -0x3FFF)
        {
            sc_regSet(ERRORADD, 1);
            break;
        }
        accumulator -= value;
        break;
    case DIVIDE:
        if (value == 0)
        {
            sc_regSet(ERRORFLAG, 1);
            break;
        }
        accumulator /= value;
        break;
    case MUL:
        if (accumulator * value > 0x3FFF)
        {
            sc_regSet(ERRORADD, 1);
            break;
        }
        accumulator *= value;
        break;
    case AND:
    	if ((accumulator & value) > 0x3FFF)
        {
            sc_regSet(ERRORADD, 1);
            break;
        }
        accumulator &= value;
        break;  
    default:
        sc_regSet(ERRORCOMS, 1);
        break;
    }
    return 0;
}