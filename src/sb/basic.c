#include "basic.h"

struct stack_t *stack_push(struct stack_t *head, char a)
{
    struct stack_t *ptr;

    if ((ptr = malloc(sizeof(struct stack_t))) == NULL)
    {
        perror("Out of memory\n");
        exit(1);
    }
    ptr->c = a;
    ptr->next = head;
    return ptr;
}

char stack_pop(struct stack_t **head)
{
    struct stack_t *ptr;
    char a;

    if (*head == NULL)
        return '\0';
    ptr = *head;
    a = ptr->c;
    *head = ptr->next;
    free(ptr);
    return a;
}

int priority(char c)
{
    switch (c)
    {
    case '*':
        return 3;
    case '/':
        return 3;
    case '-':
        return 2;
    case '+':
        return 2;
    case '(':
        return 1;
    default:
        break;
    }
    return 0;
}

void translate_to_rpn(char *outstr, char *a)
{
    struct stack_t *opers = NULL;
    int k, point;
    k = 0;
    point = 0;
    while (a[k] != '\0' && a[k] != '\n')
    {
        if (a[k] == ')')
        {
            while ((opers->c) != '(')
                outstr[point++] = stack_pop(&opers);
            stack_pop(&opers);
        }
        if ((a[k] >= 'A' && a[k] <= 'Z') || (a[k] >= '0' && a[k] <= '9'))
            outstr[point++] = a[k];
        else if (a[k] == '(')
            opers = stack_push(opers, '(');
        else if (a[k] == '+' || a[k] == '-' || a[k] == '/' || a[k] == '*')
        {
            if (opers == NULL)
                opers = stack_push(opers, a[k]);
            else if (priority(opers->c) < priority(a[k]))
                opers = stack_push(opers, a[k]);
            else
            {
                while ((opers != NULL) && (priority(opers->c) >= priority(a[k])))
                    outstr[point++] = stack_pop(&opers);
                opers = stack_push(opers, a[k]);
            }
        }
        k++;
    }
    while (opers != NULL)
        outstr[point++] = stack_pop(&opers);
    outstr[point] = '\0';
}

int command_type(const char *cmd)
{
    if (!strcmp(cmd, "REM"))
        return 1;
    if (!strcmp(cmd, "INPUT"))
        return 2;
    if (!strcmp(cmd, "PRINT"))
        return 3;
    if (!strcmp(cmd, "GOTO"))
        return 4;
    if (!strcmp(cmd, "IF"))
        return 5;
    if (!strcmp(cmd, "LET"))
        return 6;
    if (!strcmp(cmd, "END"))
        return 7;

    return 0;
}

unsigned variable_id(const struct Variables *vars, unsigned max, char variables) 
{
    for (unsigned id = 0; id < max; ++id)
        if (vars[id].name == variables)
            return id;
    return max;
}

#define getVarID(toID, varName)                   \
    if (isdigit(varName))                         \
    {                                             \
        toID = var_id;                            \
        variables[toID].name = tmp_var;                 \
        variables[toID].address = 99 - toID;            \
        variables[toID].init_value = atoi(&varName);    \
        ++var_id;                                 \
        ++tmp_var;                                \
    }                                             \
    else                                          \
    {                                             \
        toID = variable_id(variables, var_id, varName); \
        if (toID == var_id)                       \
        {                                         \
            variables[toID].name = varName;             \
            variables[toID].address = 99 - toID;        \
            variables[toID].init_value = 0;             \
            ++var_id;                             \
        }                                         \
    }

