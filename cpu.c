#include "cpu.h"

unsigned long r_eflags;
unsigned long r_eip;
unsigned long regs_table[52];

void cmp_set_flags(unsigned long l, unsigned long r)
{
	unsigned long tmp = ~r + 1;

	//handle unsigned flags: ZF CF
	if (l == r) {
		ST_FLAG(ZF);
		CL_FLAG(CF);
	} else if (l > r) {
		CL_FLAG(ZF);
		CL_FLAG(CF);
	} else {
		CL_FLAG(ZF);
		ST_FLAG(CF);
	}

	//handle signed flags: SF OF
	if (GET_SIGN((l + tmp))) {
		ST_FLAG(SF);
	}else{
		CL_FLAG(SF);
	}

	CL_FLAG(OF);
	if ((GET_SIGN(l) == GET_SIGN(tmp)) && 
		(GET_SIGN(l) != GET_SIGN(l + tmp))) {
		ST_FLAG(OF);
	}
}
