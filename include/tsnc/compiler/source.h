#ifndef TSNC_SOURCE_H
#define TSNC_SOURCE_H
#include <stdio.h>

/* holds source fields */
struct tsnc_source {
  /* source path */
  char *path;
  /* file pointer */
  FILE *fp;
  /* holds root token */
  struct tsnc_token *token;
  /* holds current token */
  struct tsnc_token *currtoken;
  /* utf-8 char position */
  size_t charpos;
};

int tsnc_source_memory_create(struct tsnc_source *dest,
    const char *source, int len);
int tsnc_source_file_create(struct tsnc_source *dest,
    const char *path);

int32_t tsnc_source_peek(struct tsnc_source *source);
int32_t tsnc_source_ahead(struct tsnc_source *source);
int32_t tsnc_source_consume(struct tsnc_source *source);

void tsnc_source_free(struct tsnc_source *source);
#endif
