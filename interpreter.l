%option noyywrap
%{
#include "interpreter.tab.h"
%}
%%
\"[^\f\v\n\0]*\" {
	int length = 0, i, j;
	yylval.str = strndup(yytext + 1, strchr(yytext + 1, '"') - yytext - 1);
	length = strlen(yylval.str);
	memset(yylval.str, 0, length);
	for (i = 0, j = 0; j < length;) {
		if (*(yytext + 1 + j) == '\\') {
			if (*(yytext + 1 + j + 1) == 'n') {
				yylval.str[i] = '\n';
				j += 2;
			}
			if (*(yytext + 1 + j + 1) == 't') {
				yylval.str[i] = '\t';
				j += 2;
			}
		} else {
			yylval.str[i] = *(yytext + 1 + j);
			j++;
		}
		i++;
	}
	return STR;
}
print {yylval.opcode = "print"; return OPCODE;}
mov {yylval.opcode = "mov"; return OPCODE;}
je {yylval.opcode = "je"; return OPCODE;}
jl {yylval.opcode = "jl"; return OPCODE;}
jle {yylval.opcode = "jle"; return OPCODE;}
jg {yylval.opcode = "jg"; return OPCODE;}
jge {yylval.opcode = "jge"; return OPCODE;}
jz {yylval.opcode = "jz"; return OPCODE;}
jmp {yylval.opcode = "jmp"; return OPCODE;}
cmp {yylval.opcode = "cmp"; return OPCODE;}
halt {yylval.opcode = "halt"; return OPCODE;}
call {yylval.opcode = "call"; return OPCODE;}
ret {yylval.opcode = "ret"; return OPCODE;}
push {yylval.opcode = "push"; return OPCODE;}
pop {yylval.opcode = "pop"; return OPCODE;}

[re]?ax {yylval.reg = strdup(yytext); return REG;}
[re]?bx {yylval.reg = strdup(yytext); return REG;}
[re]?cx {yylval.reg = strdup(yytext); return REG;}
[re]?dx {yylval.reg = strdup(yytext); return REG;}
[re]si {yylval.reg = strdup(yytext); return REG;}
[re]di {yylval.reg = strdup(yytext); return REG;}
[re]sp {yylval.reg = strdup(yytext); return REG;}
[re]bp {yylval.reg = strdup(yytext); return REG;}

 /* \s is not supported*/
[^ \t\r\v\f]+: {yylval.word = strndup(yytext, strchr(yytext, ':') - yytext); return LABL;}
[0-9]+ {yylval.d = (unsigned long long)atoll(yytext); return NUM;}
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
. {printf("unsupported char %x\n", (char)*yytext);}
%%

