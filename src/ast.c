#include <assert.h>
#include <string.h>
#include <tsnc/ast.h>
#include <stdio.h>

static struct tsnc_ast_node *tsnc_ast_node_create(enum tsnc_ast_node_kind kind,
    size_t startpos, size_t endpos) {
  struct tsnc_ast_node *node;

  node = (struct tsnc_ast_node*)malloc(sizeof(struct tsnc_ast_node));
  assert(node && "Unable to allocate memory for AST node");

  node->kind = kind;
  node->startpos = startpos;
  node->endpos = endpos;
  node->prev = NULL;
  node->next = NULL;

  return node;
}

struct tsnc_ast_node *tsnc_ast_import_decl_create(struct tsnc_ast_node *clause,
    struct tsnc_ast_node *modspec, size_t startpos, size_t endpos) {
  struct tsnc_ast_node *node;

  node = tsnc_ast_node_create(TSNC_AST_NODE_KIND_IMPORT_DECL,
      startpos, endpos);

  node->m.import_decl.clause = clause;
  node->m.import_decl.modspec = modspec;
  return node;
}

struct tsnc_ast_node *tsnc_ast_strlit_create(const char *str,
    size_t startpos, size_t endpos) {
  struct tsnc_ast_node *node;
  char *strbuf;
  size_t len;

  node = tsnc_ast_node_create(TSNC_AST_NODE_KIND_STRLIT,
      startpos, endpos);

  len = strlen(str);
  strbuf = (char*)malloc(len + 1);
  assert(strbuf && "Unable to allocate memory for string literal");
  memcpy(strbuf, str, len);
  strbuf[len] = '\0';

  node->m.strlit.str = strbuf;

  return node;
}

struct tsnc_ast_node *tsnc_ast_ident_create(const char *name,
    size_t startpos, size_t endpos) {
  struct tsnc_ast_node *node;
  char *identbuf;
  size_t len;

  node = tsnc_ast_node_create(TSNC_AST_NODE_KIND_IDENT,
      startpos, endpos);

  len = strlen(name);
  identbuf = (char*)malloc(len + 1);
  assert(identbuf && "Unable to allocate memory for identifier");
  memcpy(identbuf, name, len);
  identbuf[len] = '\0';

  node->m.ident.name = identbuf;
  return node;
}

struct tsnc_ast_node *tsnc_ast_import_namesp_create(struct tsnc_ast_node *bind,
    size_t startpos, size_t endpos) {
  struct tsnc_ast_node *node;

  node = tsnc_ast_node_create(TSNC_AST_NODE_KIND_IMPORT_NAMESP,
      startpos, endpos);

  node->m.import_namesp.bind = bind;
  return node;
}

int tsnc_ast_node_equal(struct tsnc_ast_node *left, struct tsnc_ast_node *right) {
  struct tsnc_ast_node *node0, *node1;

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
      if (tsnc_ast_node_equal(TSNC_AST_NODE_IMPORT_DECL(left).clause,
            TSNC_AST_NODE_IMPORT_DECL(right).clause) == 0)
        return 0;

      return tsnc_ast_node_equal(TSNC_AST_NODE_IMPORT_DECL(left).modspec,
          TSNC_AST_NODE_IMPORT_DECL(right).modspec);
    case TSNC_AST_NODE_KIND_IMPORT_CLAUSE:
      if (tsnc_ast_node_equal(TSNC_AST_NODE_IMPORT_CLAUSE(left).named_imports,
            TSNC_AST_NODE_IMPORT_CLAUSE(right).named_imports) == 0)
        return 0;

      return tsnc_ast_node_equal(TSNC_AST_NODE_IMPORT_CLAUSE(left).default_import,
          TSNC_AST_NODE_IMPORT_CLAUSE(right).default_import);
    case TSNC_AST_NODE_KIND_NAMED_IMPORTS:
      node0 = TSNC_AST_NODE_NAMED_IMPORTS(left).named_import;
      node1 = TSNC_AST_NODE_NAMED_IMPORTS(right).named_import;

      while (1) {
        if (node0 == NULL && node1 == NULL)
          return 1;

        if (tsnc_ast_node_equal(node0, node1) == 0)
          return 0;

        node0 = node0->next;
        node1 = node1->next;
      }

      return 1;
    case TSNC_AST_NODE_KIND_NAMED_IMPORT:
      if (tsnc_ast_node_equal(TSNC_AST_NODE_NAMED_IMPORT(left).name,
            TSNC_AST_NODE_NAMED_IMPORT(right).name) == 0)
        return 0;

      return tsnc_ast_node_equal(TSNC_AST_NODE_NAMED_IMPORT(left).alias,
          TSNC_AST_NODE_NAMED_IMPORT(right).alias);
    case TSNC_AST_NODE_KIND_STRLIT:
      return strcmp(TSNC_AST_NODE_STRLIT(left).str,
          TSNC_AST_NODE_STRLIT(right).str) == 0;

    case TSNC_AST_NODE_KIND_IDENT:
      return strcmp(TSNC_AST_NODE_IDENT(left).name,
          TSNC_AST_NODE_IDENT(right).name) == 0;

    case TSNC_AST_NODE_KIND_IMPORT_NAMESP:
      return tsnc_ast_node_equal(TSNC_AST_NODE_IMPORT_NAMESP(left).bind,
          TSNC_AST_NODE_IMPORT_NAMESP(right).bind);
    default:
      return 0;
  }

  return 1;
}

struct tsnc_ast_node *tsnc_ast_import_clause_create(struct tsnc_ast_node *named_imports,
    struct tsnc_ast_node *default_import, size_t startpos, size_t endpos) {
  struct tsnc_ast_node *node;

  node = tsnc_ast_node_create(TSNC_AST_NODE_KIND_IMPORT_CLAUSE,
      startpos, endpos);

  TSNC_AST_NODE_IMPORT_CLAUSE(node).named_imports = named_imports;
  TSNC_AST_NODE_IMPORT_CLAUSE(node).default_import = default_import;

  return node;
}

