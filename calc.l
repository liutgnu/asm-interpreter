%option noyywrap
%{
#include "calc.tab.h"
%}
%%
\"[^\f\v\n\0]*\" {yylval.str = strndup(yytext + 1, strchr(yytext + 1, '"') - yytext - 1); return STR;}
print {yylval.opcode = "print"; return OPCODE;}
mov {yylval.opcode = "mov"; return OPCODE;}
je {yylval.opcode = "je"; return OPCODE;}
jmp {yylval.opcode = "jmp"; return OPCODE;}
cmp {yylval.opcode = "cmp"; return OPCODE;}
halt {yylval.opcode = "halt"; return OPCODE;}
[^ \t\s]+: {yylval.word = strndup(yytext, strchr(yytext, ':') - yytext); return LABL;}
[0-9]*\.?[0-9]+ {yylval.d = atof(yytext); return NUM;}
"+" {return ADD;}
"-" {return SUB;}
"*" {return MUL;}
"/" {return DIV;}
"(" {return LBK;}
")" {return RBK;}
"|" {return ABS;}
"," {return COMA;}
[\0\n] {return EOL;}
[ \t] {}
[a-zA-Z] {yylval.reg = *yytext; return REG;}
. {printf("unsupported char %x\n", (char)*yytext);}
%%
