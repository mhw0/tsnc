#ifndef TSNC_REPORT_H
#define TSNC_REPORT_H

enum tsnc_report_kind {
  TSNC_REPORT_ERROR,
  TSNC_REPORT_WARNING,
  TSNC_REPORT_INFO
};

struct tsnc_report {
  enum tsnc_report_kind kind;
  char *message;
  size_t pos;
  size_t end;
};

#endif
