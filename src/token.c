#include <assert.h>
#include <tsnc/token.h>

int tsnc_token_create(struct tsnc_token *dest,
    enum tsnc_token_kind kind, char *token, size_t len,
    size_t startpos, size_t endpos) {
  char *buf;

  if (dest == NULL)
    return 0;

  buf = malloc(len + 1);
  if (buf == NULL)
    return 0;

  memcpy(buf, token, len);
  buf[len] = '\0';

  dest->kind = kind;
  dest->str = buf;
  dest->startpos = startpos;
  dest->endpos = endpos;
  
  return 1;
}

static int tsnc_tokenize_source_string(struct tsnc_token *dest,
    struct tsnc_source *source) {
  char startch, currch, *tokenstr;
  size_t charscnt = 0, startpos = 0, endpos = 0;
  FILE *srcfp = source->fp;

  startpos = ftell(srcfp);
  startch = fgetc(srcfp);

  while (((currch = fgetc(srcfp)) != EOF)
      && (currch != '\n') && (currch != startch))
    charscnt++;

  if (currch != startch) {
    tsnc_source_report_error(source, startpos, startpos + charscnt,
        "Invalid or unexpected token", NULL);
    return 0;
  }

  endpos = ftell(srcfp);
  fseek(srcfp, startpos + 1, SEEK_SET);

  tokenstr = (char*)malloc(charscnt + 1);
  assert(tokenstr && "Unable to allocate memory for token string");
  fread(tokenstr, sizeof(char), charscnt, srcfp);
  tokenstr[charscnt] = '\0';

  // restore the position
  fseek(srcfp, endpos, SEEK_SET);

  // TODO: tsnc_token_create could be used here
  dest->kind = TSNC_TOKEN_KIND_STRING;
  dest->str = tokenstr;
  dest->startpos = startpos;
  dest->endpos = startpos + charscnt + 1;
  return 1;
}

static int tsnc_tokenize_number_hex(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *srcfp = source->fp;
  size_t startpos=0, charscnt=0, hexcharscnt=0;
  char currch, *hexbuf;

  startpos = ftell(srcfp);

  if (fgetc(srcfp) != '0' || fgetc(srcfp) != 'x') {
    fseek(srcfp, -2, SEEK_CUR);
    return 0;
  }

  charscnt += 2;

  while ((currch = fgetc(srcfp)) && currch != ' ' && currch != EOF) {
    if ((currch >= 'a' && currch <= 'f')
        || (currch >= 'A' && currch <= 'F')
        || (currch >= '0' && currch <= '9')) {
      hexcharscnt++;
      continue;
    }

    charscnt++;
  }

  if (hexcharscnt == 0 || charscnt > 2) {
    tsnc_source_report_error(source, startpos, startpos + charscnt - 1,
        "Invalid hexadecimal literal", NULL);
    return 0;
  }

  charscnt = hexcharscnt + charscnt;

  hexbuf = (char*)malloc(charscnt + 1);
  assert(hexbuf && "Unable to allocate memory for hex number token");

  fseek(srcfp, startpos, SEEK_SET);
  fread(hexbuf, sizeof(char), charscnt, srcfp);
  hexbuf[charscnt] = '\0';

  fseek(srcfp, startpos + charscnt, SEEK_SET);

  dest->kind = TSNC_TOKEN_KIND_NUMBER;
  dest->startpos = startpos;
  dest->endpos = startpos + charscnt - 1;
  dest->str = hexbuf;
  return 1;
}

static int tsnc_tokenize_number_bin(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *srcfp = source->fp;
  size_t startpos=0, charscnt=0, bincharscnt=0;
  char currch, *binbuf;

  startpos = ftell(srcfp);

  if (fgetc(srcfp) != '0' || fgetc(srcfp) != 'b') {
    fseek(srcfp, startpos, SEEK_SET);
    return 0;
  }

  charscnt += 2;

  while ((currch = fgetc(srcfp)) && currch != ' ' && currch != EOF) {
    if (currch == '0' || currch == '1') {
      bincharscnt++;
      continue;
    }

    charscnt++;
  }

  if (bincharscnt == 0 || charscnt > 2) {
    charscnt = bincharscnt + charscnt;
    tsnc_source_report_error(source, startpos, startpos + charscnt - 1,
        "Invalid binary number literal", NULL);
    return 0;
  }

  charscnt = bincharscnt + charscnt;

  binbuf = (char*)malloc(charscnt + 1);
  assert(binbuf && "Unable to allocate memory for bin number token");

  fseek(srcfp, startpos, SEEK_SET);
  fread(binbuf, sizeof(char), charscnt, srcfp);
  binbuf[charscnt] = '\0';

  fseek(srcfp, startpos + charscnt, SEEK_SET);

  dest->kind = TSNC_TOKEN_KIND_NUMBER;
  dest->startpos = startpos;
  dest->endpos = startpos + charscnt - 1;
  dest->str = binbuf;
  return 1;
}

