#ifndef TSNC_AST_H
#define TSNC_AST_H
#include <stdlib.h>

/* getters */
#define TSNC_AST_NODE_IMPORT_DECL(node) node->m.import_decl
#define TSNC_AST_NODE_IMPORT_NAMESP(node) node->m.import_namesp
#define TSNC_AST_NODE_STRLIT(node) node->m.strlit
#define TSNC_AST_NODE_IDENT(node) node->m.ident
#define TSNC_AST_NODE_NAMED_IMPORT(node) node->m.named_import
#define TSNC_AST_NODE_NAMED_IMPORTS(node) node->m.named_imports
#define TSNC_AST_NODE_IMPORT_CLAUSE(node) node->m.import_clause

enum tsnc_ast_node_kind {
  TSNC_AST_NODE_KIND_IMPORT_DECL,

  TSNC_AST_NODE_KIND_NAMED_IMPORTS,
  TSNC_AST_NODE_KIND_NAMED_IMPORT,
  TSNC_AST_NODE_KIND_IMPORT_CLAUSE,

  TSNC_AST_NODE_KIND_IMPORT_NAMESP,

  TSNC_AST_NODE_KIND_IDENT,
  TSNC_AST_NODE_KIND_STRLIT
};

struct tsnc_ast_node_strlit {
  /* points to the content of the string literal */
  char *str;
};

struct tsnc_ast_node_ident {
  /* points to the name of the identifier */
  char *name;
};

struct tsnc_ast_node_named_import {
  /* importing name */
  struct tsnc_ast_node *name;
  /* local name */
  struct tsnc_ast_node *alias;
};

struct tsnc_ast_node_named_imports {
  /* points to the root named import */
  struct tsnc_ast_node *named_import;
};

struct tsnc_ast_node_import_clause {
  /* points to the named imports */
  struct tsnc_ast_node *named_imports;
  /* points to the default import */
  struct tsnc_ast_node *default_import;
};

struct tsnc_ast_node_namesp_import {
  /* holds imported identifier */
  struct tsnc_ast_node *bind;
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
    /* import declaration */
    struct tsnc_ast_node_import_decl import_decl;
    struct tsnc_ast_node_namesp_import import_namesp;
    struct tsnc_ast_node_import_clause import_clause;
    struct tsnc_ast_node_named_imports named_imports;
    struct tsnc_ast_node_named_import named_import;

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

struct tsnc_ast_node *tsnc_ast_import_namesp_create(struct tsnc_ast_node *bind,
    size_t startpos, size_t endpos);

struct tsnc_ast_node *tsnc_ast_import_decl_create(struct tsnc_ast_node *clause,
    struct tsnc_ast_node *modspec, size_t startpos, size_t endpos);

struct tsnc_ast_node *tsnc_ast_strlit_create(const char *str,
    size_t startpos, size_t endpos);

struct tsnc_ast_node *tsnc_ast_ident_create(const char *name,
    size_t startpos, size_t endpos);

int tsnc_ast_node_equal(struct tsnc_ast_node *left, struct tsnc_ast_node *right);
struct tsnc_ast_node *tsnc_ast_named_imports_create(struct tsnc_ast_node *named_import,
    size_t startpos, size_t endpos);

struct tsnc_ast_node *tsnc_ast_named_import_create(struct tsnc_ast_node *name,
    struct tsnc_ast_node *alias, size_t startpos, size_t endpos);

struct tsnc_ast_node *tsnc_ast_import_clause_create(struct tsnc_ast_node *named_imports,
    struct tsnc_ast_node *default_import, size_t startpos, size_t endpos);

void tsnc_ast_node_free(struct tsnc_ast_node *node);
void tsnc_ast_node_pprint(struct tsnc_ast_node *node, const char *base, int indent);
#endif
