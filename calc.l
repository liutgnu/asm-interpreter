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
[\0\n] {return EOL;}
"|" {return ABS;}
"," {return COMA;}
[ \t] {}
[0-9]*\.?[0-9]+ {yylval.d = atof(yytext); return NUM;}
"print" {yylval.opcode = "print"; return OPCODE;}
"mov" {yylval.opcode = "mov"; return OPCODE;}
[a-zA-Z] {yylval.reg = *yytext; return REG;}
. {printf("unsupported char %x\n", (char)*yytext);}
%%
