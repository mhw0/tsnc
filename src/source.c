#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <tsnc/source.h>
#include <tsnc/token-stream.h>

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
  
  tsnc_token_stream_init(&dest->tokens);
  tsnc_vector_init(&dest->reportv);
  
  dest->fp = fp;
  dest->path = NULL;
  dest->node = NULL;
  dest->currnode = NULL;
  return 1;
}

int tsnc_source_file_create(struct tsnc_source *dest,
    const char *path) {
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

  dest->fp = fp;
  return 1;
}

void tsnc_source_free(struct tsnc_source *source) {
  if (source == NULL)
    return;

  fclose(source->fp);
  
  // if the source kind is not file, we do not have path
  if (source->path != NULL)
    free(source->path);

  tsnc_token_stream_free(&source->tokens);
}

void tsnc_source_report(struct tsnc_source *source, enum tsnc_report_kind kind,
    size_t startpos, size_t endpos, const char *message, ...) {
  struct tsnc_report report;
  char *fmtmsg;
  va_list args;

  va_start(args, message);
  vasprintf(&fmtmsg, message, args);
  va_end(args);

  report.kind = kind;
  report.startpos = startpos;
  report.endpos = endpos;
  report.message = fmtmsg;

  tsnc_vector_push(&source->reportv, sizeof(struct tsnc_report), &report);
}
