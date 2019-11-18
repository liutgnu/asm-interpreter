#include "stack.h"
#include "cpu.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void *stack = NULL;

void init_stack(void)
{
    stack = malloc(STACK_SIZE);
    if (!stack) {
        printf("init stack error!\n");
        exit(-1);
    }
    memset(stack, 0xfe, STACK_FORBIDDEN_SIZE);
    memset(stack + STACK_BASE_OFFSET, 0xfe, STACK_FORBIDDEN_SIZE);
    set_reg_value("rsp", STACK_START_OFFSET);
    set_reg_value("rbp", STACK_START_OFFSET);
}

void do_push(unsigned long long v, int size)
{
    unsigned long long rsp = get_reg_value("rsp");
    switch (size) {
    case 8:
        set_reg_value("rsp", rsp - 8);
        *(unsigned long long *)(stack + rsp - 8) = v;
        break;
    case 4:
        set_reg_value("rsp", rsp - 4);
        *(unsigned long *)(stack + rsp - 4) = (unsigned long)v;
        break;
    case 2:
        set_reg_value("rsp", rsp - 2);
        *(unsigned short *)(stack + rsp - 2) = (unsigned short)v;
        break;    
    default:
        printf("invalid push width: %d\n", size);
        exit(-1);
    }
}

unsigned long long do_pop(int size)
{
    unsigned long long rsp = get_reg_value("rsp");
    switch (size) {
    case 8:
        set_reg_value("rsp", rsp + 8);
        return *(unsigned long long *)(stack + rsp);
    case 4:
        set_reg_value("rsp", rsp + 4);
        return (unsigned long long)(*(unsigned long *)(stack + rsp));
    case 2:
        set_reg_value("rsp", rsp + 2);
        return (unsigned long long)(*(unsigned short *)(stack + rsp));
    default:
        printf("invalid pop width: %d\n", size);
        exit(-1);
    }
}

void stack_push(const char *reg)
{
    unsigned long long v = get_reg_value(reg);
    switch (*reg) {
        case 'r': do_push(v, 8); break;
        case 'e': do_push(v, 4); break;
        default: do_push(v, 2); break;
    }
}

void stack_pop(const char *reg)
{
    switch (*reg) {
        case 'r': set_reg_value(reg, do_pop(8)); break;
        case 'e': set_reg_value(reg, do_pop(4)); break;
        default: set_reg_value(reg, do_pop(2)); break;
    }
}
