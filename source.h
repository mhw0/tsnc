#ifndef TSNC_SOURCE_H
#define TSNC_SOURCE_H
#include <stdio.h>
#include "token.h"

struct tsnc_source {
  char *path;
  FILE *fp;
  struct tsnc_vector *tokenv;
};

int tsnc_source_from_path(struct tsnc_source **dest, const char *path);
int tsnc_source_cleanup(struct tsnc_source *source);

#endif
