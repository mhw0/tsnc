#ifndef TSNC_TOKEN_H
#define TSNC_TOKEN_H
#include <stdlib.h>
#include <tsnc/vector.h>
#include <tsnc/source.h>

/* token kind */
enum tsnc_token_kind {
  TSNC_TOKEN_KIND_QUESTION_MARK                                 = '?',
  TSNC_TOKEN_KIND_EXCLAMATION_MARK                              = '!',
  TSNC_TOKEN_KIND_DOUBLE_QUOTE                                  = '"',
  TSNC_TOKEN_KIND_SINGLE_QUOTE                                  = '\'',
  TSNC_TOKEN_KIND_GRAVE_ACCENT                                  = '`',
  TSNC_TOKEN_KIND_HASHTAG                                       = '#',
  TSNC_TOKEN_KIND_ASPERAND                                      = '@',
  TSNC_TOKEN_KIND_PERCENTAGE                                    = '%',
  TSNC_TOKEN_KIND_AMPERSAND                                     = '&',
  TSNC_TOKEN_KIND_ROUND_BRACKET_OPEN                            = '(',
  TSNC_TOKEN_KIND_ROUND_BRACKET_CLOSE                           = ')',
  TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN                            = '{',
  TSNC_TOKEN_KIND_CURLY_BRACKET_CLOSE                           = '}',
  TSNC_TOKEN_KIND_SQUARE_BRACKET_OPEN                           = '[',
  TSNC_TOKEN_KIND_SQUARE_BRACKET_CLOSE                          = ']',
  TSNC_TOKEN_KIND_VERTICAL_BAR                                  = '|',
  TSNC_TOKEN_KIND_ASTERISK                                      = '*',
  TSNC_TOKEN_KIND_CARET                                         = '^',
  TSNC_TOKEN_KIND_PLUS                                          = '+',
  TSNC_TOKEN_KIND_MINUS                                         = '-',
  TSNC_TOKEN_KIND_COMMA                                         = ',',
  TSNC_TOKEN_KIND_DOT                                           = '.',
  TSNC_TOKEN_KIND_COLON                                         = ':',
  TSNC_TOKEN_KIND_SEMICOLON                                     = ';',
  TSNC_TOKEN_KIND_SLASH                                         = '/',
  TSNC_TOKEN_KIND_BACKSLASH                                     = '\\',
  TSNC_TOKEN_KIND_GREATER_THAN                                  = '>',
  TSNC_TOKEN_KIND_LESS_THAN                                     = '<',
  TSNC_TOKEN_KIND_EQUAL                                         = '=',
  TSNC_TOKEN_KIND_TILDE                                         = '~',

  TSNC_TOKEN_KIND_QUESTION_MARK_QUESTION_MARK,                  // ??
  TSNC_TOKEN_KIND_AMPERSAND_AMPERSAND,                          // &&
  TSNC_TOKEN_KIND_VERTICAL_BAR_VERTICAL_BAR,                    // ||
  TSNC_TOKEN_KIND_ASTERISK_ASTERISK,                            // **
  TSNC_TOKEN_KIND_EQUAL_EQUAL,                                  // ==
  TSNC_TOKEN_KIND_EQUAL_EQUAL_EQUAL,                            // ===
  TSNC_TOKEN_KIND_PERCENTAGE_EQUAL,                             // %=
  TSNC_TOKEN_KIND_VERTICAL_BAR_EQUAL,                           // |=
  TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL,                       // !=
  TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL_EQUAL,                 // !==
  TSNC_TOKEN_KIND_CARET_EQUAL,                                  // ^=
  TSNC_TOKEN_KIND_ASTERISK_EQUAL,                               // *=
  TSNC_TOKEN_KIND_AMPERSAND_EQUAL,                              // *=
  TSNC_TOKEN_KIND_ASTERISK_ASTERISK_EQUAL,                      // **=
  TSNC_TOKEN_KIND_SLASH_EQUAL,                                  // /=
  TSNC_TOKEN_KIND_PLUS_PLUS,                                    // ++
  TSNC_TOKEN_KIND_MINUS_MINUS,                                  // --
  TSNC_TOKEN_KIND_PLUS_EQUAL,                                   // +=
  TSNC_TOKEN_KIND_MINUS_EQUAL,                                  // -=
  TSNC_TOKEN_KIND_LESS_THAN_EQUAL,                              // <=
  TSNC_TOKEN_KIND_GREATER_THAN_EQUAL,                           // >=
  TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN,                          // <<
  TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN_EQUAL,                    // <<=
  TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN,                    // >>
  TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_EQUAL,              // >>=
  TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN,       // >>>
  TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN_EQUAL, // >>>=

  TSNC_TOKEN_KIND_NUMBER,
  TSNC_TOKEN_KIND_STRING,
  TSNC_TOKEN_KIND_BIGNUMBER,
  TSNC_TOKEN_KIND_KEYWORD,
  TSNC_TOKEN_KIND_IDENTIFIER
};

/* holds token fields */
struct tsnc_token {
  /* token kind */
  enum tsnc_token_kind kind;
  /* token string */
  char *str;
  /* start position of token */
  size_t startpos;
  /* end position of token */
  size_t endpos;
};

int tsnc_token_create(struct tsnc_token *dest,
    enum tsnc_token_kind kind, char *token, size_t len,
    size_t startpos, size_t endpos);
void tsnc_tokenize_source(struct tsnc_source *source);
int tsnc_token_equal(struct tsnc_token *left,
    struct tsnc_token *right);
int tsnc_tokenv_cleanup(struct tsnc_vector *tokenv);
int tsnc_token_cleanup(struct tsnc_token *token);

#endif
