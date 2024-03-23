#ifndef TSNC_SOURCE_H
#define TSNC_SOURCE_H
#include <stdlib.h>
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
void tsnc_source_free(struct tsnc_source *src);

#endif
