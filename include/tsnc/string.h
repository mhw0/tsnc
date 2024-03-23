#ifndef TSNC_STRING_H
#define TSNC_STRING_H
#include <stdio.h>

#define TSNC_STR_CHARPOS(str) str.charpos

struct tsnc_str {
  FILE *srcfp;
  size_t charpos;
};

/* UTF-8 specific functions */
int tsnc_stru8_iter(int32_t *dest, struct tsnc_str *str);
int tsnc_stru8_getc(int32_t *dest, struct tsnc_str *str);
int tsnc_stru8_peek(int32_t *dest, struct tsnc_str *str);
int tsnc_stru8_read(int32_t *dest, struct tsnc_str *str, size_t count);

int tsnc_str_getpos(int32_t *dest, struct tsnc_str *str);
void tsnc_str_free(struct tsnc_str *str);
#endif
