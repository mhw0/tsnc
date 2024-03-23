#include <tap.h>
#include "test-tokenizer.h"

int main(void) {
  printf("=== single character tokens ===\n");
  tsnc_test_tokenizer_single_character_tokens();
  printf("=== punctuators ===\n");
  tsnc_test_tokenizer_punctuators();
  printf("=== line terminators ===\n");
  tsnc_test_tokenizer_line_terminators();

  done_testing();
  return 0;
}
