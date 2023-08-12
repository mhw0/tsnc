#include <tsnc/token-stream.h>
#include <tsnc/parser.h>
#include <tsnc/ast.h>

#define TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, token, expkind) \
  tsnc_source_report_error(source, token->startpos, token->endpos, \
      "Expected \"%s\", got \"%s\"", tsnc_token_kind_tostr(expkind), token->str)

#define TSNC_PARSER_REPORT_EXPECTED_IDENT_ERROR(source, token) \
  tsnc_source_report_error(source, token->startpos, token->endpos, \
      "Expected identifier, got \"%s\"", token->str)

#define TSNC_PARSER_REPORT_EXPECTED_STRING_ERROR(source, token) \
  tsnc_source_report_error(source, token->startpos, token->endpos, \
      "Expected string, got \"%s\"", token->str)

#define TSNC_PARSER_REPORT_EXPECTED_EXPR_ERROR(source, token) \
  tsnc_source_report_error(source, token->startpos, token->endpos, \
      "Expected expression, got \"%s\"", token->str)

#define TSNC_PARSER_REPORT_EXPECTED_MODSPEC_ERROR(source, token) \
  tsnc_source_report_error(source, token->startpos, token->endpos, \
      "Expected module specifier, got \"%s\"", token->str)

// returns the next token
struct tsnc_token *tsnc_token_next(struct tsnc_source *source, int skipeol) {
  if (source->currtoken->next == NULL)
    return NULL;

  source->currtoken = source->currtoken->next;

  if (skipeol && source->currtoken->kind == TSNC_TOKEN_KIND_EOL)
    return tsnc_token_next(source, skipeol);

  return source->currtoken;
}

// returns the current token
struct tsnc_token *tsnc_token_peek(struct tsnc_source *source, int skipeol) {
  if (source->currtoken == NULL)
    return NULL;

  if (skipeol && source->currtoken->kind == TSNC_TOKEN_KIND_EOL)
    return tsnc_token_next(source, skipeol);

  return source->currtoken;
}

// moves the token cursor to the end of the statement (to semicolon or EOF)
void tsnc_parser_consume_stmt(struct tsnc_source *source) {
  // if the current token is already "end of statement", do nothing
  if (source->currtoken->kind == TSNC_TOKEN_KIND_SEMICOLON
      || source->currtoken->kind == TSNC_TOKEN_KIND_EOF)
    return;

  while (source->currtoken->next) {
    if (source->currtoken->kind == TSNC_TOKEN_KIND_SEMICOLON
        || source->currtoken->kind == TSNC_TOKEN_KIND_EOF)
      break;
    source->currtoken = source->currtoken->next;
  }
}

/**
 * ModuleSpecifier:
 *   StringLiteral
 */
static struct tsnc_ast_node *tsnc_parse_import_modspec(struct tsnc_source *source) {
  struct tsnc_token *token;

  token = tsnc_token_peek(source, 1);
  if (token->kind != TSNC_TOKEN_KIND_STRING) {
    TSNC_PARSER_REPORT_EXPECTED_STRING_ERROR(source, token);
    return NULL;
  }

  tsnc_token_next(source, 1);
  return tsnc_ast_strlit_create(token->str, token->startpos, token->endpos);
}

/**
 * FromClause:
 *   from ModuleSpecifier
 */
struct tsnc_ast_node *tsnc_parse_import_from_clause(struct tsnc_source *source) {
  struct tsnc_token *token;

  token = tsnc_token_peek(source, 1);
  if (token->kind != TSNC_TOKEN_KIND_KEYWORD_FROM) {
    TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, token,
        TSNC_TOKEN_KIND_KEYWORD_FROM);
    return NULL;
  }

  tsnc_token_next(source, 1);
  return tsnc_parse_import_modspec(source);
}

/**
 * NameSpaceImport:
 *   * as ImportedBinding
 */
struct tsnc_ast_node *tsnc_parse_namesp_import(struct tsnc_source *source) {
  struct tsnc_ast_node *bind;
  struct tsnc_token *start_token, *token;

  start_token = tsnc_token_peek(source, 1);
  if (start_token->kind != TSNC_TOKEN_KIND_ASTERISK) {
    TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, start_token,
        TSNC_TOKEN_KIND_ASTERISK);
    return NULL;
  }

  token = tsnc_token_next(source, 1);
  if (token->kind != TSNC_TOKEN_KIND_KEYWORD_AS) {
    TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, token,
        TSNC_TOKEN_KIND_KEYWORD_AS);
    return NULL;
  }

  token = tsnc_token_next(source, 1);
  if (token->kind != TSNC_TOKEN_KIND_IDENTIFIER) {
    TSNC_PARSER_REPORT_EXPECTED_IDENT_ERROR(source, token);
    return NULL;
  }

  tsnc_token_next(source, 1);
  bind = tsnc_ast_ident_create(token->str, token->startpos, token->endpos);
  return tsnc_ast_import_namesp_create(bind, start_token->startpos, bind->endpos);
}

