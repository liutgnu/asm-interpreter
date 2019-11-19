#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t regs_table[9];
/*Don't change order!!!*/
//uint64_t r_rax;
//uint64_t r_rbx;
//uint64_t r_rcx;
//uint64_t r_rdx;
//uint64_t r_rsi;
//uint64_t r_rdi;
//uint64_t r_rsp;
//uint64_t r_rbp;
//uint64_t r_rip;
uint32_t r_eflags;

void init_regs(void)
{
	memset(regs_table, 0, sizeof(regs_table));
	r_eflags = 0;	
}

static int get_reg_index(const char *reg)
{
	int base = 0;
	int len = strlen(reg);
	switch(reg[len - 1]) {
		case 'x': base = 0; break;
		case 'i': base = 4; break;
		case 'p': base = 6; break;
		default: goto out;
	}
	if ((base == 0) && (reg[len - 2] <= 'd') && 
	   (reg[len - 2] >= 'a')) {
		return base + reg[len - 2] - 'a';
	}
	switch(reg[len - 2]) {
		case 's': return base;
		case 'd':
		case 'b': return base + 1;
		case 'i': return base + 2;
		default: goto out;
	}
out:
	printf("invalid reg %s\n", reg); 
	exit(-1);
}

void set_reg_value(const char *reg, uint64_t v)
{
	int index = get_reg_index(reg);

	switch (reg[0]) {
		case 'r': SET_64(regs_table + index, v); break;
		case 'e': SET_32(regs_table + index, v); break;
		default : SET_16(regs_table + index, v); break;
	}
}

uint64_t get_reg_value(const char *reg)
{
	int index = get_reg_index(reg);

	switch (reg[0]) {
		case 'r': return GET_64(regs_table + index);
		case 'e': return GET_32(regs_table + index);
		default : return GET_16(regs_table + index);
	}
}

void cmp_set_flags(uint64_t l, uint64_t r)
{
	uint64_t tmp = ~r + 1;

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
