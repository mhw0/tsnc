#ifndef TSNC_TOKEN_H
#define TSNC_TOKEN_H
#include <stdlib.h>
#include <tsnc/vector.h>

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
  TSNC_TOKEN_KIND_IDENTIFIER,
  TSNC_TOKEN_KIND_EOL,
  TSNC_TOKEN_KIND_EOF,

  TSNC_TOKEN_KIND_KEYWORD,
  TSNC_TOKEN_KIND_KEYWORD_AS,
  TSNC_TOKEN_KIND_KEYWORD_BREAK,
  TSNC_TOKEN_KIND_KEYWORD_CASE,
  TSNC_TOKEN_KIND_KEYWORD_CATCH,
  TSNC_TOKEN_KIND_KEYWORD_CLASS,
  TSNC_TOKEN_KIND_KEYWORD_CONST,
  TSNC_TOKEN_KIND_KEYWORD_CONTINUE,
  TSNC_TOKEN_KIND_KEYWORD_DEBUGGER,
  TSNC_TOKEN_KIND_KEYWORD_DEFAULT,
  TSNC_TOKEN_KIND_KEYWORD_DELETE,
  TSNC_TOKEN_KIND_KEYWORD_DO,
  TSNC_TOKEN_KIND_KEYWORD_ELSE,
  TSNC_TOKEN_KIND_KEYWORD_ENUM,
  TSNC_TOKEN_KIND_KEYWORD_EXPORT,
  TSNC_TOKEN_KIND_KEYWORD_EXTENDS,
  TSNC_TOKEN_KIND_KEYWORD_FALSE,
  TSNC_TOKEN_KIND_KEYWORD_FINALLY,
  TSNC_TOKEN_KIND_KEYWORD_FOR,
  TSNC_TOKEN_KIND_KEYWORD_FROM,
  TSNC_TOKEN_KIND_KEYWORD_FUNCTION,
  TSNC_TOKEN_KIND_KEYWORD_IF,
  TSNC_TOKEN_KIND_KEYWORD_IMPLEMENTS,
  TSNC_TOKEN_KIND_KEYWORD_IMPORT,
  TSNC_TOKEN_KIND_KEYWORD_IN,
  TSNC_TOKEN_KIND_KEYWORD_INSTANCEOF,
  TSNC_TOKEN_KIND_KEYWORD_INTERFACE,
  TSNC_TOKEN_KIND_KEYWORD_LET,
  TSNC_TOKEN_KIND_KEYWORD_NEW,
  TSNC_TOKEN_KIND_KEYWORD_NULL,
  TSNC_TOKEN_KIND_KEYWORD_PACKAGE,
  TSNC_TOKEN_KIND_KEYWORD_PRIVATE,
  TSNC_TOKEN_KIND_KEYWORD_PROTECTED,
  TSNC_TOKEN_KIND_KEYWORD_PUBLIC,
  TSNC_TOKEN_KIND_KEYWORD_RETURN,
  TSNC_TOKEN_KIND_KEYWORD_STATIC,
  TSNC_TOKEN_KIND_KEYWORD_SUPER,
  TSNC_TOKEN_KIND_KEYWORD_SWITCH,
  TSNC_TOKEN_KIND_KEYWORD_THIS,
  TSNC_TOKEN_KIND_KEYWORD_THROW,
  TSNC_TOKEN_KIND_KEYWORD_TRUE,
  TSNC_TOKEN_KIND_KEYWORD_TRY,
  TSNC_TOKEN_KIND_KEYWORD_TYPEOF,
  TSNC_TOKEN_KIND_KEYWORD_VAR,
  TSNC_TOKEN_KIND_KEYWORD_VOID,
  TSNC_TOKEN_KIND_KEYWORD_WHILE,
  TSNC_TOKEN_KIND_KEYWORD_WITH,
  TSNC_TOKEN_KIND_KEYWORD_YIELD,
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

int tsnc_token_create(struct tsnc_token *dest, enum tsnc_token_kind kind,
    char *token, size_t len, size_t startpos, size_t endpos);
int tsnc_token_equal(struct tsnc_token *left, struct tsnc_token *right);
void tsnc_token_free(struct tsnc_token *token);

#endif
