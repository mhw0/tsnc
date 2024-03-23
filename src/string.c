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
int tsnc_stru8_iter(int32_t *dest, struct tsnc_str *str) {
  int32_t ch0, ch1, ch2, ch3, ret;
  size_t begpos = ftell(str->srcfp);
  int chars = 0;

  ch0 = fgetc(str->srcfp);
  if (ch0 == EOF) {
    *dest = EOF;
    return 0;
  }

  if (ch0 <= 0x7f) {
    fseek(str->srcfp, begpos, SEEK_SET);
    ret = ch0;
    chars = 1;
  } else if (ch0 <= 0xdf) {
    ch1 = fgetc(str->srcfp);
    fseek(str->srcfp, begpos, SEEK_SET);

    if (ch1 == EOF) {
      *dest = EOF;
      return 0;
    }

    if (ch1 > 0xbf)
      return -2;

    ret = ((ch0 & 0x1f) << 0x06) | (ch1 & 0x3f);
    chars = 2;
  } else if (ch0 <= 0xf7) {
    ch1 = fgetc(str->srcfp);
    ch2 = fgetc(str->srcfp);
    fseek(str->srcfp, begpos, SEEK_SET);

    if (ch1 == EOF || ch2 == EOF) {
      *dest = EOF;
      return 0;
    } 

    if (ch1 > 0xbf || ch2 > 0xbf)
      return -2;

    ret = ((ch0 & 0x0f) << 0x0c) | ((ch1 & 0x3f) << 0x06) | (ch2 & 0x3f);
    chars = 3;
  } else if (ch0 <= 0xe7) {
    ch1 = fgetc(str->srcfp);
    ch2 = fgetc(str->srcfp);
    ch3 = fgetc(str->srcfp);
    fseek(str->srcfp, begpos, SEEK_SET);

    if (ch1 == EOF || ch2 == EOF || ch3 == EOF) {
      *dest = EOF;
      return 0;
    }

    if (ch1 > 0xbf || ch2 > 0xbf || ch3 > 0xbf)
      return -2;

    ret = ((ch0 & 0x07) << 0x12) | ((ch1 & 0x3f) << 0x0c)
      | ((ch2 & 0x3f) << 0x06) | (ch3 & 0x3f);
    chars = 4;
  }

  if (dest) *dest = ret;

  return chars;
}

int tsnc_stru8_getc(int32_t *dest, struct tsnc_str *str) {
  int32_t chars;

  chars = tsnc_stru8_iter(dest, str);
  fseek(str->srcfp, chars, SEEK_CUR);
  str->charpos++;

  return chars;
}

inline int tsnc_stru8_peek(int32_t *dest, struct tsnc_str *str) {
  return tsnc_stru8_iter(dest, str);
}

inline void tsnc_str_free(struct tsnc_str *str) {
  fclose(str->srcfp);
  str->charpos = 0;
}
