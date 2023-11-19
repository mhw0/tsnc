#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <tsnc/compiler/tokenizer.h>
#include <tsnc/compiler/utf8.h>
#include <tsnc/compiler/unicode.h>

enum tsnc_token_kind tsnc_token_create(struct tsnc_token *token, struct tsnc_source *source,
    enum tsnc_token_kind kind, const char *str, size_t begpos, size_t endpos) {

  if (source != NULL)
    tsnc_source_getc(NULL, source);

  if (token == NULL)
    return kind;

  token->kind = kind;
  token->str = strdup(str);
  token->begpos = begpos;
  token->endpos = endpos;

  return kind;
}

enum tsnc_token_kind tsnc_tokenize_next(struct tsnc_token *dest, struct tsnc_source *source) {
  struct tsnc_utf8_charstr *charstr = &source->charstr;
  int32_t unicodep;
  size_t begpos;
  char tokbuf[2];

  begpos = charstr->charpos;
  tsnc_source_peek(&unicodep, source);

  switch (unicodep) {
    case EOF:
      return tsnc_token_create(dest, NULL, TokenKindEndOfFile, "<eof>", begpos, begpos);
    case UnicodeColon:
    case UnicodeComma:
    case UnicodeCommercialAt:
    case UnicodeLeftCurlyBracket:
    case UnicodeLeftParenthesis:
    case UnicodeLeftSquareBracket:
    case UnicodeNumberSign:
    case UnicodeRightCurlyBracket:
    case UnicodeRightParenthesis:
    case UnicodeRightSquareBracket:
    case UnicodeSemicolon:
    case UnicodeTilde:
      tokbuf[0] = unicodep;
      tokbuf[1] = '\0';
      return tsnc_token_create(dest, source, unicodep, tokbuf, begpos, begpos+1);
    case UnicodePlusSign:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodePlusSign)
        return tsnc_token_create(dest, source, TokenKindPlusPlus, "++", begpos, begpos+2);
      else if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindPlusEqual, "+=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindPlus, "+", begpos, begpos+1);
    case UnicodeMinusSign:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeMinusSign)
        return tsnc_token_create(dest, source, TokenKindMinusMinus, "--", begpos, begpos+2);
      else if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindMinusEqual, "-=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindMinus, "-", begpos, begpos+1);
    case UnicodeSolidus:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindSlashEqual, "/=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindSlash, "/", begpos, begpos+1);
    case UnicodeAsterisk:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeAsterisk) {
        tsnc_source_getc(NULL, source);

        tsnc_source_peek(&unicodep, source);
        if (unicodep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindAsteriskAsteriskEqual, "**=", begpos, begpos+3);

        return tsnc_token_create(dest, NULL, TokenKindAsteriskAsterisk, "**", begpos, begpos+2);
      } else if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindAsteriskEqual, "*=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindAsterisk, "*", begpos, begpos+1);
    case UnicodeEqualsSign:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeEqualsSign) {
        tsnc_source_getc(NULL, source);

        tsnc_source_peek(&unicodep, source);
        if (unicodep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindEqualEqualEqual, "===", begpos, begpos+3);

        return tsnc_token_create(dest, NULL, TokenKindEqualEqual, "==", begpos, begpos+2);
      }

      return tsnc_token_create(dest, NULL, TokenKindEqual, "=", begpos, begpos+1);
    case UnicodeExclamationMark:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeEqualsSign) {
        tsnc_source_getc(NULL, source);

        tsnc_source_peek(&unicodep, source);
        if (unicodep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindExclamationMarkEqualEqual, "!==", begpos, begpos+3);

        return tsnc_token_create(dest, NULL, TokenKindExclamationMarkEqual, "!=", begpos, begpos+2);
      }

      return tsnc_token_create(dest, NULL, TokenKindExclamationMark, "!", begpos, begpos+1);
    case UnicodeLessThanSign:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeLessThanSign) {
        tsnc_source_getc(NULL, source);

        tsnc_source_peek(&unicodep, source);
        if (unicodep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindLessThanLessThanEqual, "<<=", begpos, begpos+3);

        return tsnc_token_create(dest, source, TokenKindLessThanLessThan, "<<", begpos, begpos+2);
      }

      return tsnc_token_create(dest, NULL, TokenKindLessThan, "<", begpos, begpos+1);
    case UnicodeGreaterThanSign:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeGreaterThanSign) {
        tsnc_source_getc(NULL, source);

        tsnc_source_peek(&unicodep, source);
        if (unicodep == UnicodeGreaterThanSign) {
          tsnc_source_getc(NULL, source);

          tsnc_source_peek(&unicodep, source);
          if (unicodep == UnicodeEqualsSign)
            return tsnc_token_create(dest, source, TokenKindGreaterThanGreaterThanGreaterThanEqual, ">>>=", begpos, begpos+4);

          return tsnc_token_create(dest, NULL, TokenKindGreaterThanGreaterThanGreaterThan, ">>>", begpos, begpos+3);
        } else if (unicodep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindGreaterThanGreaterThanEqual, ">>=", begpos, begpos+3);

        return tsnc_token_create(dest, source, TokenKindGreaterThanGreaterThan, ">>", begpos, begpos+2);
      } else if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindGreaterThanEqual, ">=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindGreaterThan, ">", begpos, begpos+1);
    case UnicodeCircumflexAccent:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindCaretEqual, "^=", begpos, begpos + 2);

      return tsnc_token_create(dest, NULL, TokenKindCaret, "^", begpos, begpos + 1);
    case UnicodeAmpersand:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeAmpersand)
        return tsnc_token_create(dest, source, TokenKindAmpersandAmpersand, "&&", begpos, begpos + 2);
      else if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindAmpersandEqual, "&=", begpos, begpos + 2);

      return tsnc_token_create(dest, NULL, TokenKindAmpersand, "&", begpos, begpos + 1);
    case UnicodeVerticalLine:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeVerticalLine)
        return tsnc_token_create(dest, source, TokenKindVerticalBarVerticalBar, "||", begpos, begpos + 2);
      else if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindVerticalBarEqual, "|=", begpos, begpos + 2);

      return tsnc_token_create(dest, NULL, TokenKindVerticalBar, "|", begpos, begpos + 1);
    case UnicodePercentSign:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindPercentageEqual, "%=", begpos, begpos + 2);

      return tsnc_token_create(dest, NULL, TokenKindPercentage, "%", begpos, begpos + 1);
    case UnicodeQuestionMark:
      tsnc_source_getc(NULL, source);

      tsnc_source_peek(&unicodep, source);
      if (unicodep == UnicodeQuestionMark)
        return tsnc_token_create(dest, source, TokenKindQuestionMarkQuestionMark, "??", begpos, begpos + 2);

      return tsnc_token_create(dest, source, TokenKindQuestionMark, "?", begpos, begpos + 1);
    case UnicodeApostrophe:
    case UnicodeQuotationMark:
      assert(0 && "Not implemented yet");
    case UnicodeGraveAccent:
      assert(0 && "Not implemented yet");
    case UnicodeZero:
    case UnicodeOne:
    case UnicodeTwo:
    case UnicodeThree:
    case UnicodeFour:
    case UnicodeFive:
    case UnicodeSix:
    case UnicodeSeven:
    case UnicodeEight:
    case UnicodeNine:
      assert(0 && "Not implemented yet");
    /**
     * 11.2 White Space
     *
     * +--------+------------------------------------+
     * | U+0009 | CHARACTER TABULATION <TAB>         |
     * +--------+------------------------------------+
     * | U+000B | LINE TABULATION <VT>               |
     * +--------+------------------------------------+
     * | U+000C | FORM FEED (FF) <FF>                |
     * +--------+------------------------------------+
     * | U+0020 | SPACE <SP>                         |
     * +--------+------------------------------------+
     * | U+00A0 | NO-BREAK SPACE <NBSP>              |
     * +--------+------------------------------------+
     * | U+FEFF | ZERO WIDTH NO-BREAK SPACE <ZWNBSP> |
     * +--------+------------------------------------+
     * |  "Zs"  | "Separator, space" <USP>           |
     * +--------+------------------------------------+
     */
    case UnicodeEmQuad:
    case UnicodeEmSpace:
    case UnicodeEnQuad:
    case UnicodeEnSpace:
    case UnicodeFF:
    case UnicodeFigureSpace:
    case UnicodeFourPerEmSpace:
    case UnicodeHairSpace:
    case UnicodeIdeographicSpace:
    case UnicodeMMSP:
    case UnicodeNBSP:
    case UnicodeNNBSP:
    case UnicodeOghamSpaceMark:
    case UnicodePunctuationSpace:
    case UnicodeSP:
    case UnicodeSixPerEmSpace:
    case UnicodeTAB:
    case UnicodeThinSpace:
    case UnicodeThreePerEmSpace:
    case UnicodeVT:
    case UnicodeZWNBSP:
      tsnc_source_getc(NULL, source);
      return tsnc_tokenize_next(dest, source);
    /**
     * 11.3 Line Terminators
     *
     * +--------+--------------------------+
     * | U+000A | LINE FEED <LF>           |
     * +--------+--------------------------+
     * | U+000D | CARRIAGE RETURN <CR>     |
     * +--------+--------------------------+
     * | U+2028 | LINE SEPARATOR <LS>      |
     * +--------+--------------------------+
     * | U+2029 | PARAGRAPH SEPARATOR <PS> |
     * +--------+--------------------------+
     */
    case UnicodeLF:
    case UnicodeCR:
    case UnicodeLS:
    case UnicodePS:
      assert(0 && "Not implemented yet");
    default:
      assert(0 && "Not implemented yet");
  }

  return TokenKindInvalid;
}
