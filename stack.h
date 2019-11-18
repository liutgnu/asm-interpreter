#ifndef _STACK_H_
#define _STACK_H_
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
#endif
