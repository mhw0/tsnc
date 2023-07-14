#include <assert.h>
#include <string.h>
#include <tsnc/ast.h>

void tsnc_ast_node_create(struct tsnc_ast_node **dest,
    enum tsnc_ast_node_kind kind, size_t startpos, size_t endpos) {

  *dest = (struct tsnc_ast_node*)malloc(sizeof(struct tsnc_ast_node));
  assert(*dest && "Unable to allocate memory for AST struct");

  (*dest)->kind = kind;
  (*dest)->startpos = startpos;
  (*dest)->endpos = endpos;
  (*dest)->prev = NULL;
  (*dest)->next = NULL;
}

void tsnc_ast_import_decl_create(struct tsnc_ast_node **dest,
    struct tsnc_ast_node *clause, struct tsnc_ast_node *modspec,
    size_t startpos, size_t endpos) {

  tsnc_ast_node_create(dest, TSNC_AST_NODE_KIND_IMPORT_DECL,
      startpos, endpos);

  (*dest)->m.import_decl.clause = clause;
  (*dest)->m.import_decl.modspec = modspec;
}

void tsnc_ast_strlit_create(struct tsnc_ast_node **dest,
    const char *str, size_t startpos, size_t endpos) {
  char *strbuf;
  size_t len;

  tsnc_ast_node_create(dest, TSNC_AST_NODE_KIND_STRLIT,
      startpos, endpos);

  len = strlen(str);
  strbuf = (char*)malloc(len + 1);
  assert(strbuf && "Unable to allocate memory for string literal");
  memcpy(strbuf, str, len);
  strbuf[len] = '\0';

  (*dest)->m.strlit.str = strbuf;
}

void tsnc_ast_ident_create(struct tsnc_ast_node **dest,
    const char *name, size_t startpos, size_t endpos) {
  char *identbuf;
  size_t len;

  tsnc_ast_node_create(dest, TSNC_AST_NODE_KIND_IDENT,
      startpos, endpos);

  len = strlen(name);
  identbuf = (char*)malloc(len + 1);
  assert(identbuf && "Unable to allocate memory for identifier");
  memcpy(identbuf, name, len);
  identbuf[len] = '\0';

  (*dest)->m.ident.name = identbuf;
}

int tsnc_ast_node_equal(struct tsnc_ast_node *left,
    struct tsnc_ast_node *right) {

  if (left == NULL && right == NULL)
    return 1;

  if (left == NULL || right == NULL)
    return 0;

  if (left->kind != right->kind)
    return 0;

  if ((left->startpos != right->startpos)
      || (left->endpos != right->endpos))
    return 0;

  switch (left->kind) {
    case TSNC_AST_NODE_KIND_IMPORT_DECL:
      if (tsnc_ast_node_equal(left->m.import_decl.clause,
            right->m.import_decl.clause) == 0)
        return 0;

      return tsnc_ast_node_equal(left->m.import_decl.modspec,
            right->m.import_decl.modspec);
    case TSNC_AST_NODE_KIND_STRLIT:
      return strcmp(left->m.strlit.str, right->m.strlit.str) == 0;
    case TSNC_AST_NODE_KIND_IDENT:
      return strcmp(left->m.ident.name, right->m.ident.name) == 0;
    default:
      return 0;
  }

  return 1;
}