int basic_to_asm(const char *filename_bas, const char *filename_asm)
{
    FILE *fbas = fopen(filename_bas, "r");
    if (!fbas)
        return 1;

    char *asm_code = (char *)malloc(100 * 16);
    asm_code[0] = 0;
    struct Lines *lines = (struct Lines *)malloc(sizeof(*lines) * 100); 
    struct Variables *variables = (struct Variables *)malloc(sizeof(*variables) * 52); 
    struct Goto_goto *forgoto = (struct Goto_goto *)malloc(sizeof(*forgoto) * 100); 
    char *buffer = (char *)malloc(128); 
    unsigned address = 0; 
    unsigned goto_id = 0;
    unsigned var_id = 0;  
    unsigned line = 0;   
    unsigned line_id = 0; 
    char fail = 0, end = 0, tmp_var = 'a';


    while (end == 0 && fscanf(fbas, "%u %[A-Z] ", &line, buffer) != 0) 
    {
        if (line_id != 0 && line <= lines[line_id].line_number)
        { 
            end = fail = 1;
            break;
        }
        int type = command_type(buffer); 
        lines[line_id].line_number = line; 
        lines[line_id].asm_line_address = address;
        ++line_id; 
    type_check:
        switch (type)
        {
        case 0: 
            fail = 1;
            break;
        case 1: 
            continue;
        case 2: 
        {
            fscanf(fbas, "%[A-Z]", buffer); 
            if (buffer[1] != 0 || !(buffer[0] >= 'A' && buffer[0] <= 'Z'))
            {
                end = fail = 1;
                break;
            }
            unsigned id = variable_id(variables, var_id, buffer[0]);
            if (id == var_id) 
            {
                variables[id].name = buffer[0];
                variables[id].address = 99 - id; 
                variables[id].init_value = 0;
                ++var_id;
            }
            sprintf(buffer, "%02u READ %02u\n", address, variables[id].address);
            strcat(asm_code, buffer);
        }
        break;
        case 3:  
        {
            fscanf(fbas, "%[A-Z]", buffer);
            if (buffer[1] != 0 || !(buffer[0] >= 'A' && buffer[0] <= 'Z'))
            {
                end = fail = 1;
                break;
            }
            unsigned id = variable_id(variables, var_id, buffer[0]);
            if (id == var_id)
            {
                variables[id].name = buffer[0];
                variables[id].address = 99 - id;
                variables[id].init_value = 0;
                ++var_id;
            }
            sprintf(buffer, "%02u WRITE %02u\n", address, variables[id].address);
            strcat(asm_code, buffer);
        }
        break;
        case 4: 
        {
            sprintf(buffer, "%02u JUMP ", address); 
            strcat(asm_code, buffer); 
            fscanf(fbas, "%u", &line); 
            if (line > lines[line_id - 1].line_number) 
            {
                forgoto[goto_id].goto_basicline = line;
                forgoto[goto_id].address_pos = strlen(asm_code); 
                strcat(asm_code, "00\n");
                ++goto_id;

                ++address;
                continue;
            }
            int id = line_id - 1;
            while (line != lines[id].line_number) {
                --id;          
            if (id < 0)
            {
                end = fail = 1;
                break;
            }
        }
            sprintf(buffer, "%02u\n", lines[id].asm_line_address);   
            strcat(asm_code, buffer);
        }
        break;
        case 5:  
        {
            char op1[8], op2[8];
            fscanf(fbas, "%[0-9A-Z] %1[<=>] %[0-9A-Z]", op1, buffer, op2);
            if (isdigit(op1[0]) && isdigit(op2[0]))
            {
                int result = 0;
                switch (buffer[0])
                {
                case '<':
                    result = atoi(op1) < atoi(op2);
                    break;
                case '>':
                    result = atoi(op1) > atoi(op2);
                    break;
                default:
                    result = atoi(op1) == atoi(op2);
                    break;
                }
                if (result == 0)
                    continue;
                fscanf(fbas, "%[A-Z]", buffer);
                type = command_type(buffer);
                goto type_check;
            }

            int id1 = var_id, id2 = var_id; 
            if (isdigit(op1[0]))  
            {
                variables[id1].name = tmp_var;  
                variables[id1].address = 99 - var_id;
                variables[id1].init_value = atoi(op1);
                ++var_id;
                ++tmp_var;  

                id2 = variable_id(variables, var_id, op2[0]); 
                if (id2 == var_id)
                {
                    variables[id2].name = op2[0];
                    variables[id2].address = 99 - var_id;
                    variables[id2].init_value = 0;
                    ++var_id;
                }
            }
            else if (isdigit(op2[0]))
            {
                variables[id2].name = tmp_var;
                variables[id2].address = 99 - var_id;
                variables[id2].init_value = atoi(op2);
                ++var_id;
                ++tmp_var;

                id1 = variable_id(variables, var_id, op1[0]);
                if (id1 == var_id)
                {
                    variables[id1].name = op1[0];
                    variables[id1].address = 99 - var_id;
                    variables[id1].init_value = 0;
                    ++var_id;
                }
            }
            else 
            {
                id1 = variable_id(variables, var_id, op1[0]);
                if (id1 == var_id)
                {
                    variables[id1].name = op1[0];
                    variables[id1].address = 99 - var_id;
                    variables[id1].init_value = 0;
                    ++var_id;
                }

                id2 = variable_id(variables, var_id, op2[0]);
                if (id2 == var_id)
                {
                    variables[id2].name = op2[0];
                    variables[id2].address = 99 - var_id;
                    variables[id2].init_value = 0;
                    ++var_id;
                }
            }

            switch (buffer[0])
            {
            case '<':
                sprintf(buffer, "%02u LOAD %02u\n%02u SUB %02u\n%02u JNEG 00\n",
                        address, variables[id2].address,
                        address + 1, variables[id1].address,
                        address + 2);
                address += 3;
                forgoto[goto_id].goto_basicline = line + 1;
                forgoto[goto_id].address_pos = strlen(asm_code) + 29;
                ++goto_id;
                break;
            case '>':
                sprintf(buffer, "%02u LOAD %02u\n%02u SUB %02u\n%02u JNEG 00\n",
                        address, variables[id1].address,
                        address + 1, variables[id2].address,
                        address + 2);
                address += 3;
                forgoto[goto_id].goto_basicline = line + 1;
                forgoto[goto_id].address_pos = strlen(asm_code) + 29;
                ++goto_id;
                break;
            default:
                sprintf(buffer, "%02u LOAD %02u\n%02u SUB %02u\n%02u JZ %02u\n%02u JUMP 00\n",
                        address, variables[id2].address,
                        address + 1, variables[id1].address,
                        address + 2, address + 4,
                        address + 3);
                address += 4;
                forgoto[goto_id].goto_basicline = line + 1;
                forgoto[goto_id].address_pos = strlen(asm_code) + 38;
                ++goto_id;
                break;
            }
            strcat(asm_code, buffer);
            fscanf(fbas, " %[A-Z] ", buffer);
            type = command_type(buffer);
            goto type_check;
        }
        break;
        case 6: 
        {
            fscanf(fbas, "%[A-Z]", buffer);
            if (buffer[1] != 0 || !(buffer[0] >= 'A' && buffer[0] <= 'Z')) 
            {
                end = fail = 1;
                break;
            }
            char toVar = buffer[0]; 
            char exp[256] = "\0";
            char rpn[256];
            fgets(exp, 255, fbas);      // = B * A
            translate_to_rpn(rpn, exp); // B A *
            if (strlen(rpn) > 1)
            {
                char stack[100] = "\0";
                int pos = 0;
                for (size_t i = 0; rpn[i]; i++)
                {
                    if (pos > 1 && isalnum(rpn[i]) == 0)
                    {

                        unsigned id;
                        char var1 = stack[pos - 2]; 
                        getVarID(id, var1); 
                        sprintf(buffer, "%02u LOAD %02u\n", address, variables[id].address); 
                        strcat(asm_code, buffer);
                        ++address;

                        char var2 = stack[pos - 1]; 
                        getVarID(id, var2);
                        if (rpn[i] == '+')
                            sprintf(buffer, "%02u ADD %02u\n", address, variables[id].address);
                        else if (rpn[i] == '-')
                            sprintf(buffer, "%02u SUB %02u\n", address, variables[id].address);
                        else if (rpn[i] == '*')
                            sprintf(buffer, "%02u MUL %02u\n", address, variables[id].address);
                        else if (rpn[i] == '/')
                            sprintf(buffer, "%02u DIV %02u\n", address, variables[id].address);
                        strcat(asm_code, buffer);
                        ++address;

                        id = var_id;   
                        variables[id].name = tmp_var;
                        variables[id].address = 99 - id;
                        variables[id].init_value = 0;
                        ++tmp_var;
                        ++var_id;

                        sprintf(buffer, "%02u STORE %02u\n", address, variables[id].address); 
                        strcat(asm_code, buffer);
                        ++address;
                        pos--;  
                        stack[pos - 1] = tmp_var - 1;   
                    }
                    else       
                    {
                        stack[pos] = rpn[i];
                        pos++;
                    }
                }
                sprintf(buffer, "%02u LOAD %02u\n", address, variables[var_id - 1].address); 
                strcat(asm_code, buffer);
                ++address;
                int id;
                getVarID(id, toVar);
                sprintf(buffer, "%02u STORE %02u\n", address, variables[id].address); 
                strcat(asm_code, buffer);
            }
            else
            {
                int id = var_id;
                if (isdigit(rpn[0]))
                {
                    id = var_id;
                    ++var_id;
                    if (variable_id(variables, var_id, toVar) == var_id)
                    {
                        variables[id].name = toVar;
                        variables[id].address = 99 - id;
                        variables[id].init_value = atoi(rpn);
                        --address;
                        break;
                    }
                    else
                    {
                        variables[id].name = tmp_var;
                        variables[id].address = 99 - id;
                        variables[id].init_value = atoi(rpn);
                        ++tmp_var;
                    }
                }
                else
                {
                    id = variable_id(variables, var_id, rpn[0]);
                    if (id == var_id)
                    {
                        variables[id].name = rpn[0];
                        variables[id].address = 99 - id;
                        variables[id].init_value = 0;
                        ++var_id;
                    }
                }

                sprintf(buffer, "%02u LOAD %02u\n", address, variables[id].address);
                strcat(asm_code, buffer);
                ++address;
                id = variable_id(variables, var_id, toVar);
                if (id == var_id)
                {
                    variables[id].name = toVar;
                    variables[id].address = 99 - id;
                    variables[id].init_value = 0;
                    ++var_id;
                }
                sprintf(buffer, "%02u STORE %02u\n", address, variables[id].address);
                strcat(asm_code, buffer);
            }
        }
        break;
        case 7:
            sprintf(buffer, "%02u HALT 00", address);
            strcat(asm_code, buffer);
            end = 1;
            break;
        }
        ++address;
        if (type == 6)
            continue;

        int ignore;
        do
        {
            ignore = fgetc(fbas);
        } while (ignore != '\n' && ignore != EOF);
        if (ignore == EOF)
            break;
    }

    if (address + var_id > 99)
        fail = 1;

    if (fail == 0)
    {
        int id = 0;
        for (; id < goto_id; ++id)
        {
            int lid = 0;
            while (lid < line_id &&
                   lines[lid].line_number < forgoto[id].goto_basicline)
                ++lid;
            if (lid != line_id)
            {
                sprintf(asm_code + forgoto[id].address_pos, "%02u", lines[lid].asm_line_address);
                asm_code[forgoto[id].address_pos + 2] = '\n';
            }
        }

        for (id = var_id - 1; id >= 0; --id)
        {
            sprintf(buffer, "\n%02u = %x", variables[id].address, variables[id].init_value);
            strcat(asm_code, buffer);
        }

        FILE *fasm = fopen(filename_asm, "w");
        if (fasm != NULL)
        {
            fputs(asm_code, fasm);
            fclose(fasm);
        }
        else
            fail = 1;
    }

    free(buffer);
    free(forgoto);
    free(variables);
    free(lines);
    free(asm_code);
    return fail == 1;
}