/**
 * ImportSpecifier:
 *   ImportedBinding
 *   IdentifierName as ImportedBinding
 */
struct tsnc_ast_node *tsnc_parse_named_import(struct tsnc_source *source) {
  struct tsnc_token *start_token, *token;
  struct tsnc_ast_node *bind, *alias;

  start_token = tsnc_token_peek(source, 1);

  if (start_token->kind == TSNC_TOKEN_KIND_IDENTIFIER
      || start_token->kind == TSNC_TOKEN_KIND_KEYWORD_DEFAULT) {

    token = tsnc_token_next(source, 1);
    if (token->kind == TSNC_TOKEN_KIND_KEYWORD_AS) {

      token = tsnc_token_next(source, 1);
      if (token->kind == TSNC_TOKEN_KIND_IDENTIFIER) {

        tsnc_token_next(source, 1);
        bind = tsnc_ast_ident_create(start_token->str, start_token->startpos, start_token->endpos);
        alias = tsnc_ast_ident_create(token->str, token->startpos, token->endpos);
        return tsnc_ast_named_import_create(bind, alias, bind->startpos, alias->endpos);
      }

      return NULL;
    }

    bind = tsnc_ast_ident_create(start_token->str, start_token->startpos, start_token->endpos);
    return tsnc_ast_named_import_create(bind, NULL, bind->startpos, bind->endpos);
  }

  return NULL;
}

/**
 * NamedImports:
 *   {  }
 *   { ImportsList }
 *   { ImportsList , }
 *
 * ImportsList:
 *   ImportSpecifier
 *   ImportsList , ImportSpecifier
 */
struct tsnc_ast_node *tsnc_parse_named_imports(struct tsnc_source *source) {
  struct tsnc_ast_node *named_import=NULL, *root_named_import=NULL,
     *leaf_named_import=NULL;
  struct tsnc_token *start_token, *token;

  start_token = tsnc_token_peek(source, 1);
  if (start_token->kind != TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN) {
    TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, start_token,
        TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN);
    return NULL;
  }

  tsnc_token_next(source, 1);

  while ((named_import = tsnc_parse_named_import(source))) {
    // if it's the first named import
    if (root_named_import == NULL) {
      root_named_import = named_import;
      leaf_named_import = named_import;
    } else {
      leaf_named_import->next = named_import;
      named_import->prev = leaf_named_import;
      leaf_named_import = named_import;
    }

    token = tsnc_token_peek(source, 1);
    if (token->kind == TSNC_TOKEN_KIND_CURLY_BRACKET_CLOSE)
      break;

    // named imports should always end with comma
    if (token->kind == TSNC_TOKEN_KIND_COMMA) {
      tsnc_token_next(source, 1);
      continue;
    }

    TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, token,
        TSNC_TOKEN_KIND_COMMA);
    return NULL;
  }

  token = tsnc_token_peek(source, 1);
  if (token->kind != TSNC_TOKEN_KIND_CURLY_BRACKET_CLOSE) {
    tsnc_ast_node_free(root_named_import);
    TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, token,
        TSNC_TOKEN_KIND_CURLY_BRACKET_CLOSE);
    return NULL;
  }

  tsnc_token_next(source, 1);

  // if there's no named imports, return empty list
  if (root_named_import == NULL)
    return tsnc_ast_named_imports_create(NULL,
        start_token->startpos, token->endpos);

  return tsnc_ast_named_imports_create(root_named_import,
      start_token->startpos, token->endpos);
}

/**
 * ImportedDefaultBinding:
 *   ImportedBinding
 */
struct tsnc_ast_node *tsnc_parse_default_import(struct tsnc_source *source) {
  struct tsnc_token *token;

  token = tsnc_token_peek(source, 1);
  if (token->kind == TSNC_TOKEN_KIND_IDENTIFIER) {
    tsnc_token_next(source, 1);
    return tsnc_ast_ident_create(token->str,
        token->startpos, token->endpos);
  }

  return NULL;
}

/**
 * ImportClause:
 *   ImportedDefaultBinding
 *   NameSpaceImport
 *   NamedImports
 *   ImportedDefaultBinding, NameSpaceImport
 *   ImportedDefaultBinding, NamedImports
 */
struct tsnc_ast_node *tsnc_parse_import_clause(struct tsnc_source *source) {
  struct tsnc_ast_node *named_imports=NULL, *default_import=NULL;
  struct tsnc_token *token;
  size_t clause_endpos = 0;

  // TODO: parse type modifier

  token = tsnc_token_peek(source, 1);

