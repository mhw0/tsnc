#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "report.h"
#include "token.h"

static void tsnc_token_vector_pprint(struct tsnc_vector *tokenv) {
  struct tsnc_token *token;

  while ((token = tsnc_vector_iter(tokenv, sizeof(struct tsnc_vector)))) {
    if (token->ptr != NULL)
      printf("token[%d:%zu,%zu]: %s\n",
          token->kind, token->pos, token->end, token->ptr);
    else
      printf("token[%d:%zu,%zu]: %c\n",
          token->kind, token->pos, token->end, token->kind);
  }
}

static int tsnc_tokenize_source_hex(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *fp = source->fp;
  size_t pos=ftell(fp), endpos=0, chars=0;
  char ch, *tokenstr;

  // just to make sure that the input has correct prefix
  assert(fgetc(fp) == '0' && fgetc(fp) == 'x');

  while ((ch = fgetc(fp)) != EOF && ch != ' ' && ch != '\n') {
    if ((ch < 'a' || ch > 'f')
        && (ch < 'A' || ch > 'F')
        && (ch < '0' || ch > '9')) {

      tsnc_source_report_error(source, pos, ftell(fp) - 1,
          "Hexadecimal digit expected.");
      return 0;
    }
    chars++;
  }

  ungetc(ch, fp);
  endpos = ftell(fp);

  // check if the hexadecimal has digits.
  if (chars == 0) {
    tsnc_source_report_error(source, pos, endpos-1,
        "Hexadecimal digit expected.");
    return 0;
  }

  chars = endpos - pos;

  // move the cursor to the start of the hexadecimal value
  fseek(fp, pos, SEEK_SET);

  tokenstr = (char*)malloc(sizeof(char) * 2 + 1);
  assert(tokenstr && "Unable to allocate memory for token string");

  // copy the hexadecimal value
  fread(tokenstr, sizeof(char), chars, fp);
  tokenstr[chars] = '\0';

  dest->kind = TSNC_TOKEN_NUMBER;
  dest->pos = pos;
  dest->end = endpos - 1;
  dest->ptr = tokenstr;
  return 1;
}

static int tsnc_tokenize_source_string(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *fp = source->fp;
  size_t startpos = ftell(fp), endpos=startpos, chars=0;
  char ch, opench = fgetc(fp), *tokenstr;

  assert(opench == '\'' || opench == '"');

  while (((ch = fgetc(fp)) != EOF) && ch != '\n' && ch != opench)
    chars++;

  ungetc(ch, fp);
  endpos = ftell(fp);

  if (ch != opench) {
    tsnc_source_report_error(source, startpos, endpos - 1,
        "Unterminated string literal.");
    return 0;
  }

  fseek(fp, startpos + 1, SEEK_SET);

  tokenstr = (char*)malloc(sizeof(char) * chars + 1);
  assert(tokenstr && "Unable to allocate memory for string token string");
  fread(tokenstr, sizeof(char), chars, fp);
  tokenstr[chars] = '\0';

  dest->kind = TSNC_TOKEN_STRING;
  dest->pos = startpos;
  dest->end = endpos;
  dest->ptr = tokenstr;

  fseek(fp, endpos + 1, SEEK_SET);
  return 1;
}

static int tsnc_tokenize_source_number(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *fp = source->fp;
  size_t startpos = ftell(fp), endpos=0, numchars=0, chars=0;
  char ch, *tokenstr;

  while ((ch = fgetc(fp)) != EOF && ch != '\n' && ch != ' ') {
    if (ch >= '0' && ch <= '9') {
      numchars++;
    } else {
      chars++;
    }
  }

  ungetc(ch, fp);
  endpos = ftell(fp);

  if (chars != 0) {
    tsnc_source_report_error(source, startpos, endpos,
        "Invalid numeric literal.");
    return 0;
  }

  fseek(fp, startpos, SEEK_SET);

  tokenstr = (char*)malloc(sizeof(char) * chars + 1);
  assert(tokenstr && "Unable to allocate memory for number token string");
  fread(tokenstr, sizeof(char), chars, fp);
  tokenstr[chars] = '\0';

  dest->kind = TSNC_TOKEN_NUMBER;
  dest->pos = 0;
  dest->end = 0;
  dest->ptr = tokenstr;
  return 1;
}

