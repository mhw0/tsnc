#include <tsnc/utf8.h>

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
int tsnc_utf8_getch(FILE *srcfp) {
  int32_t ch0, ch1, ch2, ch3;

  if (srcfp == NULL)
    return -2;

  ch0 = fgetc(srcfp);

  if (ch0 == EOF)
    return EOF;

  if (ch0 <= 0x7f) {
    return ch0;
  } else if (ch0 <= 0xdf) {
    ch1 = fgetc(srcfp);

    if (ch1 > 0xbf)
      return -2;

    return ((ch0 & 0x1f) << 0x06) | (ch1 & 0x3f);
  } else if (ch0 <= 0xef) {
    ch1 = fgetc(srcfp);
    ch2 = fgetc(srcfp);

    if (ch1 == EOF || ch2 == EOF)
      return EOF;

    if (ch1 > 0xbf || ch2 > 0xbf)
      return -2;

    return ((ch0 & 0x0f) << 0x0c) | ((ch1 & 0x3f) << 0x06)
      | (ch2 & 0x3f);
  } else if (ch0 <= 0xf7) {
    ch1 = fgetc(srcfp);
    ch2 = fgetc(srcfp);
    ch3 = fgetc(srcfp);

    if (ch1 == EOF || ch2 == EOF || ch3 == EOF)
      return EOF;

    if (ch1 > 0xbf || ch2 > 0xbf || ch3 > 0xbf)
      return -2;

    return ((ch0 & 0x07) << 0x12) | ((ch1 & 0x3f) << 0x0c)
      | ((ch2 & 0x3f) << 0x06) | (ch3 & 0x3f);
  }

  return -2;
}

int tsnc_utf8_ungetch(int ch, FILE *srcfp) {
  if (srcfp == NULL || ch <= 0)
    return -1;

  if (ch <= 0x7f)
    fseek(srcfp, -1, SEEK_CUR);
  else if (ch <= 0x7ff)
    fseek(srcfp, -2, SEEK_CUR);
  else if (ch <= 0xffff)
    fseek(srcfp, -3, SEEK_CUR);
  else if (ch <= 0x1fffff)
    fseek(srcfp, -4, SEEK_CUR);

  return 1;
}
