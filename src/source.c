#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <tsnc/source.h>

int tsnc_source_memory_create(struct tsnc_source *dest, const char *source,
    int len) {
	FILE *fp;

  if (dest == NULL || source == NULL)
    return 0;

	if (len < 0)
		len = (int)strlen(source);

  // TODO: if the source is empty, this
  // returns NULL and it should be checked
	fp = fmemopen((void*)source, len, "r");

  dest->str.srcfp = fp;
  dest->str.charpos = 0;

  dest->path = NULL;
  return 1;
}

int tsnc_source_file_create(struct tsnc_source *dest, const char *path) {
  FILE *fp;
  size_t len;
  char *pathbuf;

  if (dest == NULL || path == NULL)
    return 0;
  
  len = strlen(path);

  fp = fopen(path, "r");
  if (fp == NULL)
    return 0;
  
  pathbuf = (char*)malloc(len + 1);
  assert(pathbuf && "Unable to allocate memory for path");
  
  memcpy(pathbuf, path, len);
  pathbuf[len] = '\0';

  dest->str.srcfp = fp;
  dest->str.charpos = 0;
  return 1;
}

inline void tsnc_source_free(struct tsnc_source *source) {
  tsnc_str_free(&source->str);
  free(source->path);
}
