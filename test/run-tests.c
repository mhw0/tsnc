#include <tap.h>
#include <tsnc/token.h>
#include "test-token.h"

int main(void) {
  tsnc_test_tokenizer_single_character_tokens();
  tsnc_test_tokenizer_operators();
  tsnc_test_tokenizer_string();

  done_testing();
  return 0;
}
