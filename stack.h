#ifndef _STACK_H_
#define _STACK_H_
#include <stdint.h>
/*
*  --------- stack_size
*  xxxxxxxxx
*  --------- stack_base
*
*  --------- stack_start
*      |
*      v
*
*
*  --------- stack_top
*  xxxxxxxxx
*  --------- stack (malloc start)
*/
#define STACK_SIZE (1 << 20)
#define STACK_FORBIDDEN_SIZE (1 << 14)

#define STACK_START_OFFSET (STACK_SIZE - STACK_FORBIDDEN_SIZE * 2)
#define STACK_BASE_OFFSET (STACK_SIZE - STACK_FORBIDDEN_SIZE)
extern void stack_push(const char *);
extern void stack_pop(const char *);
extern void init_stack(void);
extern uint64_t stack_get(uint64_t, int);
extern void stack_set(uint64_t, uint64_t, int);
extern int size(const char *);
#endif
