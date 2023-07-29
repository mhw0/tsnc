#include <tsnc/token.h>
#include <tsnc/source.h>

struct tsnc_token *tsnc_token_create(enum tsnc_token_kind kind,
    const char *tokenstr, size_t startpos, size_t endpos) {
  struct tsnc_token *token;
  char *buf;
  size_t len;

  token = (struct tsnc_token*)malloc(sizeof(struct tsnc_token));
  if (token == NULL)
    return NULL;

  len = strlen(tokenstr);
  buf = malloc(len + 1);
  if (buf == NULL)
    return 0;

  memcpy(buf, tokenstr, len);
  buf[len] = '\0';

  token->kind = kind;
  token->str = buf;
  token->startpos = startpos;
  token->endpos = endpos;
  token->prev = NULL;
  token->next = NULL;
  return token;
}

int tsnc_token_equal(struct tsnc_token *left,
    struct tsnc_token *right) {
  if (left == NULL || right == NULL)
    return 0;

  if (left->kind != right->kind)
    return 0;

  if (left->startpos != right->startpos)
    return 0;

  if (left->endpos != right->endpos)
    return 0;

  if (strcmp(left->str, right->str) != 0)
    return 0;

  return 1;
}

void tsnc_token_free(struct tsnc_token *token) {
  if (token == NULL)
    return;

  free(token->str);
}

const char *tsnc_token_kind_tostr(enum tsnc_token_kind kind) {
  switch (kind) {
    case TSNC_TOKEN_KIND_KEYWORD_AS:
      return "as";
    case TSNC_TOKEN_KIND_KEYWORD_BREAK:
      return "break";
    case TSNC_TOKEN_KIND_KEYWORD_CASE:
      return "case";
    case TSNC_TOKEN_KIND_KEYWORD_CATCH:
      return "catch";
    case TSNC_TOKEN_KIND_KEYWORD_CLASS:
      return "class";
    case TSNC_TOKEN_KIND_KEYWORD_CONST:
      return "const";
    case TSNC_TOKEN_KIND_KEYWORD_CONTINUE:
      return "continue";
    case TSNC_TOKEN_KIND_KEYWORD_DEBUGGER:
      return "debugger";
    case TSNC_TOKEN_KIND_KEYWORD_DEFAULT:
      return "default";
    case TSNC_TOKEN_KIND_KEYWORD_DELETE:
      return "delete";
    case TSNC_TOKEN_KIND_KEYWORD_DO:
      return "do";
    case TSNC_TOKEN_KIND_KEYWORD_ELSE:
      return "else";
    case TSNC_TOKEN_KIND_KEYWORD_ENUM:
      return "enum";
    case TSNC_TOKEN_KIND_KEYWORD_EXPORT:
      return "export";
    case TSNC_TOKEN_KIND_KEYWORD_EXTENDS:
      return "export";
    case TSNC_TOKEN_KIND_KEYWORD_FALSE:
      return "false";
    case TSNC_TOKEN_KIND_KEYWORD_FINALLY:
      return "finally";
    case TSNC_TOKEN_KIND_KEYWORD_FOR:
      return "for";
    case TSNC_TOKEN_KIND_KEYWORD_FROM:
      return "from";
    case TSNC_TOKEN_KIND_KEYWORD_FUNCTION:
      return "function";
    case TSNC_TOKEN_KIND_KEYWORD_IF:
      return "if";
    case TSNC_TOKEN_KIND_KEYWORD_IMPLEMENTS:
      return "implements";
    case TSNC_TOKEN_KIND_KEYWORD_IMPORT:
      return "import";
    case TSNC_TOKEN_KIND_KEYWORD_IN:
      return "in";
    case TSNC_TOKEN_KIND_KEYWORD_INSTANCEOF:
      return "instanceof";
    case TSNC_TOKEN_KIND_KEYWORD_INTERFACE:
      return "interface";
    case TSNC_TOKEN_KIND_KEYWORD_LET:
      return "let";
    case TSNC_TOKEN_KIND_KEYWORD_NEW:
      return "new";
    case TSNC_TOKEN_KIND_KEYWORD_NULL:
      return "NULL";
    case TSNC_TOKEN_KIND_KEYWORD_PACKAGE:
      return "package";
    case TSNC_TOKEN_KIND_KEYWORD_PRIVATE:
      return "private";
    case TSNC_TOKEN_KIND_KEYWORD_PROTECTED:
      return "protected";
    case TSNC_TOKEN_KIND_KEYWORD_PUBLIC:
      return "public";
    case TSNC_TOKEN_KIND_KEYWORD_RETURN:
      return "return";
    case TSNC_TOKEN_KIND_KEYWORD_STATIC:
      return "static";
    case TSNC_TOKEN_KIND_KEYWORD_SUPER:
      return "super";
    case TSNC_TOKEN_KIND_KEYWORD_SWITCH:
      return "switch";
    case TSNC_TOKEN_KIND_KEYWORD_THIS:
      return "this";
    case TSNC_TOKEN_KIND_KEYWORD_THROW:
      return "throw";
    case TSNC_TOKEN_KIND_KEYWORD_TRUE:
      return "true";
    case TSNC_TOKEN_KIND_KEYWORD_TRY:
      return "try";
    case TSNC_TOKEN_KIND_KEYWORD_TYPEOF:
      return "typeof";
    case TSNC_TOKEN_KIND_KEYWORD_VAR:
      return "var";
    case TSNC_TOKEN_KIND_KEYWORD_VOID:
      return "void";
    case TSNC_TOKEN_KIND_KEYWORD_WHILE:
      return "while";
    case TSNC_TOKEN_KIND_KEYWORD_WITH:
      return "with";
    case TSNC_TOKEN_KIND_KEYWORD_YIELD:
      return "yield";
    default:
      /* TODO: add others */
      return "";
  }
}
