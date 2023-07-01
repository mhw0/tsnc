#include <tsnc/report.h>
#include <string.h>

int tsnc_report_equal(struct tsnc_report *left,
    struct tsnc_report *right) {

  if (left == NULL || right == NULL)
    return 0;

  if (left->kind != right->kind)
    return 0;

  if (left->startpos != right->startpos)
    return 0;

  if (left->endpos != right->endpos)
    return 0;

  if (strcmp(left->message, right->message) != 0)
    return 0;

  return 1;
}
