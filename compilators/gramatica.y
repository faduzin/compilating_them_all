%define parse.error verbose

%code requires {
  #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
extern int yylineno;
AST *RAIZ = NULL;

int yylex(void);
int yyerror(const char *msg);
%}

/* ---- Tipos do yylval ---- */
%union {
  int ival;
  char* sval;
  AST* node;
}

/* ---- Tokens com valor ---- */
%token IF ELSE WHILE FOR RETURN
%token <sval> ID
%token <ival> NUM
%token TIPO VOID
%token OP_SOMA OP_MULT
%token ATRIBUICAO COMPARACAO
%token ABRE_CHAVE FECHA_CHAVE
%token ABRE_PAREN FECHA_PAREN
%token ABRE_COL FECHA_COL
%token PONTOVIRGULA VIRGULA

%left OP_SOMA
%left OP_MULT
%right UMINUS
%nonassoc IF_SEM_ELSE 
%nonassoc ELSE        

%type <node> programa lista_declaracoes declaracao declaracao_var declaracao_func
%type <node> parametros lista_param param composto_decl local_declaracoes lista_comandos
%type <node> comando comando_expressao comando_composto comando_selecao comando_iteracao comando_retorno
%type <node> expressao var simples_expressao soma_expressao termo fator
%type <node> lista_args lista_args_opt

%start programa

%%

programa
    : lista_declaracoes          { RAIZ = ast_new(N_PROGRAM, 1, $1); $$ = RAIZ; }
    ;

lista_declaracoes
    : lista_declaracoes declaracao  { $$ = ast_new(N_LIST, 2, $1, $2); }
    | declaracao                    { $$ = ast_new(N_LIST, 1, $1); }
    ;

declaracao
    : declaracao_var     { $$ = $1; }
    | declaracao_func    { $$ = $1; }
    ;

declaracao_var
    : TIPO ID PONTOVIRGULA                            { $$ = ast_new(N_DECL_VAR, 1, ast_id($2)); free($2); }
    | TIPO ID ABRE_COL NUM FECHA_COL PONTOVIRGULA     { $$ = ast_new(N_DECL_ARRAY, 2, ast_id($2), ast_num($4)); free($2); }
    ;

declaracao_func
    : TIPO ID ABRE_PAREN parametros FECHA_PAREN composto_decl
        { $$ = ast_new(N_DECL_FUNC, 3, ast_id($2), $4, $6); free($2); }
    | VOID ID ABRE_PAREN parametros FECHA_PAREN composto_decl
        { $$ = ast_new(N_DECL_FUNC, 3, ast_id($2), $4, $6); free($2); }
    ;

parametros
    : lista_param       { $$ = ast_new(N_PARAMS, 1, $1); }
    |                   { $$ = ast_new(N_PARAMS, 0); }
    | VOID              { $$ = ast_new(N_PARAMS, 1, ast_new(N_PARAM, 1, ast_id("void"))); }
    ;

lista_param
    : lista_param VIRGULA param   { $$ = ast_new(N_LIST, 2, $1, $3); }
    | param                       { $$ = ast_new(N_LIST, 1, $1); }
    ;

param
    : TIPO ID                     { $$ = ast_new(N_PARAM, 1, ast_id($2)); free($2); }
    | TIPO ID ABRE_COL FECHA_COL  { $$ = ast_new(N_PARAM, 2, ast_id($2), ast_new(N_EMPTY,0)); free($2); }
    ;

composto_decl
    : ABRE_CHAVE local_declaracoes lista_comandos FECHA_CHAVE
        { $$ = ast_new(N_BLOCK, 2, $2, $3); }
    ;

local_declaracoes
    : local_declaracoes declaracao_var  { $$ = ast_new(N_LIST, 2, $1, $2); }
    |                                   { $$ = ast_new(N_LIST, 0); }
    ;

lista_comandos
    : lista_comandos comando   { $$ = ast_new(N_STMT_LIST, 2, $1, $2); }
    |                          { $$ = ast_new(N_STMT_LIST, 0); }
    ;

