#include<stdio.h>
extern double calc_num(const char *);
int main(void)
{
	const char *exp[] = {"2+3", "4*(3-7)-8"};
	int i;
	for (i = 0; i < 2; i++) {
		printf("%s", exp[i]);
		printf("=%lf\n", calc_num(exp[i]));
	}
	return 0;
}
