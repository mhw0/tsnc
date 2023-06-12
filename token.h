#ifndef TSNC_TOKEN_H
#define TSNC_TOKEN_H
#include "source.h"
#include "vector.h"

enum tsnc_token_kind {
  TSNC_TOKEN_QUESTION_MARK                                 = '?',
  TSNC_TOKEN_EXCLAMATION_MARK                              = '!',
  TSNC_TOKEN_DOUBLE_QUOTE                                  = '"',
  TSNC_TOKEN_SINGLE_QUOTE                                  = '\'',
  TSNC_TOKEN_GRAVE_ACCENT                                  = '`',
  TSNC_TOKEN_HASHTAG                                       = '#',
  TSNC_TOKEN_ASPERAND                                      = '@',
  TSNC_TOKEN_PERCENTAGE                                    = '%',
  TSNC_TOKEN_AMPERSAND                                     = '&',
  TSNC_TOKEN_ROUND_BRACKET_OPEN                            = '(',
  TSNC_TOKEN_ROUND_BRACKET_CLOSE                           = ')',
  TSNC_TOKEN_CURLY_BRACKET_OPEN                            = '{',
  TSNC_TOKEN_CURLY_BRACKET_CLOSE                           = '}',
  TSNC_TOKEN_SQUARE_BRACKET_OPEN                           = '[',
  TSNC_TOKEN_SQUARE_BRACKET_CLOSE                          = ']',
  TSNC_TOKEN_VERTICAL_BAR                                  = '|',
  TSNC_TOKEN_ASTERISK                                      = '*',
  TSNC_TOKEN_CARET                                         = '^',
  TSNC_TOKEN_PLUS                                          = '+',
  TSNC_TOKEN_MINUS                                         = '-',
  TSNC_TOKEN_COMMA                                         = ',',
  TSNC_TOKEN_DOT                                           = '.',
  TSNC_TOKEN_COLON                                         = ':',
  TSNC_TOKEN_SEMICOLON                                     = ';',
  TSNC_TOKEN_SLASH                                         = '/',
  TSNC_TOKEN_BACKSLASH                                     = '\\',
  TSNC_TOKEN_GREATER_THAN                                  = '>',
  TSNC_TOKEN_LESS_THAN                                     = '<',
  TSNC_TOKEN_EQUAL                                         = '=',

  TSNC_TOKEN_ASTERISK_ASTERISK,                            // **
  TSNC_TOKEN_EQUAL_EQUAL,                                  // ==
  TSNC_TOKEN_EQUAL_EQUAL_EQUAL,                            // ===
  TSNC_TOKEN_EXCLAMATION_MARK_EQUAL,                       // !=
  TSNC_TOKEN_EXCLAMATION_MARK_EQUAL_EQUAL,                 // !==
  TSNC_TOKEN_CARET_EQUAL,                                  // ^=
  TSNC_TOKEN_ASTERISK_EQUAL,                               // *=
  TSNC_TOKEN_AMPERSAND_EQUAL,                              // *=
  TSNC_TOKEN_ASTERISK_ASTERISK_EQUAL,                      // **=
  TSNC_TOKEN_SLASH_EQUAL,                                  // /=
  TSNC_TOKEN_PLUS_PLUS,                                    // ++
  TSNC_TOKEN_MINUS_MINUS,                                  // --
  TSNC_TOKEN_PLUS_EQUAL,                                   // +=
  TSNC_TOKEN_MINUS_EQUAL,                                  // -=
  TSNC_TOKEN_LESS_THAN_EQUAL,                              // <=
  TSNC_TOKEN_GREATER_THAN_EQUAL,                           // >=
  TSNC_TOKEN_LESS_THAN_LESS_THAN,                          // <<
  TSNC_TOKEN_LESS_THAN_LESS_THAN_EQUAL,                    // <<=
  TSNC_TOKEN_GREATER_THAN_GREATER_THAN,                    // >>
  TSNC_TOKEN_GREATER_THAN_GREATER_THAN_EQUAL,              // >>=
  TSNC_TOKEN_GREATER_THAN_GREATER_THAN_GREATER_THAN,       // >>>
  TSNC_TOKEN_GREATER_THAN_GREATER_THAN_GREATER_THAN_EQUAL, // >>>=


