#ifndef TSNC_SOURCE_H
#define TSNC_SOURCE_H
#include <stdio.h>
#include "token.h"

#define tsnc_source_report_error(source, pos, end, message) \
  tsnc_source_report(source, TSNC_REPORT_ERROR, pos, end, message)

#define tsnc_source_report_warning(source, pos, end, message) \
  tsnc_source_report(source, TSNC_REPORT_WARNING, pos, end, message)

#define tsnc_source_report_info(source, pos, end, message) \
  tsnc_source_report(source, TSNC_REPORT_INFO, pos, end, message)

struct tsnc_source {
  char *path;
  FILE *fp;
  struct tsnc_vector *tokenv;
  struct tsnc_vector *reportv;
};

int tsnc_source_from_path(struct tsnc_source **dest, const char *path);
int tsnc_source_cleanup(struct tsnc_source *source);

void tsnc_source_report(struct tsnc_source *source, int kind,
    size_t pos, size_t end, char *message);
#endif
