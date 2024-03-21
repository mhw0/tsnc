#include <tap.h>
#include <tsnc/compiler/tokenizer.h>
#include <tsnc/compiler/source.h>
#include "test-tokenizer.h"

void tsnc_test_tokenizer_single_character_tokens() {
  struct tsnc_source source;
  struct tsnc_token token, exptoken;

  tsnc_source_memory_create(&source, "#@(){}[],:;", -1);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindHashtag;
  exptoken.begpos = 0; exptoken.endpos = 1; exptoken.str = "#";
  ok(tsnc_token_equal(&token, &exptoken), "token: #");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindAsperand;
  exptoken.begpos = 1; exptoken.endpos = 2; exptoken.str = "@";
  ok(tsnc_token_equal(&token, &exptoken), "token: @");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindRoundBracketOpen;
  exptoken.begpos = 2; exptoken.endpos = 3; exptoken.str = "(";
  ok(tsnc_token_equal(&token, &exptoken), "token: (");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindRoundBracketClose;
  exptoken.begpos = 3; exptoken.endpos = 4; exptoken.str = ")";
  ok(tsnc_token_equal(&token, &exptoken), "token: )");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindCurlyBracketOpen;
  exptoken.begpos = 4; exptoken.endpos = 5; exptoken.str = "{";
  ok(tsnc_token_equal(&token, &exptoken), "token: {");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindCurlyBracketClose;
  exptoken.begpos = 5; exptoken.endpos = 6; exptoken.str = "}";
  ok(tsnc_token_equal(&token, &exptoken), "token: }");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindSquareBracketOpen;
  exptoken.begpos = 6; exptoken.endpos = 7; exptoken.str = "[";
  ok(tsnc_token_equal(&token, &exptoken), "token: [");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindSquareBracketClose;
  exptoken.begpos = 7; exptoken.endpos = 8; exptoken.str = "]";
  ok(tsnc_token_equal(&token, &exptoken), "token: ]");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindComma;
  exptoken.begpos = 8; exptoken.endpos = 9; exptoken.str = ",";
  ok(tsnc_token_equal(&token, &exptoken), "token: ,");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindColon;
  exptoken.begpos = 9; exptoken.endpos = 10; exptoken.str = ":";
  ok(tsnc_token_equal(&token, &exptoken), "token: :");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindSemicolon;
  exptoken.begpos = 10; exptoken.endpos = 11; exptoken.str = ";";
  ok(tsnc_token_equal(&token, &exptoken), "token: ;");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindEndOfFile;
  exptoken.begpos = 11; exptoken.endpos = 11; exptoken.str = "<EOF>";
  ok(tsnc_token_equal(&token, &exptoken), "token: <EOF>");
  tsnc_token_free(&token);

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_punctuators() {
  struct tsnc_source source;
  struct tsnc_token token, exptoken;

  tsnc_source_memory_create(&source,
      "+ ++ += - -- -= / /= * ** *= **= = == === "
      "! != !== < << <<= > >> >= >>> >>= >>>= "
      "^ ^= & && &= | || |= % %= ? ??", -1);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindPlus;
  exptoken.begpos = 0; exptoken.endpos = 1; exptoken.str = "+";
  ok(tsnc_token_equal(&token, &exptoken), "token: +");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindPlusPlus;
  exptoken.begpos = 2; exptoken.endpos = 4; exptoken.str = "++";
  ok(tsnc_token_equal(&token, &exptoken), "token: ++");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindPlusEqual;
  exptoken.begpos = 5; exptoken.endpos = 7; exptoken.str = "+=";
  ok(tsnc_token_equal(&token, &exptoken), "token: +=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindMinus;
  exptoken.begpos = 8; exptoken.endpos = 9; exptoken.str = "-";
  ok(tsnc_token_equal(&token, &exptoken), "token: -");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindMinusMinus;
  exptoken.begpos = 10; exptoken.endpos = 12; exptoken.str = "--";
  ok(tsnc_token_equal(&token, &exptoken), "token: --");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindMinusEqual;
  exptoken.begpos = 13; exptoken.endpos = 15; exptoken.str = "-=";
  ok(tsnc_token_equal(&token, &exptoken), "token: -=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindSlash;
  exptoken.begpos = 16; exptoken.endpos = 17; exptoken.str = "/";
  ok(tsnc_token_equal(&token, &exptoken), "token: /");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindSlashEqual;
  exptoken.begpos = 18; exptoken.endpos = 20; exptoken.str = "/=";
  ok(tsnc_token_equal(&token, &exptoken), "token: /=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindAsterisk;
  exptoken.begpos = 21; exptoken.endpos = 22; exptoken.str = "*";
  ok(tsnc_token_equal(&token, &exptoken), "token: *");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindAsteriskAsterisk;
  exptoken.begpos = 23; exptoken.endpos = 25; exptoken.str = "**";
  ok(tsnc_token_equal(&token, &exptoken), "token: **");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindAsteriskEqual;
  exptoken.begpos = 26; exptoken.endpos = 28; exptoken.str = "*=";
  ok(tsnc_token_equal(&token, &exptoken), "token: *=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindAsteriskAsteriskEqual;
  exptoken.begpos = 29; exptoken.endpos = 32; exptoken.str = "**=";
  ok(tsnc_token_equal(&token, &exptoken), "token: **=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindEqual;
  exptoken.begpos = 33; exptoken.endpos = 34; exptoken.str = "=";
  ok(tsnc_token_equal(&token, &exptoken), "token: =");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindEqualEqual;
  exptoken.begpos = 35; exptoken.endpos = 37; exptoken.str = "==";
  ok(tsnc_token_equal(&token, &exptoken), "token: ==");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindEqualEqualEqual;
  exptoken.begpos = 38; exptoken.endpos = 41; exptoken.str = "===";
  ok(tsnc_token_equal(&token, &exptoken), "token: ===");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindExclamationMark;
  exptoken.begpos = 42; exptoken.endpos = 43; exptoken.str = "!";
  ok(tsnc_token_equal(&token, &exptoken), "token: !");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindExclamationMarkEqual;
  exptoken.begpos = 44; exptoken.endpos = 46; exptoken.str = "!=";
  ok(tsnc_token_equal(&token, &exptoken), "token: !=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindExclamationMarkEqualEqual;
  exptoken.begpos = 47; exptoken.endpos = 50; exptoken.str = "!==";
  ok(tsnc_token_equal(&token, &exptoken), "token: !==");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindLessThan;
  exptoken.begpos = 51; exptoken.endpos = 52; exptoken.str = "<";
  ok(tsnc_token_equal(&token, &exptoken), "token: <");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindLessThanLessThan;
  exptoken.begpos = 53; exptoken.endpos = 55; exptoken.str = "<<";
  ok(tsnc_token_equal(&token, &exptoken), "token: <<");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindLessThanLessThanEqual;
  exptoken.begpos = 56; exptoken.endpos = 59; exptoken.str = "<<=";
  ok(tsnc_token_equal(&token, &exptoken), "token: <<=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindGreaterThan;
  exptoken.begpos = 60; exptoken.endpos = 61; exptoken.str = ">";
  ok(tsnc_token_equal(&token, &exptoken), "token: >");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindGreaterThanGreaterThan;
  exptoken.begpos = 62; exptoken.endpos = 64; exptoken.str = ">>";
  ok(tsnc_token_equal(&token, &exptoken), "token: >>");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindGreaterThanEqual;
  exptoken.begpos = 65; exptoken.endpos = 67; exptoken.str = ">=";
  ok(tsnc_token_equal(&token, &exptoken), "token: >=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindGreaterThanGreaterThanGreaterThan;
  exptoken.begpos = 68; exptoken.endpos = 71; exptoken.str = ">>>";
  ok(tsnc_token_equal(&token, &exptoken), "token: >>>");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindGreaterThanGreaterThanEqual;
  exptoken.begpos = 72; exptoken.endpos = 75; exptoken.str = ">>=";
  ok(tsnc_token_equal(&token, &exptoken), "token: >>=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindGreaterThanGreaterThanGreaterThanEqual;
  exptoken.begpos = 76; exptoken.endpos = 80; exptoken.str = ">>>=";
  ok(tsnc_token_equal(&token, &exptoken), "token: >>>=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindCaret;
  exptoken.begpos = 81; exptoken.endpos = 82; exptoken.str = "^";
  ok(tsnc_token_equal(&token, &exptoken), "token: ^");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindCaretEqual;
  exptoken.begpos = 83; exptoken.endpos = 85; exptoken.str = "^=";
  ok(tsnc_token_equal(&token, &exptoken), "token: ^=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindAmpersand;
  exptoken.begpos = 86; exptoken.endpos = 87; exptoken.str = "&";
  ok(tsnc_token_equal(&token, &exptoken), "token: &");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindAmpersandAmpersand;
  exptoken.begpos = 88; exptoken.endpos = 90; exptoken.str = "&&";
  ok(tsnc_token_equal(&token, &exptoken), "token: &&");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindAmpersandEqual;
  exptoken.begpos = 91; exptoken.endpos = 93; exptoken.str = "&=";
  ok(tsnc_token_equal(&token, &exptoken), "token: &=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindVerticalBar;
  exptoken.begpos = 94; exptoken.endpos = 95; exptoken.str = "|";
  ok(tsnc_token_equal(&token, &exptoken), "token: |");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindVerticalBarVerticalBar;
  exptoken.begpos = 96; exptoken.endpos = 98; exptoken.str = "||";
  ok(tsnc_token_equal(&token, &exptoken), "token: ||");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindVerticalBarEqual;
  exptoken.begpos = 99; exptoken.endpos = 101; exptoken.str = "|=";
  ok(tsnc_token_equal(&token, &exptoken), "token: |=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindPercentage;
  exptoken.begpos = 102; exptoken.endpos = 103; exptoken.str = "%";
  ok(tsnc_token_equal(&token, &exptoken), "token: %%");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindPercentageEqual;
  exptoken.begpos = 104; exptoken.endpos = 106; exptoken.str = "%=";
  ok(tsnc_token_equal(&token, &exptoken), "token: %%=");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindQuestionMark;
  exptoken.begpos = 107; exptoken.endpos = 108; exptoken.str = "?";
  ok(tsnc_token_equal(&token, &exptoken), "token: ?");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindQuestionMarkQuestionMark;
  exptoken.begpos = 109; exptoken.endpos = 111; exptoken.str = "??";
  ok(tsnc_token_equal(&token, &exptoken), "token: ??");
  tsnc_token_free(&token);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindEndOfFile;
  exptoken.begpos = 111; exptoken.endpos = 111; exptoken.str = "<EOF>";
  ok(tsnc_token_equal(&token, &exptoken), "token: <EOF>");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_line_terminators() {
  struct tsnc_source source;
  struct tsnc_token token, exptoken;

  // UnicodeLF = 0x000A,
  // UnicodeCR = 0x000D,
  // UnicodeLS = 0x2028,
  // UnicodePS = 0x2029,

  tsnc_source_memory_create(&source, "\x0A\x0D\xE2\x80\xA8\xE2\x80\xA8\x0D\x0A", -1);

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindLineTerminator;
  exptoken.begpos = 0; exptoken.endpos = 1; exptoken.str = "";
  ok(tsnc_token_equal(&token, &exptoken), "token: <LF>");

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindLineTerminator;
  exptoken.begpos = 1; exptoken.endpos = 2; exptoken.str = "";
  ok(tsnc_token_equal(&token, &exptoken), "token: <CR>");

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindLineTerminator;
  exptoken.begpos = 2; exptoken.endpos = 3; exptoken.str = "";
  ok(tsnc_token_equal(&token, &exptoken), "token: <LS>");

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindLineTerminator;
  exptoken.begpos = 3; exptoken.endpos = 4; exptoken.str = "";
  ok(tsnc_token_equal(&token, &exptoken), "token: <PS>");

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindLineTerminator;
  exptoken.begpos = 4; exptoken.endpos = 5; exptoken.str = "";
  ok(tsnc_token_equal(&token, &exptoken), "token: <CR><LF>");

  tsnc_tokenize_next(&token, &source);
  exptoken.kind = TokenKindEndOfFile;
  /**
   * 7.3 Line Terminators: ""The character sequence <CR><LF> is commonly used as a line terminator.
   * It should be considered a single character for the purpose of reporting line numbers"
   *
   * TODO(mhw0): begpos and endpos of EOF should be 5 not 6
   */
  exptoken.begpos = 6; exptoken.endpos = 6; exptoken.str = "<EOF>";
  ok(tsnc_token_equal(&token, &exptoken), "token: <EOF>");

  tsnc_source_free(&source);
}
