#ifndef TSNC_TOKEN_H
#define TSNC_TOKEN_H
#include <stdlib.h>
#include <tsnc/unicode.h>

/* token kind */
enum tsnc_token_kind {
  TokenKindQuestionMark = UnicodeQuestionMark,               // ?
  TokenKindExclamationMark = UnicodeExclamationMark,         // !
  TokenKindDoubleQuote = UnicodeQuotationMark,               // "
  TokenKindSingleQuote = UnicodeApostrophe,                  // '
  TokenKindGraveAccent = UnicodeGraveAccent,                 // `
  TokenKindHashtag = UnicodeNumberSign,                      // #
  TokenKindAsperand = UnicodeCommercialAt,                   // @
  TokenKindPercentage = UnicodePercentSign,                  // %
  TokenKindAmpersand = UnicodeAmpersand,                     // &
  TokenKindRoundBracketOpen = UnicodeLeftParenthesis,        // (
  TokenKindRoundBracketClose = UnicodeRightParenthesis,      // )
  TokenKindCurlyBracketOpen = UnicodeLeftCurlyBracket,       // {
  TokenKindCurlyBracketClose = UnicodeRightCurlyBracket,     // }
  TokenKindSquareBracketOpen = UnicodeLeftSquareBracket,     // [
  TokenKindSquareBracketClose = UnicodeRightSquareBracket,   // ]
  TokenKindVerticalBar = UnicodeVerticalLine,                // |
  TokenKindAsterisk = UnicodeAsterisk,                       // *
  TokenKindCaret = UnicodeCircumflexAccent,                  // ^
  TokenKindPlus = UnicodePlusSign,                           // +
  TokenKindMinus = UnicodeMinusSign,                         // -
  TokenKindComma = UnicodeComma,                             // ,
  TokenKindDot = UnicodeFullStop,                            // .
  TokenKindColon = UnicodeColon,                             //
  TokenKindSemicolon = UnicodeSemicolon,                     // ;
  TokenKindSlash = UnicodeSolidus,                           // /
  TokenKindBackslash = UnicodeReverseSolidus,                // "\"
  TokenKindGreaterThan = UnicodeGreaterThanSign,             // >
  TokenKindLessThan = UnicodeLessThanSign,                   // <
  TokenKindEqual = UnicodeEqualsSign,                        // =
  TokenKindTilde = UnicodeTilde,                             // ~
  TokenKindUnderscore = UnicodeLowLine,                      // _
  
  TokenKindQuestionMarkQuestionMark,                         // ??
  TokenKindAmpersandAmpersand,                               // &&
  TokenKindVerticalBarVerticalBar,                           // ||
  TokenKindVerticalBarVerticalBarEqual,                      // ||=
  TokenKindAsteriskAsterisk,                                 // **
  TokenKindEqualEqual,                                       // ==
  TokenKindEqualEqualEqual,                                  // ===
  TokenKindPercentageEqual,                                  // %=
  TokenKindVerticalBarEqual,                                 // |=
  TokenKindExclamationMarkEqual,                             // !=
  TokenKindExclamationMarkEqualEqual,                        // !==
  TokenKindDotDotDot,                                        // ...
  TokenKindCaretEqual,                                       // ^=
  TokenKindAsteriskEqual,                                    // *=
  TokenKindAmpersandEqual,                                   // &=
  TokenKindAsteriskAsteriskEqual,                            // **=
  TokenKindSlashEqual,                                       // /=
  TokenKindPlusPlus,                                         // ++
  TokenKindPlusEqual,                                        // +=
  TokenKindMinusMinus,                                       // --
  TokenKindMinusEqual,                                       // -=
  TokenKindLessThanEqual,                                    // <=
  TokenKindGreaterThanEqual,                                 // >=
  TokenKindLessThanLessThan,                                 // <<
  TokenKindLessThanLessThanEqual,                            // <<=
  TokenKindGreaterThanGreaterThan,                           // >>
  TokenKindGreaterThanGreaterThanEqual,                      // >>=
  TokenKindGreaterThanGreaterThanGreaterThan,                // >>>
  TokenKindGreaterThanGreaterThanGreaterThanEqual,           // >>>=

  TokenKindNumber,

  TokenKindEndOfFile,
  TokenKindInvalid,

  TokenKindLineTerminator,

  TokenKindKeywordAny,
  TokenKindKeywordAs,
  TokenKindKeywordAwait,
  TokenKindKeywordBoolean,
  TokenKindKeywordBreak,
  TokenKindKeywordCase,
  TokenKindKeywordCatch,
  TokenKindKeywordClass,
  TokenKindKeywordConst,
  TokenKindKeywordConstructor,
  TokenKindKeywordContinue,
  TokenKindKeywordDebugger,
  TokenKindKeywordDeclare,
  TokenKindKeywordDefault,
  TokenKindKeywordDelete,
  TokenKindKeywordDo,
  TokenKindKeywordElse,
  TokenKindKeywordEnum,
  TokenKindKeywordExport,
  TokenKindKeywordExtends,
  TokenKindKeywordFinally,
  TokenKindKeywordFor,
  TokenKindKeywordFrom,
  TokenKindKeywordFunction,
  TokenKindKeywordGet,
  TokenKindKeywordIf,
  TokenKindKeywordImplements,
  TokenKindKeywordImport,
  TokenKindKeywordIn,
  TokenKindKeywordInfer,
  TokenKindKeywordInstanceof,
  TokenKindKeywordInterface,
  TokenKindKeywordLet,
  TokenKindKeywordModule,
  TokenKindKeywordNew,
  TokenKindKeywordNumber,
  TokenKindKeywordOf,
  TokenKindKeywordPackage,
  TokenKindKeywordPrivate,
  TokenKindKeywordProtected,
  TokenKindKeywordPublic,
  TokenKindKeywordRequire,
  TokenKindKeywordReturn,
  TokenKindKeywordSet,
  TokenKindKeywordStatic,
  TokenKindKeywordString,
  TokenKindKeywordSuper,
  TokenKindKeywordSwitch,
  TokenKindKeywordSymbol,
  TokenKindKeywordThis,
  TokenKindKeywordThrow,
  TokenKindKeywordTry,
  TokenKindKeywordType,
  TokenKindKeywordTypeof,
  TokenKindKeywordVar,
  TokenKindKeywordVoid,
  TokenKindKeywordWhile,
  TokenKindKeywordWith,
  TokenKindKeywordYield,
};

/* holds token fields */
struct tsnc_token {
  /* token kind */
  enum tsnc_token_kind kind;
  /* token string */
  char *str;
  /* beginning position of token */
  size_t begpos;
  /* ending position of token */
  size_t endpos;
};

int tsnc_token_equal(struct tsnc_token *left, struct tsnc_token *right);
void tsnc_token_free(struct tsnc_token *token);

#endif
