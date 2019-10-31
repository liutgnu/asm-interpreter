#include<stdio.h>
extern double calc_num(const char *);
extern void init_regs(void);
int main(void)
{
	init_regs();
	const char *exp[] = {"2+x", "4*(x-7)-8"};
	int i;
	for (i = 0; i < 2; i++) {
		printf("%s", exp[i]);
		printf("=%lf\n", calc_num(exp[i]));
	}
	return 0;
}