static int tsnc_tokenize_source_keyword(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *fp = source->fp;
  size_t startpos=ftell(fp), endpos=0, chars=0;
  char ch=fgetc(fp), kwbuf[11];

  // first character should always start with a-z
  if (ch < 'a' && ch > 'z') {
    ungetc(ch, fp);
    return 0;
  }

  do
    chars++;
  while ((ch = fgetc(fp)) != EOF && ch != ' ' && ch != '\n'
      && (ch >= 'a' && ch <= 'z'));

  if (chars > 10) {
    fseek(fp, startpos, SEEK_SET);
    return 0;
  }

  // restore the start position
  fseek(fp, startpos, SEEK_SET);

  // read `chars` characters from the stream
  fread(kwbuf, sizeof(char), chars, fp);
  kwbuf[chars] = '\0';

  if (strcmp(kwbuf, "as") == 0)
    dest->kind = TSNC_TOKEN_KEYWORD_AS;
  else if (strcmp(kwbuf, "break") == 0)
    dest->kind = TSNC_TOKEN_KEYWORD_BREAK;
  else if (strcmp(kwbuf, "case") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_CASE;
  else if (strcmp(kwbuf, "catch") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_CATCH;
  else if (strcmp(kwbuf, "class") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_CLASS;
  else if (strcmp(kwbuf, "const") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_CONST;
  else if (strcmp(kwbuf, "continue") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_CONTINUE;
  else if (strcmp(kwbuf, "debugger") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_DEBUGGER;
  else if (strcmp(kwbuf, "default") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_DEFAULT;
  else if (strcmp(kwbuf, "delete") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_DELETE;
  else if (strcmp(kwbuf, "do") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_DO;
  else if (strcmp(kwbuf, "else") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_ELSE;
  else if (strcmp(kwbuf, "enum") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_ENUM;
  else if (strcmp(kwbuf, "export") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_EXPORT;
  else if (strcmp(kwbuf, "extends") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_EXTENDS;
  else if (strcmp(kwbuf, "for") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_FOR;
  else if (strcmp(kwbuf, "function") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_FUNCTION;
  else if (strcmp(kwbuf, "if") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_IF;
  else if (strcmp(kwbuf, "implements") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_IMPLEMENTS;
  else if (strcmp(kwbuf, "from") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_FROM;
  else if (strcmp(kwbuf, "import") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_IMPORT;
  else if (strcmp(kwbuf, "in") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_IN;
  else if (strcmp(kwbuf, "instanceof") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_INSTANCEOF;
  else if (strcmp(kwbuf, "interface") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_INTERFACE;
  else if (strcmp(kwbuf, "new") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_NEW;
  else if (strcmp(kwbuf, "package") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_PACKAGE;
  else if (strcmp(kwbuf, "private") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_PRIVATE;
  else if (strcmp(kwbuf, "protected") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_PROTECTED;
  else if (strcmp(kwbuf, "public") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_PUBLIC;
  else if (strcmp(kwbuf, "return") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_RETURN;
  else if (strcmp(kwbuf, "static") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_STATIC;
  else if (strcmp(kwbuf, "super") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_SUPER;
  else if (strcmp(kwbuf, "switch") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_SWITCH;
  else if (strcmp(kwbuf, "this") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_THIS;
  else if (strcmp(kwbuf, "throw") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_THROW;
  else if (strcmp(kwbuf, "try") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_TRY;
  else if (strcmp(kwbuf, "typeof") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_TYPEOF;
  else if (strcmp(kwbuf, "var") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_VAR;
  else if (strcmp(kwbuf, "void") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_VOID;
  else if (strcmp(kwbuf, "while") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_WHILE;
  else if (strcmp(kwbuf, "with") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_WITH;
  else if (strcmp(kwbuf, "yield") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_YIELD;
  else if (strcmp(kwbuf, "number") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_NUMBER;
  else if (strcmp(kwbuf, "string") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_STRING;
  else if (strcmp(kwbuf, "true") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_TRUE;
  else if (strcmp(kwbuf, "false") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_FALSE;
  else if (strcmp(kwbuf, "null") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_NULL;
  else if (strcmp(kwbuf, "undefined") == 0) 
    dest->kind = TSNC_TOKEN_KEYWORD_UNDEFINED;
  else {
    fseek(fp, startpos, SEEK_SET);
    return 0;
  }

  dest->pos = startpos;
  dest->end = ftell(fp) - 1;
  dest->ptr = NULL;
  return 1;
}

static int tsnc_tokenize_source_ident(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *fp = source->fp;
  size_t startpos=ftell(fp), chars=0;
  char ch=fgetc(fp), *tokenstr;

  if ((ch < 'A' && ch > 'Z') || (ch < 'a' && ch > 'z')) {
    ungetc(ch, fp);
    return 0;
  }

  do
    chars++;
  while ((ch = fgetc(fp)) != EOF && ch != ' ' && ch != '\n'
      && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '$' || ch == '_')));

  tokenstr = (char*)malloc(sizeof(char) * chars + 1);
  assert(tokenstr && "Unable to allocate memory for identifier token string");

  fseek(fp, startpos, SEEK_SET);
  fread(tokenstr, sizeof(char), chars, fp);
  tokenstr[chars] = '\0';

  dest->kind = TSNC_TOKEN_IDENTIFIER;
  dest->pos = startpos;
  dest->end = startpos + chars - 1;
  dest->ptr = tokenstr;
  return 1;
}

static int tsnc_token_seek_next(struct tsnc_token *dest,
    struct tsnc_source *source, FILE *fp) {
  size_t startpos;
  char ch;

  while ((ch = fgetc(fp)) != EOF) {
    startpos = ftell(fp);

    switch (ch) {
      case ' ':
      case '\n':
        continue;
      case TSNC_TOKEN_HASHTAG:
      case TSNC_TOKEN_ASPERAND:
      case TSNC_TOKEN_QUESTION_MARK:
      case TSNC_TOKEN_ROUND_BRACKET_OPEN:
      case TSNC_TOKEN_ROUND_BRACKET_CLOSE:
      case TSNC_TOKEN_CURLY_BRACKET_OPEN:
      case TSNC_TOKEN_CURLY_BRACKET_CLOSE:
      case TSNC_TOKEN_SQUARE_BRACKET_OPEN:
      case TSNC_TOKEN_SQUARE_BRACKET_CLOSE:
      case TSNC_TOKEN_VERTICAL_BAR:
      case TSNC_TOKEN_COMMA:
      case TSNC_TOKEN_DOT:
      case TSNC_TOKEN_COLON:
      case TSNC_TOKEN_SEMICOLON:
      case TSNC_TOKEN_BACKSLASH:
        dest->pos = startpos - 1;
        dest->ptr = NULL;
        dest->kind = ch;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_AMPERSAND:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // &=
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_AMPERSAND_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // &
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_AMPERSAND;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_ASTERISK:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // **=
        if (fgetc(fp) == '*' && fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_ASTERISK_ASTERISK_EQUAL;
          dest->end = dest->pos + 2;
          return 1;
        }

        // **
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '*') {
          dest->kind = TSNC_TOKEN_ASTERISK_ASTERISK;
          dest->end = dest->pos + 1;
          return 1;
        }

        // *=
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_ASTERISK_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // *
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_ASTERISK;
        dest->end = dest->pos + 1;
        return 1;
      case TSNC_TOKEN_EQUAL:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // ===
        if (fgetc(fp) == '=' && fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_EQUAL_EQUAL_EQUAL;
          dest->end = dest->pos + 2;
          return 1;
        }

        // ==
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_EQUAL_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // =
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_EQUAL;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_EXCLAMATION_MARK:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // !==
        if (fgetc(fp) == '=' && fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_EXCLAMATION_MARK_EQUAL;
          dest->end = dest->pos + 2;
          return 1;
        }

        // !=
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_EQUAL_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // !
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_EXCLAMATION_MARK;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_CARET:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // ^=
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_CARET_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // ^
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_CARET;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_PLUS:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // ++
        if (fgetc(fp) == '+') {
          dest->kind = TSNC_TOKEN_PLUS_PLUS;
          dest->end = dest->pos + 1;
          return 1;
        }

        // +=
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_PLUS_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // +
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_PLUS;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_MINUS:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // --
        if (fgetc(fp) == '-') {
          dest->kind = TSNC_TOKEN_MINUS_MINUS;
          dest->end = dest->pos + 1;
          return 1;
        }

        // -=
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_MINUS_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // -
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_MINUS;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_SLASH:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // /=
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_SLASH_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // /
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_SLASH;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_GREATER_THAN:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // >>>=
        if (fgetc(fp) == '>' && fgetc(fp) == '>' && fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_GREATER_THAN_GREATER_THAN_GREATER_THAN_EQUAL;
          dest->end = dest->pos + 3;
          return 1;
        }

        // >>>
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '>' && fgetc(fp) == '>') {
          dest->kind = TSNC_TOKEN_GREATER_THAN_GREATER_THAN_GREATER_THAN;
          dest->end = dest->pos + 2;
          return 1;
        }

        // >>=
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '>' && fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_GREATER_THAN_GREATER_THAN_EQUAL;
          dest->end = dest->pos + 2;
          return 1;
        }

        // >>
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '>') {
          dest->kind = TSNC_TOKEN_GREATER_THAN_GREATER_THAN;
          dest->end = dest->pos + 1;
          return 1;
        }

        // >=
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '>') {
          dest->kind = TSNC_TOKEN_GREATER_THAN_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // >
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_GREATER_THAN;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_LESS_THAN:
        dest->pos = startpos - 1;
        dest->ptr = NULL;

        // <<=
        if (fgetc(fp) == '<' && fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_LESS_THAN_LESS_THAN_EQUAL;
          dest->end = dest->pos + 2;
          return 1;
        }

        // <<
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '<') {
          dest->kind = TSNC_TOKEN_LESS_THAN_LESS_THAN;
          dest->end = dest->pos + 1;
          return 1;
        }

        // <=
        fseek(fp, startpos, SEEK_SET);
        if (fgetc(fp) == '=') {
          dest->kind = TSNC_TOKEN_LESS_THAN_EQUAL;
          dest->end = dest->pos + 1;
          return 1;
        }

        // <
        fseek(fp, startpos, SEEK_SET);
        dest->kind = TSNC_TOKEN_LESS_THAN;
        dest->end = dest->pos;
        return 1;
      case TSNC_TOKEN_DOUBLE_QUOTE:
      case TSNC_TOKEN_SINGLE_QUOTE:
        ungetc(ch, fp);

        if (tsnc_tokenize_source_string(dest, source))
          return 1;

        break;
      default:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_tokenize_source_keyword(dest, source) ||
            tsnc_tokenize_source_ident(dest, source)   ||
            tsnc_tokenize_source_hex(dest, source)     ||
            tsnc_tokenize_source_number(dest, source))
          return 1;

        return 0;
    }
  }

  return 0;
}

int tsnc_tokenize_source(struct tsnc_source *source) {
  struct tsnc_vector *tokenv, *reportv;
  struct tsnc_token token;

  tokenv = (struct tsnc_vector*)malloc(sizeof(struct tsnc_vector));
  assert(tokenv && "Unable to allocate memory for token vector");

  reportv = (struct tsnc_vector*)malloc(sizeof(struct tsnc_vector));
  assert(reportv && "Unable to allocate memory for report vector");

  tsnc_vector_init(tokenv);
  tsnc_vector_init(reportv);

  source->tokenv = tokenv;
  source->reportv = reportv;

  while (tsnc_token_seek_next(&token, source, source->fp)) {
    tsnc_vector_add(tokenv, &token, sizeof(struct tsnc_token));
  }

  tsnc_token_vector_pprint(tokenv);
  return 1;
}
