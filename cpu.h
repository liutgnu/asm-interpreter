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

#define GET_64(x) (*(x))
#define GET_32(x) ((*(x)) & 0xffffffff)
#define GET_16(x) ((*(x)) & 0xffff)
#define SET_64(x, v) {(*(x)) = (unsigned long long)v;}
#define SET_32(x, v) {(*(x)) = ((*(x)) & (~0xffffffff)) + (unsigned long)v;}
#define SET_16(x, v) {(*(x)) = ((*(x)) & (~0xffff)) + (unsigned short)v;}

extern unsigned long r_eflags;
extern unsigned long long regs_table[9];
extern void set_reg_value(const char *, unsigned long long);
extern unsigned long long get_reg_value(const char *);
extern void cmp_set_flags(unsigned long long, unsigned long long);
extern void init_regs(void);
#endif