static int tsnc_tokenize_number_octal(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *srcfp = source->fp;
  size_t startpos=0, charscnt=0, octcharscnt=0;
  char currch, *octbuf;

  startpos = ftell(srcfp);

  if (fgetc(srcfp) != '0' || fgetc(srcfp) != 'o') {
    fseek(srcfp, startpos, SEEK_SET);
    return 0;
  }

  charscnt += 2;

  while ((currch = fgetc(srcfp)) && currch != ' ' && currch != EOF) {
    if (currch >= '0' && currch <= '7') {
      octcharscnt++;
      continue;
    }

    charscnt++;
  }

  if (octcharscnt == 0 || charscnt > 2) {
    charscnt = octcharscnt + charscnt;
    tsnc_source_report_error(source, startpos, startpos + charscnt - 1,
        "Invalid octal number literal", NULL);
    return 0;
  }

  charscnt = octcharscnt + charscnt;

  octbuf = (char*)malloc(charscnt + 1);
  assert(octbuf && "Unable to allocate memory for octal number token");

  fseek(srcfp, startpos, SEEK_SET);
  fread(octbuf, sizeof(char), charscnt, srcfp);
  octbuf[charscnt] = '\0';

  fseek(srcfp, startpos + charscnt, SEEK_SET);

  dest->kind = TSNC_TOKEN_KIND_NUMBER;
  dest->startpos = startpos;
  dest->endpos = startpos + charscnt - 1;
  dest->str = octbuf;
  return 1;
}

// 0123 123 123.456 .456 123n 0xff 0b111111
static int tsnc_tokenize_number(struct tsnc_token *dest,
    struct tsnc_source *source) {
  size_t charscnt = 0, currpos = 0, startpos = 0;
  FILE *srcfp = source->fp;
  int decpoint = 0;
  char currch, *numbuf;

  startpos = ftell(srcfp);

  // 0x...
  if (fgetc(srcfp) == '0' && fgetc(srcfp) == 'x') {
    fseek(srcfp, startpos, SEEK_SET);
    return tsnc_tokenize_number_hex(dest, source);
  }

  // 0b...
  fseek(srcfp, startpos, SEEK_SET);
  if (fgetc(srcfp) == '0' && fgetc(srcfp) == 'b') {
    fseek(srcfp, startpos, SEEK_SET);
    return tsnc_tokenize_number_bin(dest, source);
  }

  // 0o...
  fseek(srcfp, startpos, SEEK_SET);
  if (fgetc(srcfp) == '0' && fgetc(srcfp) == 'o') {
    fseek(srcfp, startpos, SEEK_SET);
    return tsnc_tokenize_number_octal(dest, source);
  }

  fseek(srcfp, startpos, SEEK_SET);
  while ((currch = fgetc(srcfp)) != EOF && currch != '\n') {
    currpos = ftell(srcfp);

    if (decpoint == 1 && currch == '.') {
      tsnc_source_report_error(source, startpos, currpos,
          "Invalid number", NULL);
      return 0;
    }

    if (decpoint == 0 && currch == '.')
      decpoint = 1;

    charscnt++;
  }

  numbuf = (char*)malloc(charscnt + 1);
  assert(numbuf && "Unable to allocate memory for number token");

  fseek(srcfp, startpos, SEEK_SET);
  fread(numbuf, sizeof(char), charscnt, srcfp);

  dest->kind = TSNC_TOKEN_KIND_NUMBER;
  dest->startpos = startpos;
  dest->endpos = startpos + charscnt;
  dest->str = numbuf;

  return 1;
}