  // https://github.com/microsoft/TypeScript/issues/2536#issuecomment-87194347
  TSNC_TOKEN_KEYWORD_BREAK,
  TSNC_TOKEN_KEYWORD_CASE,
  TSNC_TOKEN_KEYWORD_CATCH,
  TSNC_TOKEN_KEYWORD_CLASS,
  TSNC_TOKEN_KEYWORD_CONST,
  TSNC_TOKEN_KEYWORD_CONTINUE,
  TSNC_TOKEN_KEYWORD_DEBUGGER,
  TSNC_TOKEN_KEYWORD_DEFAULT,
  TSNC_TOKEN_KEYWORD_DELETE,
  TSNC_TOKEN_KEYWORD_DO,
  TSNC_TOKEN_KEYWORD_ELSE,
  TSNC_TOKEN_KEYWORD_ENUM,
  TSNC_TOKEN_KEYWORD_EXPORT,
  TSNC_TOKEN_KEYWORD_EXTENDS,
  TSNC_TOKEN_KEYWORD_FOR,
  TSNC_TOKEN_KEYWORD_FROM,
  TSNC_TOKEN_KEYWORD_FUNCTION,
  TSNC_TOKEN_KEYWORD_IF,
  TSNC_TOKEN_KEYWORD_IMPLEMENTS,
  TSNC_TOKEN_KEYWORD_IMPORT,
  TSNC_TOKEN_KEYWORD_IN,
  TSNC_TOKEN_KEYWORD_INSTANCEOF,
  TSNC_TOKEN_KEYWORD_INTERFACE,
  TSNC_TOKEN_KEYWORD_NEW,
  TSNC_TOKEN_KEYWORD_PACKAGE,
  TSNC_TOKEN_KEYWORD_PRIVATE,
  TSNC_TOKEN_KEYWORD_PROTECTED,
  TSNC_TOKEN_KEYWORD_PUBLIC,
  TSNC_TOKEN_KEYWORD_RETURN,
  TSNC_TOKEN_KEYWORD_STATIC,
  TSNC_TOKEN_KEYWORD_SUPER,
  TSNC_TOKEN_KEYWORD_SWITCH,
  TSNC_TOKEN_KEYWORD_THIS,
  TSNC_TOKEN_KEYWORD_THROW,
  TSNC_TOKEN_KEYWORD_TRY,
  TSNC_TOKEN_KEYWORD_TYPEOF,
  TSNC_TOKEN_KEYWORD_VAR,
  TSNC_TOKEN_KEYWORD_VOID,
  TSNC_TOKEN_KEYWORD_WHILE,
  TSNC_TOKEN_KEYWORD_WITH,
  TSNC_TOKEN_KEYWORD_YIELD,
  TSNC_TOKEN_KEYWORD_AS,
  TSNC_TOKEN_KEYWORD_STRING,
  TSNC_TOKEN_KEYWORD_TRUE,
  TSNC_TOKEN_KEYWORD_FALSE,
  TSNC_TOKEN_KEYWORD_UNDEFINED,
  TSNC_TOKEN_KEYWORD_NULL,
  TSNC_TOKEN_KEYWORD_NUMBER,

  TSNC_TOKEN_NUMBER,
  TSNC_TOKEN_STRING,

  TSNC_TOKEN_IDENTIFIER,
};

struct tsnc_token {
  enum tsnc_token_kind kind;
  char *ptr;
  size_t pos;
  size_t end;
};

int tsnc_tokenize_source(struct tsnc_source *source);

#endif
