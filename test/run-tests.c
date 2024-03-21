#include <tap.h>
#include "tsnc/compiler/token.h"
#include "test-tokenizer.h"

int main(void) {
  tsnc_test_tokenizer_single_character_tokens();
  tsnc_test_tokenizer_punctuators();
  tsnc_test_tokenizer_line_terminators();

  done_testing();
  return 0;
}
