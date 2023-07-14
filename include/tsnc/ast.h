#ifndef TSNC_AST_H
#define TSNC_AST_H
#include <stdlib.h>

enum tsnc_ast_node_kind {
  TSNC_AST_NODE_KIND_IMPORT_DECL,
  TSNC_AST_NODE_KIND_IDENT,
  TSNC_AST_NODE_KIND_STRLIT
};

struct tsnc_ast_node_strlit {
  char *str;
};

struct tsnc_ast_node_ident {
  char *name;
};

struct tsnc_ast_node_import_decl {
  /* import clause */
  struct tsnc_ast_node *clause;
  /* import module specifier */
  struct tsnc_ast_node *modspec;
};

struct tsnc_ast_node {
  /* AST node kind */
  enum tsnc_ast_node_kind kind;

  union {
    struct tsnc_ast_node_import_decl import_decl;
    struct tsnc_ast_node_strlit strlit;
    struct tsnc_ast_node_ident ident;
  } m;

  /* points to the previous node */
  struct tsnc_ast_node *prev;
  /* points to the next node */
  struct tsnc_ast_node *next;
  /* start position of the node */
  size_t startpos;
  /* end position of the node */
  size_t endpos;
};

void tsnc_ast_import_decl_create(struct tsnc_ast_node **dest,
    struct tsnc_ast_node *clause, struct tsnc_ast_node *modspec,
    size_t startpos, size_t endpos);

void tsnc_ast_strlit_create(struct tsnc_ast_node **dest,
    const char *str, size_t startpos, size_t endpos);

void tsnc_ast_ident_create(struct tsnc_ast_node **dest,
    const char *name, size_t startpos, size_t endpos);

int tsnc_ast_node_equal(struct tsnc_ast_node *left,
    struct tsnc_ast_node *right);
#endif
