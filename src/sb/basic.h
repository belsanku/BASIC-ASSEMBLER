#ifndef BASIC_H
#define BASIC_H
#pragma once
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../mt/myTerm.h"
#include <unistd.h>

struct stack_t 
{
    char c;
    struct stack_t *next;
};

struct Lines 
{
    unsigned line_number; 
    unsigned asm_line_address;  
};

struct Variables 
{
    unsigned address;   
    int init_value;  
    char name;			
};

struct Goto_goto
{
    unsigned address_pos;  
    unsigned goto_basicline;  
};


struct stack_t *stack_push(struct stack_t *head, char a);
char stack_pop(struct stack_t **head);
int priority(char c);
void translate_to_rpn(char *outstr, char *a);
int command_type(const char *cmd);
unsigned variable_id(const struct Variables *vars, unsigned max, char var);
int basic_to_asm(const char* filename_bas, const char* filename_asm);

#endif 