  switch (token->kind) {
    case TSNC_TOKEN_KIND_ASTERISK:
      if ((named_imports = tsnc_parse_namesp_import(source)) == NULL)
        return NULL;

      return tsnc_ast_import_clause_create(named_imports, NULL,
          named_imports->startpos, named_imports->endpos);
    case TSNC_TOKEN_KIND_IDENTIFIER:
      default_import = tsnc_ast_ident_create(token->str,
          token->startpos, token->endpos);

      token = tsnc_token_next(source, 1);
      if (token->kind == TSNC_TOKEN_KIND_COMMA) {
        token = tsnc_token_next(source, 1);
        if (token->kind == TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN) {
          if ((named_imports = tsnc_parse_named_imports(source)) == NULL) {
            TSNC_PARSER_REPORT_EXPECTED_EXPR_ERROR(source, token);
            tsnc_ast_node_free(default_import);
            return NULL;
          }
        }
      }

      clause_endpos = named_imports ? named_imports->endpos : default_import->endpos;
      return tsnc_ast_import_clause_create(named_imports, default_import,
          default_import->startpos, clause_endpos);
    case TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN:
      if ((named_imports = tsnc_parse_named_imports(source)) == NULL)
        return NULL;

      token = tsnc_token_peek(source, 1);
      if (token->kind == TSNC_TOKEN_KIND_COMMA) {
        token = tsnc_token_next(source, 1);
        if (token->kind != TSNC_TOKEN_KIND_IDENTIFIER) {
          TSNC_PARSER_REPORT_EXPECTED_EXPR_ERROR(source, token);
          tsnc_ast_node_free(named_imports);
          return NULL;
        }

        tsnc_token_next(source, 1);
        default_import = tsnc_ast_ident_create(token->str,
            token->startpos, token->endpos);
      }

      clause_endpos = default_import ? default_import->endpos : named_imports->endpos;
      return tsnc_ast_import_clause_create(named_imports, default_import,
          named_imports->startpos, clause_endpos);
    default:
      return NULL;
  }
}

/**
 * ImportDeclaration:
 *   import ImportClause FromClause;
 *   import ModuleSpecifier;
 */
static struct tsnc_ast_node *tsnc_parse_import_decl(struct tsnc_source *source) {
  struct tsnc_ast_node *clause, *modspec;
  struct tsnc_token *token, *start_token;

  start_token = tsnc_token_peek(source, 1);

  if (start_token->kind != TSNC_TOKEN_KIND_KEYWORD_IMPORT) {
    TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, start_token,
        TSNC_TOKEN_KIND_KEYWORD_IMPORT);
    return NULL;
  }

  token = tsnc_token_next(source, 1);

  switch (token->kind) {
    // TODO: keyword may also be used in import clause, so this does not match
    case TSNC_TOKEN_KIND_ASTERISK:
    case TSNC_TOKEN_KIND_IDENTIFIER:
    case TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN:
      /* parse import clause */
      if ((clause = tsnc_parse_import_clause(source)) == NULL)
        return NULL;

      /* parse from clause */
      if ((modspec = tsnc_parse_import_from_clause(source)) == NULL) {
        tsnc_ast_node_free(clause);
        return NULL;
      }

      return tsnc_ast_import_decl_create(clause, modspec,
          start_token->startpos, modspec->endpos);
    case TSNC_TOKEN_KIND_STRING:
      /* parse module specifier */
      if ((modspec = tsnc_parse_import_modspec(source)) == NULL)
        return NULL;

      return tsnc_ast_import_decl_create(NULL, modspec,
          start_token->startpos, modspec->endpos);
    default:
      TSNC_PARSER_REPORT_EXPECTED_MODSPEC_ERROR(source, token);
      return NULL;
  }
}

static struct tsnc_ast_node *tsnc_parse_stmt(struct tsnc_source *source) {
  struct tsnc_ast_node *stmt=NULL;
  struct tsnc_token *token;

  token = tsnc_token_peek(source, 1);

  if (token->kind == TSNC_TOKEN_KIND_EOF)
    return NULL;

  switch (token->kind) {
    case TSNC_TOKEN_KIND_KEYWORD_IMPORT:
      stmt = tsnc_parse_import_decl(source);
      break;
    case TSNC_TOKEN_KIND_EOL:
    case TSNC_TOKEN_KIND_SEMICOLON:
      tsnc_token_next(source, 1);
      return tsnc_parse_stmt(source);
    default:
      return NULL;
  }

  // if the statement cannot be parsed, just continue
  if (stmt == NULL) {
    tsnc_parser_consume_stmt(source);
    return tsnc_parse_stmt(source);
  }

  // each statement must finish with semicolon or EOF
  token = tsnc_token_peek(source, 1);
  if (token->kind != TSNC_TOKEN_KIND_SEMICOLON
      && token->kind != TSNC_TOKEN_KIND_EOF
      && token->kind != TSNC_TOKEN_KIND_EOL) {
    TSNC_PARSER_REPORT_EXPECTED_TOKEN_ERROR(source, token,
        TSNC_TOKEN_KIND_SEMICOLON);
    return NULL;
  }

  tsnc_token_next(source, 1);
  stmt->endpos++;
  return stmt;
}


void tsnc_parse_source(struct tsnc_source *source) {
  struct tsnc_ast_node *stmt;

  while ((stmt = tsnc_parse_stmt(source))) {
    if (source->node == NULL) {
      source->node = stmt;
      source->currnode = stmt;
      continue;
    }

    source->currnode->next = stmt;
    stmt->prev = source->currnode;
    source->currnode = stmt;
  }
}
