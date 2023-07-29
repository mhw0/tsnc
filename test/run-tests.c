#include <tap.h>
#include <tsnc/token.h>
#include "test-tokenizer.h"
#include "test-parser.h"

int main(void) {
  diag("tokenizer single character operators");
  tsnc_test_tokenizer_single_character_tokens();

  diag("tokenizer operators");
  tsnc_test_tokenizer_operators();

  diag("tokenizer identifiers");
  tsnc_test_tokenizer_identifiers();

  diag("tokenizer keywords");
  tsnc_test_tokenizer_keywords();

  diag("tokenizer hex number");
  tsnc_test_tokenizer_number_hex();

  diag("tokenizer end of line");
  tsnc_test_tokenizer_end_of_line();

  diag("tokenizer binary number");
  tsnc_test_tokenizer_number_bin();

  diag("tokenizer octal number");
  tsnc_test_tokenizer_number_octal();

  diag("tokenizer string");
  tsnc_test_tokenizer_string();

  diag("parser import declaration");
  tsnc_test_parser_import_decl();

  done_testing();
  return 0;
}
