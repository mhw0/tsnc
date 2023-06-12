#include <assert.h> // assert
#include <stdlib.h> // malloc, free
#include <string.h> // strlen, memcpy
#include "source.h"
#include "vector.h"
#include "report.h"

int tsnc_source_from_path(struct tsnc_source **dest, const char *path) {
  struct tsnc_source *source;
  char *strpath;
  size_t pathl;
  FILE *fp;

  fp = fopen(path, "r");
  if (fp == NULL)
    return -1;

  source = (struct tsnc_source*)malloc(sizeof(struct tsnc_source));
  assert(source && "Unable to allocate memory for source");

  pathl = strlen(path);

  strpath = (char*)malloc(pathl + 1);
  assert(strpath && "Unable to allocate memory for path"); // TODO: free source if fails
  memcpy(strpath, path, pathl);
  strpath[pathl] = '\0';

  source->fp = fp;
  source->path = strpath;

  *dest = source;
  return 1;
};

int tsnc_source_cleanup(struct tsnc_source *source) {
  if (source == NULL)
    return -1;

  fclose(source->fp);
  free(source->path);
  return 1;
}

void tsnc_source_report(struct tsnc_source *source, int kind,
    size_t pos, size_t end, char *message) {
  struct tsnc_report report = {.kind=kind, .message=message, .pos=pos, .end=end};
  tsnc_vector_add(source->reportv, &report, sizeof(struct tsnc_report));
}
