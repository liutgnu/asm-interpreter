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
. {printf("unsupported char %x\n", (char)*yytext);}
%%
