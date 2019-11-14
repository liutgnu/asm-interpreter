%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "stack.h"
typedef enum {false, true} bool;

extern int yylex();
static int yyparse();
extern FILE *yyin;
static int yyerror(const char *s);
static int reg_name_to_index(const char);
extern int convert_label_to_line(const char *);
extern void free_resources(void);
%}
%union {
	unsigned long d;
	char reg;
	char *opcode;
	char *word;
	char *str;
}

%token <d> NUM;
%token ADD SUB MUL DIV
%token LBK RBK EOL
%token ABS
%token OPCODE
%token REG COMA QUOT LABL STR

%type <d> exp factor
%type <opcode> OPCODE
%type <reg> REG
%type <word> LABL
%type <str> STR
%right ADD SUB
%right MUL DIV

%%
perline: LABL instruction EOL {
	free($1);
}
| instruction EOL {}
| LABL EOL {
	free($1);
}
| EOL {}

instruction: OPCODE exp {
	if (!strcmp($1, "print")) {
		printf("%ld", $2);
	}	   
}
| OPCODE STR {
	if (!strcmp($1, "print")) {
		printf("%s", $2);
	}
	free($2);
}
| OPCODE REG COMA exp {
	if (!strcmp($1, "mov")) {
		regs_table[reg_name_to_index($2)] = $4;
	}
	if (!strcmp($1, "cmp")) {
		cmp_set_flags(regs_table[reg_name_to_index($2)], $4);
	}
}
| OPCODE LABL {
	int tmp = convert_label_to_line($2);
	if (tmp < 0) {
		printf("Wrong label %s\n", $2);
		free($2);
		free_resources();
		exit(-1);
	}
	if (!strcmp($1, "jmp")) {
		r_eip = tmp;
	}
	if (!strcmp($1, "je") || !strcmp($1, "jz")) {
		if (GET_FLAG(ZF)) {
			r_eip = tmp;
		}
	}
	if (!strcmp($1, "jl")) {
		if (GET_FLAG(SF) != GET_FLAG(OF)) {
			r_eip = tmp;
		}
	}
	if (!strcmp($1, "jle")) {
		if (GET_FLAG(ZF) || (GET_FLAG(SF) != GET_FLAG(OF))) {
			r_eip = tmp;
		}
	}
	if (!strcmp($1, "jg")) {
		if (!GET_FLAG(ZF) && (GET_FLAG(SF) == GET_FLAG(OF))) {
			r_eip = tmp;
		}
	}
	if (!strcmp($1, "jge")) {
		if (GET_FLAG(SF) == GET_FLAG(OF)) {
			r_eip = tmp;
		}
	}
	if (!strcmp($1, "call")) {
		push_address(r_eip);
		r_eip = tmp;
	}
	free($2);
}
| OPCODE {
	if (!strcmp($1, "halt")) {
		free_resources();
		exit(0);
	}
	if (!strcmp($1, "ret")) {
		r_eip = pop_address();
	}
}

/*calculation related*/

exp: factor {
	$$ = $1;
}
| exp ADD exp {
	$$ = $1 + $3;
}
| exp SUB exp {
	$$ = $1 - $3;
}
| exp MUL exp {
	$$ = $1 * $3;
}
| exp DIV exp {
	$$ = $1 / $3;
}

factor: NUM {
	$$ = $1;
}
| REG {
	$$ = regs_table[reg_name_to_index(yylval.reg)];
}
| LBK exp RBK {
	$$ = $2;
}
| ABS exp ABS {
	$$ = $2 > 0 ? $2 : -$2;
}
;
%%
static int reg_name_to_index(const char name)
{
	if (name >= 'a' && name <= 'z')
		return name - 'a';
	if (name >= 'A' && name <= 'Z')
		return name - 'A' + 26;
	printf("reg name error!\n");
	free_resources();
	exit(-1);
}

static int yyerror(char const *str)
{
	extern char *yytext;
	fprintf(stderr, "parse error in %x, %c\n", *yytext, *yytext);
	return 0;
}

/****************export func********************/
void init_regs(void)
{
	int i;
	for (i = 0; i < 52; i++){
		regs_table[i] = 0;
	}
	r_eflags = 0;
	r_eip = 0;
}

void do_execute(const char *instruction)
{
	if (!instruction) {
		goto out;
	}
	yyin = fmemopen(instruction, strlen(instruction) + 1, "r");
	if (yyparse()) {
		printf("parse error!\n");
		goto out;
	}
out:
	if (yyin) {
		fclose(yyin);
		yyin = NULL;
	}
}
