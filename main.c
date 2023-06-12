#include "source.h"
#include "token.h"
#include "report.h"

int main(void) {
  struct tsnc_source *source;
  struct tsnc_report *report;

  tsnc_source_from_path(&source, "test.ts");

  tsnc_tokenize_source(source);

  while ((report = tsnc_vector_iter(source->reportv, sizeof(struct tsnc_report))))
    printf("%s:%zu:%zu: %s\n", source->path, report->pos, report->end, report->message);

  tsnc_source_cleanup(source);
  return 0;
};
