#include <tsnc/token-stream.h>
#include <tsnc/parser.h>
#include <tsnc/ast.h>

#define tsnc_subparse_op(expr) if (expr == 0) return 0;

static int tsnc_parse_namesp_import(struct tsnc_ast_node **dest,
    struct tsnc_source *source) {
  struct tsnc_token token;
  size_t startpos = 0;

  tsnc_token_stream_next(&token, &source->tokens);
  if (token.kind != TSNC_TOKEN_KIND_ASTERISK) {
    tsnc_source_report_error(source, token.startpos, token.endpos,
        "Expected \"*\", got \"%s\"", token.str);
    return 0;
  }

  startpos = token.startpos;

  tsnc_token_stream_next(&token, &source->tokens);
  if (token.kind != TSNC_TOKEN_KIND_KEYWORD && strcmp(token.str, "as") != 0) {
    tsnc_source_report_error(source, token.startpos, token.endpos,
        "Expected \"as\" keyword, got \"%s\"", token.str);
    return 0;
  }

  tsnc_token_stream_next(&token, &source->tokens);
  if (token.kind != TSNC_TOKEN_KIND_IDENTIFIER) {
    tsnc_source_report_error(source, token.startpos, token.endpos,
        "Expected identifier, got \"%s\"", token.str);
    return 0;
  }

  tsnc_ast_ident_create(dest, token.str, startpos, token.endpos);
  return 1;
}

static int tsnc_parse_import_clause(struct tsnc_ast_node **dest,
    struct tsnc_source *source) {
  struct tsnc_token token;

  tsnc_token_stream_peek(&token, &source->tokens);
  switch (token.kind) {
    case TSNC_TOKEN_KIND_ASTERISK:
      tsnc_subparse_op(tsnc_parse_namesp_import(dest, source));
      return 1;
    case TSNC_TOKEN_KIND_IDENTIFIER:
      tsnc_token_stream_next(NULL, &source->tokens);
      tsnc_ast_ident_create(dest, token.str, token.startpos, token.endpos);
      return 1;
    default:
      return 0;
  }
}

static int tsnc_parse_import_modspec(struct tsnc_ast_node **dest,
    struct tsnc_source *source) {
  struct tsnc_token token;

  if (dest == NULL || source == NULL)
    return 0;

  tsnc_token_stream_next(&token, &source->tokens);
  if (token.kind != TSNC_TOKEN_KIND_STRING) {
    tsnc_source_report_error(source, token.startpos, token.endpos,
        "Invalid syntax. Expected string literal, got \"%s\"", token.str);
    return 0;
  }

  tsnc_ast_strlit_create(dest, token.str, token.startpos, token.endpos);
  return 1;
}

static int tsnc_parse_import_from_clause(struct tsnc_ast_node **dest,
    struct tsnc_source *source) {
  struct tsnc_token token;

  tsnc_token_stream_next(&token, &source->tokens);
  if (token.kind != TSNC_TOKEN_KIND_KEYWORD && strcmp(token.str, "from") != 0) {
    tsnc_source_report_error(source, token.startpos, token.endpos,
        "Expected \"from\" keyword, got \"%s\"", token.str);
    return 0;
  }

  tsnc_subparse_op(tsnc_parse_import_modspec(dest, source));
  return 1;
}

static int tsnc_parse_import_decl(struct tsnc_ast_node **dest,
    struct tsnc_source *source) {
  struct tsnc_token token;
  struct tsnc_ast_node *modspec, *clause;
  size_t declstartpos;

  tsnc_token_stream_next(&token, &source->tokens);
  if (token.kind != TSNC_TOKEN_KIND_KEYWORD && strcmp(token.str, "import") != 0) {
    tsnc_source_report_error(source, token.startpos, token.endpos,
        "Expected \"import\" keyword, got \"%s\"", token.str);
    return 0;
  }

  declstartpos = token.startpos;

  tsnc_token_stream_peek(&token, &source->tokens);
  switch (token.kind) {
    case TSNC_TOKEN_KIND_STRING:
      tsnc_subparse_op(tsnc_parse_import_modspec(&modspec, source));

      tsnc_ast_import_decl_create(dest, NULL, modspec,
          declstartpos, token.endpos);
      return 1;
    default:
      tsnc_subparse_op(tsnc_parse_import_clause(&clause, source));
      tsnc_subparse_op(tsnc_parse_import_from_clause(&modspec, source));

      tsnc_ast_import_decl_create(dest, clause, modspec,
          declstartpos, modspec->endpos);
      return 1;
  }

  return 0;
}

static int tsnc_parse_stmt(struct tsnc_ast_node **dest,
    struct tsnc_source *source) {
  struct tsnc_token token;

  if (dest == NULL || source == NULL)
    return 0;

  tsnc_token_stream_peek(&token, &source->tokens);

  switch (token.kind) {
    case TSNC_TOKEN_KIND_EOF:
      return 0;
    case TSNC_TOKEN_KIND_KEYWORD:
      if (strcmp(token.str, "import") == 0)
        tsnc_subparse_op(tsnc_parse_import_decl(dest, source));

      tsnc_token_stream_next(&token, &source->tokens);
      if (token.kind != TSNC_TOKEN_KIND_SEMICOLON
          && token.kind != TSNC_TOKEN_KIND_EOL
          && token.kind != TSNC_TOKEN_KIND_EOF) {
        tsnc_source_report_error(source, token.startpos, token.endpos,
            "Expected \";\", got \"%s\"", token.str);
        return 0;
      }

      return 1;
    default:
      return 0;
  }

  return 1;
}

void tsnc_parse_source(struct tsnc_source *source) {
  struct tsnc_ast_node *stmt;

  while (tsnc_parse_stmt(&stmt, source)) {
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