struct tsnc_ast_node *tsnc_ast_named_imports_create(struct tsnc_ast_node *named_import,
    size_t startpos, size_t endpos) {
  struct tsnc_ast_node *node;

  node = tsnc_ast_node_create(TSNC_AST_NODE_KIND_NAMED_IMPORTS,
      startpos, endpos);

  TSNC_AST_NODE_NAMED_IMPORTS(node).named_import = named_import;
  return node;
}

struct tsnc_ast_node *tsnc_ast_named_import_create(struct tsnc_ast_node *name,
    struct tsnc_ast_node *alias, size_t startpos, size_t endpos) {
  struct tsnc_ast_node *node;

  node = tsnc_ast_node_create(TSNC_AST_NODE_KIND_NAMED_IMPORT,
      startpos, endpos);

  TSNC_AST_NODE_NAMED_IMPORT(node).name = name;
  TSNC_AST_NODE_NAMED_IMPORT(node).alias = alias;
  return node;
}

void tsnc_ast_node_free(struct tsnc_ast_node *node) {
  if (node == NULL)
    return;

  switch (node->kind) {
    case TSNC_AST_NODE_KIND_IMPORT_DECL:
      tsnc_ast_node_free(TSNC_AST_NODE_IMPORT_DECL(node).clause);
      tsnc_ast_node_free(TSNC_AST_NODE_IMPORT_DECL(node).modspec);
      break;
    case TSNC_AST_NODE_KIND_IMPORT_CLAUSE:
      tsnc_ast_node_free(TSNC_AST_NODE_IMPORT_CLAUSE(node).named_imports);
      tsnc_ast_node_free(TSNC_AST_NODE_IMPORT_CLAUSE(node).default_import);
      break;
    case TSNC_AST_NODE_KIND_NAMED_IMPORTS:
      tsnc_ast_node_free(TSNC_AST_NODE_NAMED_IMPORTS(node).named_import);
      break;
    case TSNC_AST_NODE_KIND_NAMED_IMPORT:
      tsnc_ast_node_free(TSNC_AST_NODE_NAMED_IMPORT(node).name);
      tsnc_ast_node_free(TSNC_AST_NODE_NAMED_IMPORT(node).alias);
      break;
    case TSNC_AST_NODE_KIND_STRLIT:
      free(TSNC_AST_NODE_STRLIT(node).str);
      break;
    case TSNC_AST_NODE_KIND_IDENT:
      free(TSNC_AST_NODE_IDENT(node).name);
      break;
    default:
      break;
  }

  tsnc_ast_node_free(node->prev);
  tsnc_ast_node_free(node->next);
}

#define TSNC_AST_NODE_PPRINT_BASIC(node, name, indent) \
  printf("%*s\x1B[32m%s\x1B[0m (Kind: \x1B[35m%d\x1B[0m, Pos: \x1B[33m%zu:%zu)\x1B[0m\n", \
      indent, "", name, node->kind, node->startpos, node->endpos);

void tsnc_ast_node_pprint(struct tsnc_ast_node *node, const char *base, int indent) {
  if (node == NULL)
    return;

  if (base != NULL) {
    TSNC_AST_NODE_PPRINT_BASIC(node, base, indent);
    indent += 2;
  }

  switch (node->kind) {
    case TSNC_AST_NODE_KIND_IMPORT_DECL:
      TSNC_AST_NODE_PPRINT_BASIC(node, "ImportDeclaration", indent);
      tsnc_ast_node_pprint(TSNC_AST_NODE_IMPORT_DECL(node).clause, NULL, indent + 2);
      tsnc_ast_node_pprint(TSNC_AST_NODE_IMPORT_DECL(node).modspec, "ModuleSpecifier", indent + 2);
      break;
    case TSNC_AST_NODE_KIND_IMPORT_CLAUSE:
      TSNC_AST_NODE_PPRINT_BASIC(node, "ImportClause", indent);
      tsnc_ast_node_pprint(TSNC_AST_NODE_IMPORT_CLAUSE(node).named_imports, "NamedImports", indent + 2);
      tsnc_ast_node_pprint(TSNC_AST_NODE_IMPORT_CLAUSE(node).default_import, "DefaultImport", indent + 2);
      break;
    case TSNC_AST_NODE_KIND_NAMED_IMPORTS:
      tsnc_ast_node_pprint(TSNC_AST_NODE_NAMED_IMPORTS(node).named_import, NULL, indent);
      break;
    case TSNC_AST_NODE_KIND_NAMED_IMPORT:
      TSNC_AST_NODE_PPRINT_BASIC(node, "NamedImport", indent);
      tsnc_ast_node_pprint(TSNC_AST_NODE_NAMED_IMPORT(node).name, "Name", indent + 2);
      tsnc_ast_node_pprint(TSNC_AST_NODE_NAMED_IMPORT(node).alias, "Alias", indent + 2);
      break;
    case TSNC_AST_NODE_KIND_STRLIT:
      TSNC_AST_NODE_PPRINT_BASIC(node, "StringLiteral", indent);
      printf("%*s\"%s\"\n", indent + 2, "", TSNC_AST_NODE_STRLIT(node).str);
      break;
    case TSNC_AST_NODE_KIND_IDENT:
      TSNC_AST_NODE_PPRINT_BASIC(node, "Identifier", indent);
      printf("%*s%s\n", indent + 2, "", TSNC_AST_NODE_IDENT(node).name);
      break;
    default:
      break;
  }

  tsnc_ast_node_pprint(node->next, NULL, indent);
}
