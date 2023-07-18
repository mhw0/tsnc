#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <tsnc/tokenizer.h>

#define tsnc_is_identchar(ch) \
  ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') \
    || (ch >= '0' && ch <= '9') || (ch == '$' || ch == '_'))

static int tsnc_tokenize_string(struct tsnc_token *dest,
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
    endpos = startpos + charscnt;
    tsnc_source_report_error(source, endpos, endpos,
        "Unterminated string literal.", NULL);
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

/**
 * HexIntegerLiteral ::
 *   0x HexDigit
 *   0X HexDigit
 *   HexIntegerLiteral HexDigit
 *
 *   HexDigit :: one of
 *   0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F
 */
static int tsnc_tokenize_number_hex(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *srcfp = source->fp;
  size_t startpos=0, charscnt=0, hexcharscnt=0;
  char currch, *hexbuf;

  startpos = ftell(srcfp);

  fseek(srcfp, 2, SEEK_CUR);
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

/**
 * TODO: ES6 feature
 *
 * BinaryIntegerLiteral ::
 *   0b BinaryDigits
 *   0B BinaryDigits
 *
 * BinaryDigits ::
 *   BinaryDigit
 *   BinaryDigits BinaryDigit
 *
 * BinaryDigit :: one of
 *   0 1
 */
static int tsnc_tokenize_number_bin(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *srcfp = source->fp;
  size_t startpos=0, charscnt=0, bincharscnt=0;
  char currch, *binbuf;

  startpos = ftell(srcfp);

  fseek(srcfp, 2, SEEK_CUR);
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

/**
 * OctalIntegerLiteral ::
 *   0 OctalDigit
 *   OctalIntegerLiteral OctalDigit
 *
 * OctalDigit :: one of
 *   0 1 2 3 4 5 6 7
 */
static int tsnc_tokenize_number_octal(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *srcfp = source->fp;
  size_t startpos=0, charscnt=0, octcharscnt=0;
  char currch, *octbuf;

  startpos = ftell(srcfp);

  fseek(srcfp, 2, SEEK_CUR);
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
  if (fgetc(srcfp) == '0' && tolower(fgetc(srcfp)) == 'x') {
    fseek(srcfp, startpos, SEEK_SET);
    return tsnc_tokenize_number_hex(dest, source);
  }

  // 0b...
  fseek(srcfp, startpos, SEEK_SET);
  if (fgetc(srcfp) == '0' && tolower(fgetc(srcfp)) == 'b') {
    fseek(srcfp, startpos, SEEK_SET);
    return tsnc_tokenize_number_bin(dest, source);
  }

  // 0o...
  fseek(srcfp, startpos, SEEK_SET);
  if (fgetc(srcfp) == '0' && tolower(fgetc(srcfp)) == 'o') {
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

int tsnc_tokenize_keyword(struct tsnc_token *dest,
    struct tsnc_source *source) {
  size_t startpos=0, charscnt=0;
  FILE *srcfp = source->fp;
  char currch, *kwbuf;

  startpos = ftell(srcfp);

  while ((currch = fgetc(srcfp)) != ' ' && currch != EOF
      && (currch >= 'a' && currch <= 'z'))
    charscnt++;

  kwbuf = (char*)malloc(charscnt);
  assert(kwbuf && "Unable to allocate memory for keyword buffer");

  fseek(srcfp, startpos, SEEK_SET);
  fread(kwbuf, sizeof(char), charscnt, srcfp);
  kwbuf[charscnt] = '\0';

  if (strcmp(kwbuf, "as") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_AS;
  else if (strcmp(kwbuf, "break") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_BREAK;
  else if (strcmp(kwbuf, "case") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_CASE;
  else if (strcmp(kwbuf, "catch") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_CATCH;
  else if (strcmp(kwbuf, "class") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_CLASS;
  else if (strcmp(kwbuf, "const") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_CONST;
  else if (strcmp(kwbuf, "continue") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_CONTINUE;
  else if (strcmp(kwbuf, "debugger") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_DEBUGGER;
  else if (strcmp(kwbuf, "default") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_DEFAULT;
  else if (strcmp(kwbuf, "delete") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_DELETE;
  else if (strcmp(kwbuf, "do") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_DO;
  else if (strcmp(kwbuf, "else") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_ELSE;
  else if (strcmp(kwbuf, "enum") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_ENUM;
  else if (strcmp(kwbuf, "export") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_EXPORT;
  else if (strcmp(kwbuf, "extends") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_EXTENDS;
  else if (strcmp(kwbuf, "extends") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_EXTENDS;
  else if (strcmp(kwbuf, "false") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_FALSE;
  else if (strcmp(kwbuf, "finally") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_FINALLY;
  else if (strcmp(kwbuf, "for") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_FOR;
  else if (strcmp(kwbuf, "from") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_FROM;
  else if (strcmp(kwbuf, "function") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_FUNCTION;
  else if (strcmp(kwbuf, "if") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_IF;
  else if (strcmp(kwbuf, "implements") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_IMPLEMENTS;
  else if (strcmp(kwbuf, "import") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_IMPORT;
  else if (strcmp(kwbuf, "in") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_IN;
  else if (strcmp(kwbuf, "instanceof") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_INSTANCEOF;
  else if (strcmp(kwbuf, "interface") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_INTERFACE;
  else if (strcmp(kwbuf, "let") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_LET;
  else if (strcmp(kwbuf, "new") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_NEW;
  else if (strcmp(kwbuf, "null") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_NULL;
  else if (strcmp(kwbuf, "package") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_PACKAGE;
  else if (strcmp(kwbuf, "private") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_PRIVATE;
  else if (strcmp(kwbuf, "protected") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_PROTECTED;
  else if (strcmp(kwbuf, "public") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_PUBLIC;
  else if (strcmp(kwbuf, "return") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_RETURN;
  else if (strcmp(kwbuf, "static") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_STATIC;
  else if (strcmp(kwbuf, "super") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_SUPER;
  else if (strcmp(kwbuf, "switch") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_SWITCH;
  else if (strcmp(kwbuf, "this") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_THIS;
  else if (strcmp(kwbuf, "throw") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_THROW;
  else if (strcmp(kwbuf, "true") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_TRUE;
  else if (strcmp(kwbuf, "try") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_TRY;
  else if (strcmp(kwbuf, "typeof") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_TYPEOF;
  else if (strcmp(kwbuf, "var") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_VAR;
  else if (strcmp(kwbuf, "void") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_VOID;
  else if (strcmp(kwbuf, "while") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_WHILE;
  else if (strcmp(kwbuf, "with") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_WITH;
  else if (strcmp(kwbuf, "yield") == 0)
    dest->kind = TSNC_TOKEN_KIND_KEYWORD_YIELD;
  else {
    fseek(srcfp, startpos, SEEK_SET);
    free(kwbuf);
    return 0;
  }

  dest->startpos = startpos;
  dest->endpos = startpos + charscnt - 1;
  dest->str = kwbuf;
  return 1;
}

/**
 * Identifier ::
 *   IdentifierName but not ReservedWord
 *
 * IdentifierName ::
 *   IdentifierStart
 *   IdentifierName IdentifierPart
 *
 * IdentifierStart ::
 *   UnicodeLetter
 *   $
 *   _
 *   \ UnicodeEscapeSequence
 * ...
 */
int tsnc_tokenize_identifier(struct tsnc_token *dest,
    struct tsnc_source *source) {
  FILE *srcfp = source->fp;
  size_t startpos=0, charscnt=0;
  char currch, *identbuf;

  startpos = ftell(srcfp);

  while ((currch = fgetc(srcfp)) != ' ' && currch != EOF
      && tsnc_is_identchar(currch))
    charscnt++;

  identbuf = (char*)malloc(charscnt + 1);
  assert(identbuf && "Unable to allocate memory for identifier buffer");

  fseek(srcfp, startpos, SEEK_SET);
  fread(identbuf, sizeof(char), charscnt, srcfp);
  identbuf[charscnt] = '\0';

  dest->kind = TSNC_TOKEN_KIND_IDENTIFIER;
  dest->startpos = startpos;
  dest->endpos = startpos + charscnt - 1;
  dest->str = identbuf;
  return 1;
}

static int tsnc_token_source_next(struct tsnc_token *dest,
    struct tsnc_source *source) {
  char currch, tokbuf[4];
  size_t startpos;
  FILE *srcfp = source->fp;

  while ((currch = fgetc(srcfp))) {
    startpos = ftell(srcfp) - 1;

    switch (currch) {
      case EOF:
        tsnc_token_create(dest, TSNC_TOKEN_KIND_EOF, "", 0,
            startpos, startpos);
        return 1;
      case ' ':
        continue;
      case '\n':
        tsnc_token_create(dest, TSNC_TOKEN_KIND_EOL, "\n", 1,
            startpos, startpos);
        return 1;
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
        if (tsnc_tokenize_string(dest, source))
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
      default: {
        fseek(srcfp, -1, SEEK_CUR);

        if (tsnc_tokenize_keyword(dest, source))
          return 1;

        if (tsnc_is_identchar(currch))
          return tsnc_tokenize_identifier(dest, source);

        printf("Invalid character: %c\n", currch);
        exit(1);
      }

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
    tsnc_token_stream_add(&source->tokens, &token);

    if (token.kind == TSNC_TOKEN_KIND_EOF)
      break;
  }
}
