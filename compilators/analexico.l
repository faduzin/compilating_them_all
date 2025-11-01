%{
#include "gramatica.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}
%option yylineno

%%

"if"      { return IF; }
"else"    { return ELSE; }
"while"   { return WHILE; }
"for"     { return FOR; }
"return"  { return RETURN; }
"int"     { return TIPO; }
"void"    { return VOID; }

[0-9]+    { yylval.ival = atoi(yytext); return NUM; }
[a-zA-Z][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return ID; }

"="       { return ATRIBUICAO; }
"+"       { return OP_SOMA; }
"-"       { return OP_SOMA; }
"*"       { return OP_MULT; }
"/"       { return OP_MULT; }
"=="|"!="|"<="|">="|"<"|">"  { return COMPARACAO; }

";"       { return PONTOVIRGULA; }
","       { return VIRGULA; }
"{"       { return ABRE_CHAVE; }
"}"       { return FECHA_CHAVE; }
"("       { return ABRE_PAREN; }
")"       { return FECHA_PAREN; }
"["       { return ABRE_COL; }
"]"       { return FECHA_COL; }

[ \t\r\n]+      { }
\/\*([^*]|\*+[^\/])*\*\/   { }

. { fprintf(stderr, "[Erro Léxico] Linha %d: caractere não reconhecido: '%s'\n", yylineno, yytext); }

%%

int yywrap(void) { return 1; }
