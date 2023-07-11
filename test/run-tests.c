#include <tap.h>
#include <tsnc/token.h>
#include "test-tokenizer.h"

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

  diag("tokenizer keywords");
  tsnc_test_tokenizer_keywords();

  diag("tokenizer identifiers");
  tsnc_test_tokenizer_identifiers();

  diag("tokenizer end of line");
  tsnc_test_tokenizer_end_of_line();

  done_testing();
  return 0;
}
