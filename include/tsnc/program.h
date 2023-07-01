#ifndef TSNC_PROGRAM_H
#define TSNC_PROGRAM_H
#include <stdlib.h>
#include <tsnc/source.h>
#include <tsnc/vector.h>
#include <tsnc/report.h>

/* holds program fields */
struct tsnc_program {
  /* source files */
  struct tsnc_vector sourcev;
};

int tsnc_program_create(struct tsnc_program *dest);
int tsnc_program_cleanup(struct tsnc_program *dest);
#endif
