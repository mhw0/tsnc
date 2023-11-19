#ifndef TSNC_UTF8_H
#define TSNC_UTF8_H
#include <stdlib.h>
#include <stdio.h>

struct tsnc_utf8_charstr {
  /* holds file pointer */
  FILE *srcfp;
  /* holds byte position */
  size_t bytepos;
  /* holds char position */
  size_t charpos;
};

int tsnc_utf8_getc(int32_t *dest, struct tsnc_utf8_charstr *str);
int tsnc_utf8_peek(int32_t *dest, struct tsnc_utf8_charstr *str);
void tsnc_utf8_seek(struct tsnc_utf8_charstr *str, size_t bytepos,
    size_t charpos);
void tsnc_utf8_free(struct tsnc_utf8_charstr *str);

#endif
