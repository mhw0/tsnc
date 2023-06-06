#ifndef TSNC_TOKEN_H
#define TSNC_TOKEN_H
#include "source.h"

enum tsnc_token_kind {
  TSNC_TOKEN_EXCLAMATION_MARK                             = '!',
  TSNC_TOKEN_DOUBLE_QUOTE                                 = '"',
  TSNC_TOKEN_SIGNLE_QUOTE                                 = '\'',
  TSNC_TOKEN_HASHTAG                                      = '#',
  TSNC_TOKEN_ASPERAND                                     = '@',
  TSNC_TOKEN_DOLLAR_SIGN                                  = '$',
  TSNC_TOKEN_PERCENTAGE                                   = '%',
  TSNC_TOKEN_AMPERSAND                                    = '&',
  TSNC_TOKEN_ROUND_BRACKET_OPEN                           = '(',
  TSNC_TOKEN_ROUND_BRACKET_CLOSE                          = ')',
  TSNC_TOKEN_CURLY_BRACKET_OPEN                           = '{',
  TSNC_TOKEN_CURLY_BRACKET_CLOSE                          = '}',
  TSNC_TOKEN_SQUARE_BRACKET_OPEN                          = '[',
  TSNC_TOKEN_SQUARE_BRACKET_CLOSE                         = ']',
  TSNC_TOKEN_ASTERISK                                     = '*',
  TSNC_TOKEN_CARET                                        = '^',
  TSNC_TOKEN_PLUS                                         = '+',
  TSNC_TOKEN_MINUS                                        = '-',
  TSNC_TOKEN_COMMA                                        = ',',
  TSNC_TOKEN_DOT                                          = '.',
  TSNC_TOKEN_COLON                                        = ':',
  TSNC_TOKEN_SEMICOLON                                    = ';',
  TSNC_TOKEN_SLASH                                        = '/',
  TSNC_TOKEN_BACKSLASH                                    = '\\',
  TSNC_TOKEN_GREATER_THAN                                 = '>',
  TSNC_TOKEN_LESS_THAN                                    = '<',
  TSNC_TOKEN_EQUAL                                        = '=',

  TSNC_TOKEN_TEXT,                                        // text

  TSNC_TOKEN_ASTERISK_ASTERISK,                           // **
  TSNC_TOKEN_EQUAL_EQUAL,                                 // ==
  TSNC_TOKEN_EQUAL_EQUAL_EQUAL,                           // ===
  TSNC_TOKEN_EXCLAMATION_MARK_EQUAL,                      // !=
  TSNC_TOKEN_EXCLAMATION_MARK_EQUAL_EQUAL,                // !==
  TSNC_TOKEN_CARET_EQUAL,                                 // ^=
  TSNC_TOKEN_ASTERISK_EQUAL,                              // *=
  TSNC_TOKEN_AMPERSAND_EQUAL,                              // *=
  TSNC_TOKEN_ASTERISK_ASTERISK_EQUAL,                     // **=
  TSNC_TOKEN_SLASH_EQUAL,                                 // /=
  TSNC_TOKEN_PLUS_PLUS,                                   // ++
  TSNC_TOKEN_MINUS_MINUS,                                 // --

  TSNC_TOKEN_PLUS_EQUAL,                                  // +=
  TSNC_TOKEN_MINUS_EQUAL,                                 // -=
  TSNC_TOKEN_LESS_THAN_EQUAL,                             // <=
  TSNC_TOKEN_GREATER_THAN_EQUAL,                          // >=

  TSNC_TOKEN_LESS_THAN_LESS_THAN,                         // <<
  TSNC_TOKEN_LESS_THAN_LESS_THAN_EQUAL,                   // <<=
  TSNC_TOKEN_GRETER_THAN_GREATER_THAN,                    // >>
  TSNC_TOKEN_GRETER_THAN_GREATER_THAN_EQUAL,              // >>=
  TSNC_TOKEN_GRETER_THAN_GREATER_THAN_GREATER_THAN,       // >>>
  TSNC_TOKEN_GRETER_THAN_GREATER_THAN_GREATER_THAN_EQUAL, // >>>=
};

struct tsnc_token {
  enum tsnc_token_kind kind;
  char *ptr;
  size_t pos;
  size_t end;
};

struct tsnc_token_stream {
  struct tsnc_token *tokens[1024]; // FIXME
  size_t curs;
  size_t len;
};

int tsnc_tokenize_source(struct tsnc_source *source);

#endif
