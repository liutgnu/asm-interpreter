%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
static int yyparse();
extern FILE *yyin;
static int yyerror(const char *s);
static double res;
%}
%union {
	double d;
}

%token <d> NUM;
%token ADD SUB MUL DIV
%token LBK RBK EOL
%token ABS

%type <d> line exp factor
%right ADD SUB
%right MUL DIV

%%
line: exp EOL {
	res = $1;
}

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
| LBK exp RBK {
	$$ = $2;
}
| ABS exp ABS {
	$$ = $2 > 0 ? $2 : -$2;
}
;
%%
static int yyerror(char const *str)
{
	extern char *yytext;
	fprintf(stderr, "parse error in %s\n", yytext);
	return 0;
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