static int tsnc_token_source_next(struct tsnc_token *dest,
    struct tsnc_source *source) {
  char currch, tokbuf[4];
  size_t startpos;
  FILE *srcfp = source->fp;

  while ((currch = fgetc(srcfp)) != EOF) {
    startpos = ftell(srcfp) - 1;

    switch (currch) {
      case ' ':
      case '\n':
        continue;
      /* single character tokens */
      case ',': case '#':
      case '@': case '\\':
      case '(': case ')':
      case '{': case '}':
      case '[': case ']':
      case ';': case ':':
      case '~':
        tsnc_token_create(dest, currch, &currch, 1,
            startpos, startpos);
        return 1;
      case '0': case '1':
      case '2': case '3':
      case '4': case '5':
      case '6': case '7':
      case '8': case '9':
        fseek(srcfp, startpos, SEEK_SET);
        if (tsnc_tokenize_number(dest, source))
          return 1;
        break;

        // 0b22test
      case '.':
        currch = fgetc(srcfp);
        if (currch >= '0' && currch <= '9') {
          fseek(srcfp, -1, SEEK_CUR);
          return tsnc_tokenize_number(dest, source);
        }
        break;
      case '"':
      case '\'':
        fseek(srcfp, startpos, SEEK_SET);
        if (tsnc_tokenize_source_string(dest, source))
          return 1;
        break;
      // operators
      case '+': {
        // ++
        if (fgetc(srcfp) == '+') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_PLUS_PLUS,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // +=
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_PLUS_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // +
        tsnc_token_create(dest, TSNC_TOKEN_KIND_PLUS,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '-': {
        // --
        if (fgetc(srcfp) == '-') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_MINUS_MINUS,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // -=
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_MINUS_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // -
        tsnc_token_create(dest, TSNC_TOKEN_KIND_MINUS,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '/': {
        // /=
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_SLASH_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // /
        tsnc_token_create(dest, TSNC_TOKEN_KIND_SLASH,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '*': {
        // **=
        if (fgetc(srcfp) == '*' && fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 3, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_ASTERISK_ASTERISK_EQUAL,
              tokbuf, 3, startpos, startpos + 2);
          return 1;
        }

        // **
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '*') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_ASTERISK_ASTERISK,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // *=
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_ASTERISK_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // *
        tsnc_token_create(dest, TSNC_TOKEN_KIND_ASTERISK,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '=': {
        // ===
        if (fgetc(srcfp) == '=' && fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 3, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_EQUAL_EQUAL_EQUAL,
              tokbuf, 3, startpos, startpos + 2);
          return 1;
        }

        // ==
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_EQUAL_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // =
        tsnc_token_create(dest, TSNC_TOKEN_KIND_EQUAL,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '!': {
        // !==
        if (fgetc(srcfp) == '=' && fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 3, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL_EQUAL,
              tokbuf, 3, startpos, startpos + 2);
          return 1;
        }

        // !=
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // =
        tsnc_token_create(dest, TSNC_TOKEN_KIND_EXCLAMATION_MARK,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '<': {
        // <<=
        if (fgetc(srcfp) == '<' && fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 3, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN_EQUAL,
              tokbuf, 3, startpos, startpos + 2);
          return 1;
        }

        // <<
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '<') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // <=
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_LESS_THAN_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // <
        tsnc_token_create(dest, TSNC_TOKEN_KIND_LESS_THAN,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '>': {
        // >>>=
        if (fgetc(srcfp) == '>' && fgetc(srcfp) == '>' && fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 4, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN_EQUAL,
              tokbuf, 4, startpos, startpos + 3);
          return 1;
        }

        // >>=
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '>' && fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 3, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_EQUAL,
              tokbuf, 3, startpos, startpos + 2);
          return 1;
        }

        // >>>
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '>' && fgetc(srcfp) == '>') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 3, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN,
              tokbuf, 3, startpos, startpos + 2);
          return 1;
        }

        // >>
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '>') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // >=
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_GREATER_THAN_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // >
        tsnc_token_create(dest, TSNC_TOKEN_KIND_GREATER_THAN,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '^': {
        // ^=
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_CARET_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // ^
        tsnc_token_create(dest, TSNC_TOKEN_KIND_CARET,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '&': {
        // &&
        if (fgetc(srcfp) == '&') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_AMPERSAND_AMPERSAND,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // &=
        fseek(srcfp, startpos + 1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_AMPERSAND_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // &
        tsnc_token_create(dest, TSNC_TOKEN_KIND_AMPERSAND,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '|': {
        // ||
        if (fgetc(srcfp) == '|') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_VERTICAL_BAR_VERTICAL_BAR,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // |=
        fseek(srcfp, startpos+1, SEEK_SET);
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_VERTICAL_BAR_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // |
        tsnc_token_create(dest, TSNC_TOKEN_KIND_VERTICAL_BAR,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '%': {
        // %=
        if (fgetc(srcfp) == '=') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_PERCENTAGE_EQUAL,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // %
        tsnc_token_create(dest, TSNC_TOKEN_KIND_PERCENTAGE,
            &currch, 1, startpos, startpos);
        return 1;
      }
      case '?': {
        // ??
        if (fgetc(srcfp) ==  '?') {
          fseek(srcfp, startpos, SEEK_SET);
          fread(tokbuf, sizeof(char), 2, srcfp);
          tsnc_token_create(dest, TSNC_TOKEN_KIND_QUESTION_MARK_QUESTION_MARK,
              tokbuf, 2, startpos, startpos + 1);
          return 1;
        }

        // ?
        tsnc_token_create(dest, TSNC_TOKEN_KIND_QUESTION_MARK,
            &currch, 1, startpos, startpos);
        return 1;
      }

      // TODO: add . ... .123
    }
  }

  return 0;
}

void tsnc_tokenize_source(struct tsnc_source *source) {
  struct tsnc_token token;

  // TODO: source->fp is NULL when the source is empty
  if (source == NULL || source->fp == NULL)
    return;

  while (tsnc_token_source_next(&token, source)) {
    tsnc_vector_push(&source->tokenv,
        sizeof(struct tsnc_token), &token);
  }
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

int tsnc_token_cleanup(struct tsnc_token *token) {
  if (token == NULL)
    return 0;

  free(token->str);
  return 1;
}

int tsnc_tokenv_cleanup(struct tsnc_vector *tokenv) {
  struct tsnc_token token;

  if (tokenv == NULL)
    return 0;

  while (tsnc_vector_iter(&token, tokenv, sizeof(struct tsnc_token))) {
    tsnc_token_cleanup(&token);
  }

  return 1;
}
