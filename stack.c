#include "stack.h"
#include "cpu.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

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

void do_push(uint64_t v, int size)
{
    uint64_t rsp = get_reg_value("rsp");
    switch (size) {
    case 8:
        set_reg_value("rsp", rsp - 8);
        *(uint64_t *)(stack + rsp - 8) = v;
        break;
    case 4:
        set_reg_value("rsp", rsp - 4);
        *(uint32_t *)(stack + rsp - 4) = (uint32_t)v;
        break;
    case 2:
        set_reg_value("rsp", rsp - 2);
        *(uint16_t *)(stack + rsp - 2) = (uint16_t)v;
        break;    
    default:
        printf("invalid push width: %d\n", size);
        exit(-1);
    }
}

uint64_t do_pop(int size)
{
    uint64_t rsp = get_reg_value("rsp");
    switch (size) {
    case 8:
        set_reg_value("rsp", rsp + 8);
        return *(uint64_t *)(stack + rsp);
    case 4:
        set_reg_value("rsp", rsp + 4);
        return (uint64_t)(*(uint32_t *)(stack + rsp));
    case 2:
        set_reg_value("rsp", rsp + 2);
        return (uint64_t)(*(uint16_t *)(stack + rsp));
    default:
        printf("invalid pop width: %d\n", size);
        exit(-1);
    }
}

void stack_push(const char *reg)
{
    uint64_t v = get_reg_value(reg);
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

uint64_t stack_get(uint64_t addr, int size)
{
    switch(size) {
        case 8:
            return *(uint64_t *)(stack + addr);
        case 4:
            return (uint64_t)*(uint32_t *)(stack + addr);
        case 2:
            return (uint64_t)*(uint16_t *)(stack + addr);
        case 1:
            return (uint64_t)*(uint8_t *)(stack + addr);
        default:
            printf("invalid stack access width: %d\n", size);
            exit(-1);
    }
}

void stack_set(uint64_t addr, uint64_t v, int size)
{
    switch(size) {
        case 8:
            *(uint64_t *)(stack + addr) = v; break;
        case 4:
            *(uint32_t *)(stack + addr) = (uint32_t)v; break;
        case 2:
            *(uint16_t *)(stack + addr) = (uint16_t)v; break;
        case 1:
            *(uint8_t *)(stack + addr) = (uint8_t)v; break;
        default:
            printf("invalid stack access width: %d\n", size);
            exit(-1);
    }
}

int size(const char *pointer)
{
    switch (*pointer) {
        case 'Q': return 8;
        case 'D': return 4;
        case 'W': return 2;
        case 'B': return 1;
        default:
            printf("invalid pointer type: %s\n", pointer);
            exit(-1);
    }
}