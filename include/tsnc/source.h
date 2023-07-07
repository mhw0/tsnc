#ifndef TSNC_SOURCE_H
#define TSNC_SOURCE_H
#include <stdio.h>
#include <tsnc/vector.h>
#include <tsnc/program.h>
#include <tsnc/report.h>
#include <tsnc/token-stream.h>

#define tsnc_source_report_error(source, startpos, endpos, message, ...) \
  tsnc_source_report(source, TSNC_REPORT_KIND_ERROR, startpos, endpos, message, __VA_ARGS__);

#define tsnc_source_report_warning(source, startpos, endpos, message, ...) \
  tsnc_source_report(source, TSNC_REPORT_KIND_WARNING, startpos, endpos, message, __VA_ARGS__);

#define tsnc_source_report_info(source, startpos, endpos, message, ...) \
  tsnc_source_report(source, TSNC_REPORT_KIND_INFO, startpos, endpos, message, __VA_ARGS__);

/* holds source fields */
struct tsnc_source {
  /* source path */
  char *path;
  /* file pointer */
  FILE *fp;
  /* token stream */
  struct tsnc_token_stream tokens;
  /* holds reports for this source */
  struct tsnc_vector reportv;
};

int tsnc_source_memory_create(struct tsnc_source *dest,
    const char *source, int len);
int tsnc_source_file_create(struct tsnc_source *dest,
    const char *path);
void tsnc_source_free(struct tsnc_source *source);
void tsnc_source_report(struct tsnc_source *source,
    enum tsnc_report_kind kind, size_t startpos, size_t endpos,
    const char *message, ...);
#endif
