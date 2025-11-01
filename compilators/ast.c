// ast.c
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* xdup(const char* s){ return s?strdup(s):NULL; }

AST* ast_new(NodeKind k, int n, ...) {
  AST *t = (AST*)calloc(1, sizeof(AST));
  t->kind = k; t->nchild = n;
  if (n>0) {
    t->child = (AST**)calloc(n, sizeof(AST*));
    va_list ap; va_start(ap, n);
    for (int i=0;i<n;i++) t->child[i] = va_arg(ap, AST*);
    va_end(ap);
  }
  return t;
}

AST* ast_id(const char *s){
  AST *t = ast_new(N_ID, 0);
  t->lexeme = xdup(s);
  return t;
}

AST* ast_num(int v){
  AST *t = ast_new(N_NUM, 0);
  t->ival = v;
  return t;
}

AST* ast_op(NodeKind k, const char *op, int n, ...) {
  AST *t = ast_new(k, n);
  t->lexeme = xdup(op);
  va_list ap; va_start(ap, n);
  t->child = (AST**)calloc(n, sizeof(AST*));
  for (int i=0;i<n;i++) t->child[i] = va_arg(ap, AST*);
  va_end(ap);
  return t;
}

static const char* kname(NodeKind k){
  switch(k){
    case N_PROGRAM: return "PROGRAM";
    case N_LIST: return "LIST";
    case N_DECL_VAR: return "DECL_VAR";
    case N_DECL_ARRAY: return "DECL_ARRAY";
    case N_DECL_FUNC: return "DECL_FUNC";
    case N_PARAMS: return "PARAMS";
    case N_PARAM: return "PARAM";
    case N_BLOCK: return "BLOCK";
    case N_STMT_LIST: return "STMT_LIST";
    case N_ASSIGN: return "ASSIGN";
    case N_IF: return "IF";
    case N_IFELSE: return "IF_ELSE";
    case N_WHILE: return "WHILE";
    case N_FOR: return "FOR";
    case N_RETURN: return "RETURN";
    case N_BINOP: return "BINOP";
    case N_UNOP: return "UNOP";
    case N_VAR: return "VAR";
    case N_INDEX: return "INDEX";
    case N_CALL: return "CALL";
    case N_NUM: return "NUM";
    case N_ID: return "ID";
    case N_EMPTY: return "EMPTY";
    default: return "?";
  }
}

void ast_print(AST *t, int indent){
  if(!t) return;
  for (int i=0;i<indent;i++) putchar(' ');
  printf("(%s", kname(t->kind));
  if (t->lexeme) printf(":%s", t->lexeme);
  if (t->kind==N_NUM) printf(":%d", t->ival);
  printf(")\n");
  for (int i=0;i<t->nchild;i++) ast_print(t->child[i], indent+2);
}

void ast_free(AST *t){
  if(!t) return;
  for (int i=0;i<t->nchild;i++) ast_free(t->child[i]);
  free(t->child);
  free(t->lexeme);
  free(t);
}
