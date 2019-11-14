#ifndef _CPU_H_
#define _CPU_H_

#define CF (1 << 0)
#define PF (1 << 2)
#define AF (1 << 4)
#define ZF (1 << 6)
#define SF (1 << 7)
#define TF (1 << 8)
#define IF (1 << 9)
#define DF (1 << 10)
#define OF (1 << 11)

#define GET_FLAG(x) (r_eflags & x)
#define ST_FLAG(x) {r_eflags |= x;}
#define CL_FLAG(x) {r_eflags &= ~x;}

#define GET_SIGN(x) (x >> 31)

extern unsigned long r_eflags;
extern unsigned long r_eip;
extern unsigned long regs_table[52];

extern void cmp_set_flags(unsigned long, unsigned long);
#endif
