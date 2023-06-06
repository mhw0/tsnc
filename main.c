#include "source.h"
#include "token.h"

int main(void) {
  struct tsnc_source *source;
  tsnc_source_from_path(&source, "test.ts");

  tsnc_tokenize_source(source);

  tsnc_source_cleanup(source);
  return 0;
};
