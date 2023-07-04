#include <tap.h>
#include <tsnc/token.h>
#include "test-token.h"

int main(void) {
  diag("tokenizer single character operators");
  tsnc_test_tokenizer_single_character_tokens();

  diag("tokenizer operators");
  tsnc_test_tokenizer_operators();

  diag("tokenizer string");
  tsnc_test_tokenizer_string();

  diag("tokenizer hex number");
  tsnc_test_tokenizer_number_hex();

  diag("tokenizer binary number");
  tsnc_test_tokenizer_number_bin();

  diag("tokenizer octal number");
  tsnc_test_tokenizer_number_octal();

  done_testing();
  return 0;
}
