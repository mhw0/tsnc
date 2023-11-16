#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
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
  
  dest->fp = fp;
  dest->path = NULL;
  dest->charpos = 0;
  return 1;
}

int32_t tsnc_source_peek(struct tsnc_source *source) {
  wint_t ch = fgetwc(source->fp);
  ungetwc(ch, source->fp);
  return ch;
}

int32_t tsnc_source_consume(struct tsnc_source *source) {
  source->charpos++;
  return tsnc_utf8_getch(source->fp);
}

void tsnc_source_free(struct tsnc_source *source) {
  if (source == NULL)
    return;

  fclose(source->fp);
  
  // if the source kind is not file, we do not have path
  if (source->path != NULL)
    free(source->path);
}

/*
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

#include <stdio.h>
#include <wchar.h>


void tsnc_source_report(struct tsnc_source *source, enum tsnc_report_kind kind,
    size_t startpos, size_t endpos, const char *message, ...) {
  struct tsnc_report *report;
  char *fmtmsg;
  va_list args;

  va_start(args, message);
  vasprintf(&fmtmsg, message, args);
  va_end(args);

  report = (struct tsnc_report*)malloc(sizeof(struct tsnc_report));
  assert(report && "Unable to allocate memory for report");

  report->kind = kind;
  report->startpos = startpos;
  report->endpos = endpos;
  report->message = fmtmsg;
  report->prev = NULL;
  report->next = NULL;

  if (source->leafreport == NULL) {
    source->report = report;
    source->leafreport = report;
    return;
  }

  report->prev = source->leafreport;
  source->leafreport->next = report;
  source->leafreport = report;
}
*/
