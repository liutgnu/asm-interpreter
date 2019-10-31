%option noyywrap
%{
#include "calc.tab.h"
%}
%%
"+" {return ADD;}
"-" {return SUB;}
"*" {return MUL;}
"/" {return DIV;}
"(" {return LBK;}
")" {return RBK;}
"\0" {return EOL;}
"|" {return ABS;}
[ \t] {}
[0-9]*\.?[0-9]+ {yylval.d = atof(yytext); return NUM;}
"mov|cmp|je|print|call|ret|halt" {return OPCODE;}
[a-zA-Z] {yylval.reg = *yytext; return REG;}
"," {return COMA;}
"\"" {return QUOT;}
. {printf("unsupported char %x\n", (char)*yytext);}
%%
