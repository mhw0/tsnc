#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <tsnc/compiler/source.h>
#include <tsnc/compiler/utf8.h>

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

  dest->charstr.srcfp = fp;
  dest->charstr.bytepos = 0;
  dest->charstr.charpos = 0;

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

  dest->charstr.srcfp = fp;
  dest->charstr.bytepos = 0;
  dest->charstr.charpos = 0;
  return 1;
}

int tsnc_source_peek(int32_t *dest, struct tsnc_source *src) {
  return tsnc_utf8_peek(dest, &src->charstr);
}

int tsnc_source_getc(int32_t *dest, struct tsnc_source *src) {
  return tsnc_utf8_getc(dest, &src->charstr);
}

void tsnc_source_seek(struct tsnc_source *src, size_t bytepos, size_t charpos) {
  tsnc_utf8_seek(&src->charstr, bytepos, charpos);
}

size_t tsnc_source_rread(char *dest, struct tsnc_source *src, size_t count) {
  return tsnc_utf8_rread(dest, &src->charstr, count);
}

void tsnc_source_free(struct tsnc_source *source) {
  if (source == NULL)
    return;

  tsnc_utf8_free(&source->charstr);
  
  if (source->path != NULL)
    free(source->path);
}
