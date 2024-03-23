#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <tsnc/tokenizer.h>
#include <tsnc/utf8.h>
#include <tsnc/unicode.h>

enum tsnc_token_kind tsnc_token_create(struct tsnc_token *token, struct tsnc_source *source,
    enum tsnc_token_kind kind, const char *str, size_t begpos, size_t endpos) {

  if (source != NULL)
    tsnc_stru8_getc(NULL, &source->str);

  if (token == NULL)
    return kind;

  token->kind = kind;
  token->str = strdup(str);
  token->begpos = begpos;
  token->endpos = endpos;

  return kind;
}

enum tsnc_token_kind tsnc_tokenize_next(struct tsnc_token *dest, struct tsnc_source *source) {
  int32_t codep;
  size_t begpos;
  char tokbuf[2];

  begpos = TSNC_STR_CHARPOS(source->str);
  tsnc_stru8_peek(&codep, &source->str);

  switch (codep) {
    case EOF:
      return tsnc_token_create(dest, NULL, TokenKindEndOfFile, "<EOF>", begpos, begpos);
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
      tokbuf[0] = codep;
      tokbuf[1] = '\0';
      return tsnc_token_create(dest, source, codep, tokbuf, begpos, begpos+1);
    case UnicodePlusSign:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodePlusSign)
        return tsnc_token_create(dest, source, TokenKindPlusPlus, "++", begpos, begpos+2);
      else if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindPlusEqual, "+=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindPlus, "+", begpos, begpos+1);
    case UnicodeMinusSign:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeMinusSign)
        return tsnc_token_create(dest, source, TokenKindMinusMinus, "--", begpos, begpos+2);
      else if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindMinusEqual, "-=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindMinus, "-", begpos, begpos+1);
    case UnicodeSolidus:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindSlashEqual, "/=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindSlash, "/", begpos, begpos+1);
    case UnicodeAsterisk:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeAsterisk) {
        tsnc_stru8_getc(NULL, &source->str);

        tsnc_stru8_peek(&codep, &source->str);
        if (codep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindAsteriskAsteriskEqual, "**=", begpos, begpos+3);

        return tsnc_token_create(dest, NULL, TokenKindAsteriskAsterisk, "**", begpos, begpos+2);
      } else if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindAsteriskEqual, "*=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindAsterisk, "*", begpos, begpos+1);
    case UnicodeEqualsSign:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeEqualsSign) {
        tsnc_stru8_getc(NULL, &source->str);

        tsnc_stru8_peek(&codep, &source->str);
        if (codep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindEqualEqualEqual, "===", begpos, begpos+3);

        return tsnc_token_create(dest, NULL, TokenKindEqualEqual, "==", begpos, begpos+2);
      }

      return tsnc_token_create(dest, NULL, TokenKindEqual, "=", begpos, begpos+1);
    case UnicodeExclamationMark:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeEqualsSign) {
        tsnc_stru8_getc(NULL, &source->str);

        tsnc_stru8_peek(&codep, &source->str);
        if (codep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindExclamationMarkEqualEqual, "!==", begpos, begpos+3);

        return tsnc_token_create(dest, NULL, TokenKindExclamationMarkEqual, "!=", begpos, begpos+2);
      }

      return tsnc_token_create(dest, NULL, TokenKindExclamationMark, "!", begpos, begpos+1);
    case UnicodeLessThanSign:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeLessThanSign) {
        tsnc_stru8_getc(NULL, &source->str);

        tsnc_stru8_peek(&codep, &source->str);
        if (codep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindLessThanLessThanEqual, "<<=", begpos, begpos+3);

        return tsnc_token_create(dest, source, TokenKindLessThanLessThan, "<<", begpos, begpos+2);
      }

      return tsnc_token_create(dest, NULL, TokenKindLessThan, "<", begpos, begpos+1);
    case UnicodeGreaterThanSign:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeGreaterThanSign) {
        tsnc_stru8_getc(NULL, &source->str);

        tsnc_stru8_peek(&codep, &source->str);
        if (codep == UnicodeGreaterThanSign) {
          tsnc_stru8_getc(NULL, &source->str);

          tsnc_stru8_peek(&codep, &source->str);
          if (codep == UnicodeEqualsSign)
            return tsnc_token_create(dest, source, TokenKindGreaterThanGreaterThanGreaterThanEqual, ">>>=", begpos, begpos+4);

          return tsnc_token_create(dest, NULL, TokenKindGreaterThanGreaterThanGreaterThan, ">>>", begpos, begpos+3);
        } else if (codep == UnicodeEqualsSign)
          return tsnc_token_create(dest, source, TokenKindGreaterThanGreaterThanEqual, ">>=", begpos, begpos+3);

        return tsnc_token_create(dest, source, TokenKindGreaterThanGreaterThan, ">>", begpos, begpos+2);
      } else if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindGreaterThanEqual, ">=", begpos, begpos+2);

      return tsnc_token_create(dest, NULL, TokenKindGreaterThan, ">", begpos, begpos+1);
    case UnicodeCircumflexAccent:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindCaretEqual, "^=", begpos, begpos + 2);

      return tsnc_token_create(dest, NULL, TokenKindCaret, "^", begpos, begpos + 1);
    case UnicodeAmpersand:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeAmpersand)
        return tsnc_token_create(dest, source, TokenKindAmpersandAmpersand, "&&", begpos, begpos + 2);
      else if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindAmpersandEqual, "&=", begpos, begpos + 2);

      return tsnc_token_create(dest, NULL, TokenKindAmpersand, "&", begpos, begpos + 1);
    case UnicodeVerticalLine:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeVerticalLine)
        return tsnc_token_create(dest, source, TokenKindVerticalBarVerticalBar, "||", begpos, begpos + 2);
      else if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindVerticalBarEqual, "|=", begpos, begpos + 2);

      return tsnc_token_create(dest, NULL, TokenKindVerticalBar, "|", begpos, begpos + 1);
    case UnicodePercentSign:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeEqualsSign)
        return tsnc_token_create(dest, source, TokenKindPercentageEqual, "%=", begpos, begpos + 2);

      return tsnc_token_create(dest, NULL, TokenKindPercentage, "%", begpos, begpos + 1);
    case UnicodeQuestionMark:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeQuestionMark)
        return tsnc_token_create(dest, source, TokenKindQuestionMarkQuestionMark, "??", begpos, begpos + 2);

      return tsnc_token_create(dest, source, TokenKindQuestionMark, "?", begpos, begpos + 1);
    case UnicodeApostrophe:
    case UnicodeQuotationMark:
      assert(0 && "Not implemented yet");
    case UnicodeGraveAccent:
      assert(0 && "Not implemented yet");
    case UnicodeZero:
      assert(0 && "Not implemented yet");
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
     * WhiteSpace ::
     *   <TAB>
     *   <VT>
     *   <FF>
     *   <SP>
     *   <NBSP>
     *   <BOM>
     *   <USP>
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
      tsnc_stru8_getc(NULL, &source->str);
      return tsnc_tokenize_next(dest, source);
    /**
     * 11.3 Line Terminators
     *
     * LineTerminator ::
     *  <LF>
     *  <CR>
     *  <LS>
     *  <PS>
     *
     * LineTerminatorSequence ::
     *  <LF>
     *  <CR> [lookahead ∉ <LF> ]
     *  <LS>
     *  <PS>
     *  <CR> <LF>
     */
    case UnicodeLF:
      return tsnc_token_create(dest, source, TokenKindLineTerminator, "<LF>", begpos, begpos + 1);
    case UnicodeCR:
      tsnc_stru8_getc(NULL, &source->str);

      tsnc_stru8_peek(&codep, &source->str);
      if (codep == UnicodeLF)
        return tsnc_token_create(dest, source, TokenKindLineTerminator, "<CR><LF>", begpos, begpos + 1);

      return tsnc_token_create(dest, NULL, TokenKindLineTerminator, "<CR>", begpos, begpos + 1);

    case UnicodeLS:
      return tsnc_token_create(dest, source, TokenKindLineTerminator, "<LS>", begpos, begpos + 1);
    case UnicodePS:
      return tsnc_token_create(dest, source, TokenKindLineTerminator, "<PS>", begpos, begpos + 1);
    default:
      assert(0 && "Not implemented yet");
  }

  return TokenKindInvalid;
}
