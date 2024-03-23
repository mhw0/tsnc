#ifndef TSNC_SOURCE_H
#define TSNC_SOURCE_H
#include <stdio.h>
#include <tsnc/utf8.h>
#include <tsnc/string.h>

/* holds source fields */
struct tsnc_source {
  /* source path */
  char *path;
  /* character stream */
  struct tsnc_str str;
};

int tsnc_source_memory_create(struct tsnc_source *dest,
    const char *source, int len);
int tsnc_source_file_create(struct tsnc_source *dest,
    const char *path);
int tsnc_source_peek(int32_t *dest, struct tsnc_source *src);
int tsnc_source_getc(int32_t *dest, struct tsnc_source *src);
void tsnc_source_seek(struct tsnc_source *src, size_t bytepos, size_t charpos);
size_t tsnc_source_rread(char *dest, struct tsnc_source *src, size_t count);

void tsnc_source_free(struct tsnc_source *src);
#endif
