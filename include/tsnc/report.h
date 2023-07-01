#ifndef TSNC_REPORT_H
#define TSNC_REPORT_H
#include <stdlib.h>

/* report kind (error, warning or info)*/
enum tsnc_report_kind {
  TSNC_REPORT_KIND_ERROR   = 1 << 0,
  TSNC_REPORT_KIND_WARNING = 1 << 1,
  TSNC_REPORT_KIND_INFO    = 1 << 2
};

/* holds report fields */
struct tsnc_report {
  /* report kind */
  enum tsnc_report_kind kind;
  /* report message */
  char *message;
  /* report start position */
  size_t startpos;
  /* report end position */
  size_t endpos;
};

int tsnc_report_equal(struct tsnc_report *left, struct tsnc_report *right);

#endif
