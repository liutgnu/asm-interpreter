%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
static int yyparse();
extern FILE *yyin;
static int yyerror(const char *s);
static int reg_name_to_index(const char);
static double res;
static double regs_table[52] = {0,};
%}
%union {
	double d;
	char reg;
	char *opcode;
}

%token <d> NUM;
%token ADD SUB MUL DIV
%token LBK RBK EOL
%token ABS
%token OPCODE
%token REG COMA QUOT 

%type <d> exp factor
%type <opcode> OPCODE
%right ADD SUB
%right MUL DIV

%%
instruction: OPCODE exp EOL {
	if (!strcmp($1, "print")) {
		printf("=%lf\n", $2);
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
		return name - 'A';
	printf("reg name error!\n");
	exit(-1);
}

static int yyerror(char const *str)
{
	extern char *yytext;
	fprintf(stderr, "parse error in %s\n", yytext);
	return 0;
}


/****************export func********************/
void init_regs(void)
{
	int i;
	for (i = 0; i < 52; i++){
		regs_table[i] = 3;
	}
}

double calc_num(const char *expression)
{
	yyin = fmemopen(expression, strlen(expression)+1, "r");
	if (yyparse()) {
		printf("parse error!\n");
		goto out;
	}
	fclose(yyin);
	return res;
out:
	fclose(yyin);
	exit(-1);
}
