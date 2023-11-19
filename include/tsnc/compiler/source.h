#ifndef TSNC_SOURCE_H
#define TSNC_SOURCE_H
#include <stdio.h>
#include <tsnc/compiler/utf8.h>

/* holds source fields */
struct tsnc_source {
  /* source path */
  char *path;
  /* utf8 character stream */
  struct tsnc_utf8_charstr charstr;
};

int tsnc_source_memory_create(struct tsnc_source *dest,
    const char *source, int len);
int tsnc_source_file_create(struct tsnc_source *dest,
    const char *path);

int tsnc_source_peek(int32_t *dest, struct tsnc_source *source);
int tsnc_source_getc(int32_t *dest, struct tsnc_source *source);

void tsnc_source_free(struct tsnc_source *source);
#endif
