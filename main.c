#include<stdio.h>
extern double calc_num(const char *);
extern void init_regs(void);
int main(void)
{
	init_regs();
	const char *exp[] = {"print 2+x"};
	int i;
	for (i = 0; i < 1; i++) {
		calc_num(exp[i]);
	}
	return 0;
}
