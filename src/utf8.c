#include <tsnc/utf8.h>
#include <tsnc/string.h>

/**
 * +-------------------+----------+----------+----------+---------+
 * | Code point        | Byte 1   | Byte 2   | Byte 3   | Byte 4  |
 * +-------------------+----------+----------+----------+---------+
 * | U+0000..U+007F    | 0xxxxxxx |          |          |         |
 * | U+0080..U+07FF    | 110xxxxx | 10xxxxxx |          |         |
 * | U+0800..U+FFFF    | 1110xxxx | 10xxxxxx | 10xxxxxx |         |
 * | U+10000..U+10FFFF | 11110xxx | 10xxxxxx | 10xxxxxx |10xxxxxx |
 * +-------------------+----------+----------+----------+---------+
 */

/* TODO(mhw0): this function is not complete */
int tsnc_utf8_peek(int32_t *dest, struct tsnc_utf8_charstr *str) {
  FILE *srcfp = str->srcfp;
  int32_t ch0, ch1, ch2, ch3;
  size_t respos = ftell(srcfp);

  ch0 = fgetc(srcfp);

  if (ch0 == EOF) {
    *dest = EOF;
    return EOF;
  }

  if (ch0 <= 0x7f) {
    fseek(srcfp, respos, SEEK_SET);
    if (dest) *dest = ch0;
    return 1;
  } else if (ch0 <= 0xdf) {
    ch1 = fgetc(srcfp);
    fseek(srcfp, respos, SEEK_SET);

    if (ch1 > 0xbf)
      return -2;

    if (dest)
      *dest = ((ch0 & 0x1f) << 0x06) | (ch1 & 0x3f);

    return 2;
  } else if (ch0 <= 0xef) {
    ch1 = fgetc(srcfp);
    ch2 = fgetc(srcfp);
    fseek(srcfp, respos, SEEK_SET);

    if (ch1 == EOF || ch2 == EOF)
      return EOF;

    if (ch1 > 0xbf || ch2 > 0xbf)
      return -2;

    if (dest)
      *dest = ((ch0 & 0x0f) << 0x0c) | ((ch1 & 0x3f) << 0x06)
        | (ch2 & 0x3f);

    return 3;
  } else if (ch0 <= 0xf7) {
    ch1 = fgetc(srcfp);
    ch2 = fgetc(srcfp);
    ch3 = fgetc(srcfp);
    fseek(srcfp, respos, SEEK_SET);

    if (ch1 == EOF || ch2 == EOF || ch3 == EOF)
      return EOF;

    if (ch1 > 0xbf || ch2 > 0xbf || ch3 > 0xbf)
      return -2;

    if (dest)
     *dest = ((ch0 & 0x07) << 0x12) | ((ch1 & 0x3f) << 0x0c)
      | ((ch2 & 0x3f) << 0x06) | (ch3 & 0x3f);
    return 4;
  }

  return -2;
}

int tsnc_utf8_getc(int32_t *dest, struct tsnc_utf8_charstr *str) {
  FILE *srcfp = str->srcfp;
  int bytes = tsnc_utf8_peek(dest, str);

  if (bytes < 0)
    return -1;

  str->bytepos += bytes;
  str->charpos++;
  fseek(srcfp, bytes, SEEK_CUR);
  return bytes;
}

void tsnc_utf8_seek(struct tsnc_utf8_charstr *str, size_t bytepos, size_t charpos) {
  fseek(str->srcfp, bytepos, SEEK_SET);
  str->bytepos = bytepos;
  str->charpos = charpos;
}

size_t tsnc_utf8_rread(char *dest, struct tsnc_utf8_charstr *str, size_t count) {
  return fread(dest, sizeof(char), count, str->srcfp);
}

void tsnc_utf8_free(struct tsnc_utf8_charstr *str) {
  fclose(str->srcfp);
  str->charpos = 0;
  str->bytepos = 0;
}