comando
    : comando_expressao  { $$ = $1; }
    | comando_composto   { $$ = $1; }
    | comando_selecao    { $$ = $1; }
    | comando_iteracao   { $$ = $1; }
    | comando_retorno    { $$ = $1; }
    ;

comando_expressao
    : expressao PONTOVIRGULA { $$ = $1; }
    | PONTOVIRGULA           { $$ = ast_new(N_EMPTY,0); }
    ;

comando_composto
    : composto_decl  { $$ = $1; }
    ;

comando_selecao
    : IF ABRE_PAREN expressao FECHA_PAREN comando %prec IF_SEM_ELSE
        { $$ = ast_new(N_IF, 2, $3, $5); }
    | IF ABRE_PAREN expressao FECHA_PAREN comando ELSE comando
        { $$ = ast_new(N_IFELSE, 3, $3, $5, $7); }
    ;

comando_iteracao
    : WHILE ABRE_PAREN expressao FECHA_PAREN comando
        { $$ = ast_new(N_WHILE, 2, $3, $5); }
    | FOR ABRE_PAREN expressao PONTOVIRGULA expressao PONTOVIRGULA expressao FECHA_PAREN comando
        { $$ = ast_new(N_FOR, 4, $3, $5, $7, $9); }
    ;

comando_retorno
    : RETURN PONTOVIRGULA         { $$ = ast_new(N_RETURN, 0); }
    | RETURN expressao PONTOVIRGULA { $$ = ast_new(N_RETURN, 1, $2); }
    ;

expressao
    : var ATRIBUICAO expressao   { $$ = ast_op(N_ASSIGN, "=", 2, $1, $3); }
    | simples_expressao          { $$ = $1; }
    ;

var
    : ID                          { $$ = ast_new(N_VAR, 1, ast_id($1)); free($1); }
    | ID ABRE_COL expressao FECHA_COL { $$ = ast_new(N_INDEX, 2, ast_id($1), $3); free($1); }
    ;

simples_expressao
    : soma_expressao                           { $$ = $1; }
    | soma_expressao COMPARACAO soma_expressao { $$ = ast_op(N_BINOP, "cmp", 2, $1, $3); }
    ;

soma_expressao
    : soma_expressao OP_SOMA termo { $$ = ast_op(N_BINOP, "+-", 2, $1, $3); }
    | termo                        { $$ = $1; }
    ;

termo
    : termo OP_MULT fator { $$ = ast_op(N_BINOP, "*/", 2, $1, $3); }
    | fator               { $$ = $1; }
    ;

lista_args
    : lista_args VIRGULA expressao  { $$ = ast_new(N_LIST, 2, $1, $3); }
    | expressao                     { $$ = ast_new(N_LIST, 1, $1); }
    ;

lista_args_opt
    :                               { $$ = ast_new(N_LIST, 0); }
    | lista_args                    { $$ = $1; }
    ;

fator
    : ABRE_PAREN expressao FECHA_PAREN               { $$ = $2; }
    | var                                            { $$ = $1; }
    | NUM                                            { $$ = ast_num($1); }
    | ID ABRE_PAREN lista_args_opt FECHA_PAREN       { $$ = ast_new(N_CALL, 2, ast_id($1), $3); free($1); }
    | OP_SOMA fator %prec UMINUS                     { $$ = ast_op(N_UNOP, "un+", 1, $2); }
    ;

%%

int yyerror(const char *msg)
{
    fprintf(stderr, "\x1b[33m[Erro Sintático]\x1b[0m Linha %d: %s\n", yylineno, msg);
    return 0;
}

int main(void)
{
    printf("\x1b[1;37;44mParser C-\x1b[K\x1b[0m\n");
    printf("Analizando o código...\n");
    int status = yyparse();
    if (status == 0) {
        ast_print(RAIZ, 0);
        printf("\x1b[32mAnálise concluída com sucesso.\x1b[0m\n");
        ast_free(RAIZ);
    } else {
        printf("\x1b[31mAnálise encerrada com erros.\x1b[0m\n");
    }
    return status;
}
