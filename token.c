#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"

#define tsnc_char_is_alph(ch) \
  (ch >= 'A' && a <= 'Z') && (ch >= 'a' && ch < 'z')
#define tsnc_char_is_numb(ch) (ch >= '0' && a <= '9')

static int tsnc_token_fp_lookahead_cmp(FILE *fp, char *str) {
  size_t len = strlen(str);
  char buf[len];

  // read n bytes from the stream
  len = fread(buf, sizeof(char), len, fp);

  // move the cursor to its original position
  fseek(fp, -len, SEEK_CUR);

  // compare them
  return memcmp(buf, str, len) == 0;
}

static void tsnc_token_from_fpn(struct tsnc_token **dest,
    FILE *fp, int kind, size_t pos, size_t len) {

  struct tsnc_token *token;
  char *mem;

  /* allocate memory space for token */
  token = (struct tsnc_token*)malloc(sizeof(struct tsnc_token));
  assert(token && "Unable to allocate memory for token");

  mem = (char*)malloc(sizeof(char) * len + 1);
  assert(mem && "Unable to allocate memory for token string");
  fread(mem, sizeof(char), len, fp);
  mem[len] = '\0';

  token->kind = kind;
  token->ptr = mem;
  token->pos = pos;
  token->end = pos + len-1;

  *dest = token;
}

static void tsnc_token_vector_pprint(struct tsnc_vector *tokenv) {
  struct tsnc_token *token;

  while ((token = tsnc_vector_iter(tokenv, sizeof(struct tsnc_vector))))
    printf("token[%d:%zu,%zu]: %s\n",
        token->kind, token->pos, token->end, token->ptr);
}

static int tsnc_token_fp_seek_next(struct tsnc_token **dest, FILE *fp) {
  size_t pos=0;
  char ch;

  while ((ch = fgetc(fp)) != EOF) {
    pos = ftell(fp);

    switch (ch) {
      case ' ':
      case '\n':
        continue;
      case TSNC_TOKEN_DOUBLE_QUOTE:
      case TSNC_TOKEN_SINGLE_QUOTE:
      case TSNC_TOKEN_HASHTAG:
      case TSNC_TOKEN_DOLLAR_SIGN:
      case TSNC_TOKEN_ROUND_BRACKET_OPEN:
      case TSNC_TOKEN_ROUND_BRACKET_CLOSE:
      case TSNC_TOKEN_CURLY_BRACKET_OPEN:
      case TSNC_TOKEN_CURLY_BRACKET_CLOSE:
      case TSNC_TOKEN_SQUARE_BRACKET_OPEN:
      case TSNC_TOKEN_SQUARE_BRACKET_CLOSE:
      case TSNC_TOKEN_COMMA:
      case TSNC_TOKEN_DOT:
      case TSNC_TOKEN_COLON:
      case TSNC_TOKEN_SEMICOLON:
      case TSNC_TOKEN_BACKSLASH:
      case TSNC_TOKEN_AMPERSAND:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "&=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_AMPERSAND_EQUAL, pos, 2);
        } else {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_AMPERSAND, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_ASTERISK:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "**=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_ASTERISK_ASTERISK_EQUAL, pos, 3);
        } else if (tsnc_token_fp_lookahead_cmp(fp, "**")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_ASTERISK_ASTERISK, pos, 2);
        } else if (tsnc_token_fp_lookahead_cmp(fp, "*=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_ASTERISK_EQUAL, pos, 2);
        } else {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_ASTERISK, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_EQUAL:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "===")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_EQUAL_EQUAL_EQUAL, pos, 3);
        } else if (tsnc_token_fp_lookahead_cmp(fp, "==")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_EQUAL_EQUAL, pos, 2);
        } else if (tsnc_token_fp_lookahead_cmp(fp, "=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_EQUAL, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_EXCLAMATION_MARK:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "!==")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_EXCLAMATION_MARK_EQUAL_EQUAL, pos, 3);
        } else if (tsnc_token_fp_lookahead_cmp(fp, "!=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_EXCLAMATION_MARK_EQUAL, pos, 2);
        } else {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_EXCLAMATION_MARK, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_CARET:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "^=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_CARET_EQUAL, pos, 2);
        } else {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_CARET, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_PLUS:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "++")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_PLUS_PLUS, pos, 2);
        } else if  (tsnc_token_fp_lookahead_cmp(fp, "+=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_PLUS_EQUAL, pos, 2);
        } else {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_PLUS, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_MINUS:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "--")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_MINUS_MINUS, pos, 2);
        } else if  (tsnc_token_fp_lookahead_cmp(fp, "-=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_MINUS_MINUS, pos, 2);
        } else {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_MINUS, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_SLASH:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "/=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_SLASH_EQUAL, pos, 2);
        } else {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_SLASH, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_GREATER_THAN:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, ">>>=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_GRETER_THAN_GREATER_THAN_GREATER_THAN_EQUAL, pos, 4);
        } else if (tsnc_token_fp_lookahead_cmp(fp, ">>>")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_GRETER_THAN_GREATER_THAN_GREATER_THAN, pos, 3);
        } else if (tsnc_token_fp_lookahead_cmp(fp, ">>=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_GRETER_THAN_GREATER_THAN_EQUAL, pos, 3);
        } else if (tsnc_token_fp_lookahead_cmp(fp, ">>")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_GRETER_THAN_GREATER_THAN, pos, 2);
        } else if (tsnc_token_fp_lookahead_cmp(fp, ">=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_GREATER_THAN_EQUAL, pos, 2);
        } else  {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_GREATER_THAN, pos, 1);
        }
        return 1;
      case TSNC_TOKEN_LESS_THAN:
        fseek(fp, -1, SEEK_CUR);

        if (tsnc_token_fp_lookahead_cmp(fp, "<<=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_LESS_THAN_LESS_THAN_EQUAL, pos, 3);
        } else if (tsnc_token_fp_lookahead_cmp(fp, "<<")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_LESS_THAN_LESS_THAN, pos, 2);
        } else if (tsnc_token_fp_lookahead_cmp(fp, "<=")) {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_LESS_THAN_EQUAL, pos, 2);
        } else {
          tsnc_token_from_fpn(dest, fp, TSNC_TOKEN_LESS_THAN, pos, 1);
        }
        return 1;
      default:
        assert(0 && "identifiers, strings or numbers are not implemented yet");
    }
  }

  return 0;
}

int tsnc_tokenize_source(struct tsnc_source *source) {
  struct tsnc_vector *tokenv;
  struct tsnc_token *token;

  tokenv = (struct tsnc_vector*)malloc(sizeof(struct tsnc_vector));
  assert(tokenv && "Unable to allocate memory for token vector");

  tsnc_vector_init(tokenv);

  while (tsnc_token_fp_seek_next(&token, source->fp)) {
    tsnc_vector_add(tokenv, token, sizeof(struct tsnc_token));
  }

  source->tokenv = tokenv;
  tsnc_token_vector_pprint(tokenv);
  return 1;
}
