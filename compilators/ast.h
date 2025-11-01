// ast.h
#ifndef AST_H
#define AST_H
#include <stdarg.h>

typedef enum {
  N_PROGRAM, N_LIST, N_DECL_VAR, N_DECL_ARRAY, N_DECL_FUNC,
  N_PARAMS, N_PARAM, N_BLOCK, N_STMT_LIST, N_ASSIGN, N_IF, N_IFELSE,
  N_WHILE, N_FOR, N_RETURN, N_BINOP, N_UNOP, N_VAR, N_INDEX, N_CALL,
  N_NUM, N_ID, N_EMPTY
} NodeKind;

typedef struct AST {
  NodeKind kind;
  char *lexeme;     // para IDs/nomes/ops opcionais
  int ival;         // para números
  int nchild;
  struct AST **child;
} AST;

AST* ast_new(NodeKind k, int n, ...);          // nó com N filhos
AST* ast_id(const char *s);                     // folha ID
AST* ast_num(int v);                            // folha NUM
AST* ast_op(NodeKind k, const char *op, int n, ...); // nó operador
void ast_print(AST *t, int indent);
void ast_free(AST *t);

#endif
