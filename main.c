#include<stdio.h>
extern double calc_num(const char *);
extern void init_regs(void);
int main(void)
{
	init_regs();
	const char *exp[] = {"mov x,117.123","label:mov y,3","loop:","print \"hello world\"", "print x + y"};
	int i;
	for (i = 0; i < 5; i++) {
		calc_num(exp[i]);
	}
	return 0;
}
