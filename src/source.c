#include <assert.h>
#include <stdarg.h>
#include <tsnc/token.h>

int tsnc_source_memory_create(struct tsnc_source *dest,
    const char *source, int len) {
	FILE *fp;

  if (dest == NULL || source == NULL)
    return 0;

	if (len < 0)
		len = (int)strlen(source);

  // TODO: if the source is empty, this
  // returns NULL and it should be checked
	fp = fmemopen((void*)source, len, "r");
  
  tsnc_vector_create(&dest->tokenv);
  tsnc_vector_create(&dest->reportv);
  
  dest->fp = fp;
  dest->path = NULL;
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

int tsnc_source_compile(struct tsnc_source *source) {
  if (source == NULL)
    return 0;
  
  tsnc_tokenize_source(source);
  
  return 1;
}

int tsnc_source_cleanup(struct tsnc_source *source) {
  struct tsnc_token token;
  
  if (source == NULL)
    return 0;

  fclose(source->fp);
  
  // if the source kind is not file, we do not have path
  if (source->path != NULL)
    free(source->path);
  
  while (tsnc_vector_iter(&token, &source->tokenv,
        sizeof(struct tsnc_token))) {
    tsnc_token_cleanup(&token);
  }
  
  return 0;
}

void tsnc_source_report(struct tsnc_source *source,
    enum tsnc_report_kind kind, size_t startpos, size_t endpos,
    const char *message, ...) {
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

  tsnc_vector_push(&source->reportv,
      sizeof(struct tsnc_report), &report);
}
