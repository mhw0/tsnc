#include <tap.h>
#include <tsnc/program.h>
#include <tsnc/source.h>
#include <tsnc/tokenizer.h>
#include "test-tokenizer.h"

void tsnc_test_tokenizer_single_character_tokens() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;

  tsnc_source_memory_create(&source, "#@(){}[],:;\\", -1);
  tsnc_tokenize_source(&source);

  ok(tsnc_token_stream_size(&source.tokens) == 13,
      "single character token stream size is 13");
  ok(tsnc_vector_size(&source.reportv, sizeof(struct tsnc_report)) == 0,
      "single token character token report vector size is 0");

  extoken.kind = TSNC_TOKEN_KIND_HASHTAG;
  extoken.startpos = 0; extoken.endpos = 0; extoken.str = "#";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: #");

  extoken.kind = TSNC_TOKEN_KIND_ASPERAND;
  extoken.startpos = 1; extoken.endpos = 1; extoken.str = "@";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: @");

  extoken.kind = TSNC_TOKEN_KIND_ROUND_BRACKET_OPEN;
  extoken.startpos = 2; extoken.endpos = 2; extoken.str = "(";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: (");

  extoken.kind = TSNC_TOKEN_KIND_ROUND_BRACKET_CLOSE;
  extoken.startpos = 3; extoken.endpos = 3; extoken.str = ")";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: )");

  extoken.kind = TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN;
  extoken.startpos = 4; extoken.endpos = 4; extoken.str = "{";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: {");

  extoken.kind = TSNC_TOKEN_KIND_CURLY_BRACKET_CLOSE;
  extoken.startpos = 5; extoken.endpos = 5; extoken.str = "}";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: }");

  extoken.kind = TSNC_TOKEN_KIND_SQUARE_BRACKET_OPEN;
  extoken.startpos = 6; extoken.endpos = 6; extoken.str = "[";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: [");

  extoken.kind = TSNC_TOKEN_KIND_SQUARE_BRACKET_CLOSE;
  extoken.startpos = 7; extoken.endpos = 7; extoken.str = "]";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ]");

  extoken.kind = TSNC_TOKEN_KIND_COMMA;
  extoken.startpos = 8; extoken.endpos = 8; extoken.str = ",";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ,");

  extoken.kind = TSNC_TOKEN_KIND_COLON;
  extoken.startpos = 9; extoken.endpos = 9; extoken.str = ":";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: :");

  extoken.kind = TSNC_TOKEN_KIND_SEMICOLON;
  extoken.startpos = 10; extoken.endpos = 10; extoken.str = ";";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ;");

  extoken.kind = TSNC_TOKEN_KIND_BACKSLASH;
  extoken.startpos = 11; extoken.endpos = 11; extoken.str = "\\";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: \\");

  extoken.kind = TSNC_TOKEN_KIND_EOF;
  extoken.startpos = 11; extoken.endpos = 11; extoken.str = "";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: EOF");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_operators() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;

  tsnc_source_memory_create(&source,
      "+ ++ += - -- -= / /= * ** *= **= = == === "
      "! != !== < << <<= > >> >= >>> >>= >>>= "
      "^ ^= & && &= | || |= % %= ? ??", -1);

  tsnc_tokenize_source(&source);

  ok(tsnc_token_stream_size(&source.tokens) == 40,
      "operator token stream size is 40");
  ok(tsnc_vector_size(&source.reportv, sizeof(struct tsnc_report)) == 0,
      "operator report vector size is 0");

  extoken.kind = TSNC_TOKEN_KIND_PLUS;
  extoken.startpos = 0; extoken.endpos = 0; extoken.str = "+";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: +");

  extoken.kind = TSNC_TOKEN_KIND_PLUS_PLUS;
  extoken.startpos = 2; extoken.endpos = 3; extoken.str = "++";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ++");

  extoken.kind = TSNC_TOKEN_KIND_PLUS_EQUAL;
  extoken.startpos = 5; extoken.endpos = 6; extoken.str = "+=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: +=");

  extoken.kind = TSNC_TOKEN_KIND_MINUS;
  extoken.startpos = 8; extoken.endpos = 8; extoken.str = "-";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: -");

  extoken.kind = TSNC_TOKEN_KIND_MINUS_MINUS;
  extoken.startpos = 10; extoken.endpos = 11; extoken.str = "--";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: --");

  extoken.kind = TSNC_TOKEN_KIND_MINUS_EQUAL;
  extoken.startpos = 13; extoken.endpos = 14; extoken.str = "-=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: -=");

  extoken.kind = TSNC_TOKEN_KIND_SLASH;
  extoken.startpos = 16; extoken.endpos = 16; extoken.str = "/";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: /");

  extoken.kind = TSNC_TOKEN_KIND_SLASH_EQUAL;
  extoken.startpos = 18; extoken.endpos = 19; extoken.str = "/=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: /=");

  extoken.kind = TSNC_TOKEN_KIND_ASTERISK;
  extoken.startpos = 21; extoken.endpos = 21; extoken.str = "*";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: *");

  extoken.kind = TSNC_TOKEN_KIND_ASTERISK_ASTERISK;
  extoken.startpos = 23; extoken.endpos = 24; extoken.str = "**";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: **");

  extoken.kind = TSNC_TOKEN_KIND_ASTERISK_EQUAL;
  extoken.startpos = 26; extoken.endpos = 27; extoken.str = "*=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: *=");

  extoken.kind = TSNC_TOKEN_KIND_ASTERISK_ASTERISK_EQUAL;
  extoken.startpos = 29; extoken.endpos = 31; extoken.str = "**=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: **=");

  extoken.kind = TSNC_TOKEN_KIND_EQUAL;
  extoken.startpos = 33; extoken.endpos = 33; extoken.str = "=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: =");

  extoken.kind = TSNC_TOKEN_KIND_EQUAL_EQUAL;
  extoken.startpos = 35; extoken.endpos = 36; extoken.str = "==";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ==");

  extoken.kind = TSNC_TOKEN_KIND_EQUAL_EQUAL_EQUAL;
  extoken.startpos = 38; extoken.endpos = 40; extoken.str = "===";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ===");

  extoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK;
  extoken.startpos = 42; extoken.endpos = 42; extoken.str = "!";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: !");

  extoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL;
  extoken.startpos = 44; extoken.endpos = 45; extoken.str = "!=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: !=");

  extoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL_EQUAL;
  extoken.startpos = 47; extoken.endpos = 49; extoken.str = "!==";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: !==");

  extoken.kind = TSNC_TOKEN_KIND_LESS_THAN;
  extoken.startpos = 51; extoken.endpos = 51; extoken.str = "<";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: <");

  extoken.kind = TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN;
  extoken.startpos = 53; extoken.endpos = 54; extoken.str = "<<";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: <<");

  extoken.kind = TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN_EQUAL;
  extoken.startpos = 56; extoken.endpos = 58; extoken.str = "<<=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: <<=");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN;
  extoken.startpos = 60; extoken.endpos = 60; extoken.str = ">";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: >");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN;
  extoken.startpos = 62; extoken.endpos = 63; extoken.str = ">>";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: >>");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_EQUAL;
  extoken.startpos = 65; extoken.endpos = 66; extoken.str = ">=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: >=");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN;
  extoken.startpos = 68; extoken.endpos = 70; extoken.str = ">>>";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: >>>");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_EQUAL;
  extoken.startpos = 72; extoken.endpos = 74; extoken.str = ">>=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: >>=");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN_EQUAL;
  extoken.startpos = 76; extoken.endpos = 79; extoken.str = ">>>=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: >>>=");

  extoken.kind = TSNC_TOKEN_KIND_CARET;
  extoken.startpos = 81; extoken.endpos = 81; extoken.str = "^";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ^");

  extoken.kind = TSNC_TOKEN_KIND_CARET_EQUAL;
  extoken.startpos = 83; extoken.endpos = 84; extoken.str = "^=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ^=");

  extoken.kind = TSNC_TOKEN_KIND_AMPERSAND;
  extoken.startpos = 86; extoken.endpos = 86; extoken.str = "&";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: &");

  extoken.kind = TSNC_TOKEN_KIND_AMPERSAND_AMPERSAND;
  extoken.startpos = 88; extoken.endpos = 89; extoken.str = "&&";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: &&");

  extoken.kind = TSNC_TOKEN_KIND_AMPERSAND_EQUAL;
  extoken.startpos = 91; extoken.endpos = 92; extoken.str = "&=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: &=");

  extoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR;
  extoken.startpos = 94; extoken.endpos = 94; extoken.str = "|";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: |");

  extoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR_VERTICAL_BAR;
  extoken.startpos = 96; extoken.endpos = 97; extoken.str = "||";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ||");

  extoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR_EQUAL;
  extoken.startpos = 99; extoken.endpos = 100; extoken.str = "|=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: |=");

  extoken.kind = TSNC_TOKEN_KIND_PERCENTAGE;
  extoken.startpos = 102; extoken.endpos = 102; extoken.str = "%";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: %%");

  extoken.kind = TSNC_TOKEN_KIND_PERCENTAGE_EQUAL;
  extoken.startpos = 104; extoken.endpos = 105; extoken.str = "%=";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: %%=");

  extoken.kind = TSNC_TOKEN_KIND_QUESTION_MARK;
  extoken.startpos = 107; extoken.endpos = 107; extoken.str = "?";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ?");

  extoken.kind = TSNC_TOKEN_KIND_QUESTION_MARK_QUESTION_MARK;
  extoken.startpos = 109; extoken.endpos = 110; extoken.str = "??";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: ??");

  extoken.kind = TSNC_TOKEN_KIND_EOF;
  extoken.startpos = 110; extoken.endpos = 110; extoken.str = "";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: EOF");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_string() {
  struct tsnc_source source;
  struct tsnc_report report, exreport;
  struct tsnc_token token, extoken;

  tsnc_source_memory_create(&source,
      "\"str1\" \'str2\' \"str3\n'str4", -1);

  tsnc_tokenize_source(&source);

  ok(tsnc_token_stream_size(&source.tokens) == 3, "string token stream size is 3");
  ok(tsnc_vector_size(&source.reportv,
      sizeof(struct tsnc_report)) == 2, "string literal report vector size is 2");

  extoken.kind = TSNC_TOKEN_KIND_STRING;
  extoken.startpos = 0; extoken.endpos = 5; extoken.str = "str1";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: \"str1\"");

  extoken.kind = TSNC_TOKEN_KIND_STRING;
  extoken.startpos = 7; extoken.endpos = 12; extoken.str = "str2";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: 'str2'");

  extoken.kind = TSNC_TOKEN_KIND_EOF;
  extoken.startpos = 24; extoken.endpos = 24; extoken.str = "";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: EOF");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 14; exreport.endpos = 18;
  exreport.message = "Invalid or unexpected token";
  tsnc_vector_at(&report, &source.reportv, sizeof(struct tsnc_report), 0);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid or unexpected token error for: \"str3");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 20; exreport.endpos = 24;
  exreport.message = "Invalid or unexpected token";
  tsnc_vector_at(&report, &source.reportv, sizeof(struct tsnc_report), 1);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid or unexpected token error for: 'str4");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_number_hex() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;
  struct tsnc_report report, exreport;

  tsnc_source_memory_create(&source,
      "0xabcdef0123456789 0XFF 0xkk 0x", -1);

  tsnc_tokenize_source(&source);

  ok(tsnc_token_stream_size(&source.tokens) == 3, "hexadecimal token stream size is 3");
  ok(tsnc_vector_size(&source.reportv, sizeof(struct tsnc_report)) == 2,
      "hexadecimal report vector size is 2");

  tsnc_vector_at(&token, &source.tokens.tokenv,
      sizeof(struct tsnc_token), 0);

  extoken.kind = TSNC_TOKEN_KIND_NUMBER;
  extoken.startpos = 0; extoken.endpos = 17;
  extoken.str = "0xabcdef0123456789";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: 0xabcdef0123456789");

  extoken.kind = TSNC_TOKEN_KIND_NUMBER;
  extoken.startpos = 19; extoken.endpos = 22;
  extoken.str = "0XFF";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: 0XFF");

  extoken.kind = TSNC_TOKEN_KIND_EOF;
  extoken.startpos = 30; extoken.endpos = 30; extoken.str = "";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: EOF");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 24; exreport.endpos = 27;
  exreport.message = "Invalid hexadecimal literal";
  tsnc_vector_at(&report, &source.reportv, sizeof(struct tsnc_report), 0);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid hexadecimal literal error for: 0xkk");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 29; exreport.endpos = 30;
  exreport.message = "Invalid hexadecimal literal";
  tsnc_vector_at(&report, &source.reportv, sizeof(struct tsnc_report), 1);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid hexadecimal literal error for: 0x");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_number_bin() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;
  struct tsnc_report report, exreport;

  tsnc_source_memory_create(&source,
      "0b1100101 0B1001 0b123abc 0b", -1);

  tsnc_tokenize_source(&source);

  ok(tsnc_token_stream_size(&source.tokens) == 3,
      "binary number literal token stream size is 3");
  ok(tsnc_vector_size(&source.reportv, sizeof(struct tsnc_report)) == 2,
      "binary number literal report vector size is 2");

  extoken.kind = TSNC_TOKEN_KIND_NUMBER;
  extoken.startpos = 0; extoken.endpos = 8;
  extoken.str = "0b1100101";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: 0b1100101");

  extoken.kind = TSNC_TOKEN_KIND_NUMBER;
  extoken.startpos = 10; extoken.endpos = 15;
  extoken.str = "0B1001";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: 0B1001");

  extoken.kind = TSNC_TOKEN_KIND_EOF;
  extoken.startpos = 27; extoken.endpos = 27; extoken.str = "";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: EOF");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 17; exreport.endpos = 24;
  exreport.message = "Invalid binary number literal";
  tsnc_vector_at(&report, &source.reportv, sizeof(struct tsnc_report), 0);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid binary number literal error for: 0b123abc");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 26; exreport.endpos = 27;
  exreport.message = "Invalid binary number literal";
  tsnc_vector_at(&report, &source.reportv, sizeof(struct tsnc_report), 1);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid binary number literal error for: 0b");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_number_octal() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;
  struct tsnc_report report, exreport;

  tsnc_source_memory_create(&source,
      "0o1234567 0O1234 0o899 0o", -1);

  tsnc_tokenize_source(&source);

  ok(tsnc_token_stream_size(&source.tokens) == 3,
      "octal number literal token stream size is 3");
  ok(tsnc_vector_size(&source.reportv, sizeof(struct tsnc_report)) == 2,
      "octal number literal report vector size is 2");

  extoken.kind = TSNC_TOKEN_KIND_NUMBER;
  extoken.startpos = 0; extoken.endpos = 8;
  extoken.str = "0o1234567";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: 0o1234567");

  extoken.kind = TSNC_TOKEN_KIND_NUMBER;
  extoken.startpos = 10; extoken.endpos = 15;
  extoken.str = "0O1234";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: 0O1234");

  extoken.kind = TSNC_TOKEN_KIND_EOF;
  extoken.startpos = 24; extoken.endpos = 24; extoken.str = "";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: EOF");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 17; exreport.endpos = 21;
  exreport.message = "Invalid octal number literal";
  tsnc_vector_at(&report, &source.reportv, sizeof(struct tsnc_report), 0);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid octal number literal error for: 0o899");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 23; exreport.endpos = 24;
  exreport.message = "Invalid octal number literal";
  tsnc_vector_at(&report, &source.reportv, sizeof(struct tsnc_report), 1);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid octal number literal error for: 0o");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_keywords() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;
  struct tsnc_report report, exreport;

  tsnc_source_memory_create(&source,
      "break case catch class const continue "
      "debugger default delete do else enum "
      "export extends false finally for function "
      "if import in instanceof new null return "
      "super switch this throw true try typeof var "
      "void while with implements interface let "
      "package private protected public static yield "
      "from", -1);

  tsnc_tokenize_source(&source);

  ok(tsnc_token_stream_size(&source.tokens) == 47,
      "keyword token stream size is 47");
  ok(tsnc_vector_size(&source.reportv,
      sizeof(struct tsnc_report)) == 0, "keyword report vector size is 0");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 0; extoken.endpos = 4;
  extoken.str = "break";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: break");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 6; extoken.endpos = 9;
  extoken.str = "case";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: case");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 11; extoken.endpos = 15;
  extoken.str = "catch";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: catch");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 17; extoken.endpos = 21;
  extoken.str = "class";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: class");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 23; extoken.endpos = 27;
  extoken.str = "const";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: const");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 29; extoken.endpos = 36;
  extoken.str = "continue";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: continue");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 38; extoken.endpos = 45;
  extoken.str = "debugger";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: debugger");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 47; extoken.endpos = 53;
  extoken.str = "default";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: default");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 55; extoken.endpos = 60;
  extoken.str = "delete";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: delete");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 62; extoken.endpos = 63;
  extoken.str = "do";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: do");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 65; extoken.endpos = 68;
  extoken.str = "else";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: else");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 70; extoken.endpos = 73;
  extoken.str = "enum";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: enum");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 75; extoken.endpos = 80;
  extoken.str = "export";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: export");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 82; extoken.endpos = 88;
  extoken.str = "extends";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: extends");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 90; extoken.endpos = 94;
  extoken.str = "false";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: false");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 96; extoken.endpos = 102;
  extoken.str = "finally";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: finally");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 104; extoken.endpos = 106;
  extoken.str = "for";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: for");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 108; extoken.endpos = 115;
  extoken.str = "function";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: function");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 117; extoken.endpos = 118;
  extoken.str = "if";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: if");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 120; extoken.endpos = 125;
  extoken.str = "import";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: import");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 127; extoken.endpos = 128;
  extoken.str = "in";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: in");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 130; extoken.endpos = 139;
  extoken.str = "instanceof";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: instanceof");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 141; extoken.endpos = 143;
  extoken.str = "new";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: new");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 145; extoken.endpos = 148;
  extoken.str = "null";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: null");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 150; extoken.endpos = 155;
  extoken.str = "return";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: return");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 157; extoken.endpos = 161;
  extoken.str = "super";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: super");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 163; extoken.endpos = 168;
  extoken.str = "switch";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: switch");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 170; extoken.endpos = 173;
  extoken.str = "this";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: this");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 175; extoken.endpos = 179;
  extoken.str = "throw";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: throw");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 181; extoken.endpos = 184;
  extoken.str = "true";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: true");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 186; extoken.endpos = 188;
  extoken.str = "try";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: try");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 190; extoken.endpos = 195;
  extoken.str = "typeof";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: typeof");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 197; extoken.endpos = 199;
  extoken.str = "var";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: var");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 201; extoken.endpos = 204;
  extoken.str = "void";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: void");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 206; extoken.endpos = 210;
  extoken.str = "while";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: while");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 212; extoken.endpos = 215;
  extoken.str = "with";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: with");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 217; extoken.endpos = 226;
  extoken.str = "implements";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: implements");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 228; extoken.endpos = 236;
  extoken.str = "interface";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: interface");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 238; extoken.endpos = 240;
  extoken.str = "let";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: let");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 242; extoken.endpos = 248;
  extoken.str = "package";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: package");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 250; extoken.endpos = 256;
  extoken.str = "private";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: private");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 258; extoken.endpos = 266;
  extoken.str = "protected";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: protected");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 268; extoken.endpos = 273;
  extoken.str = "public";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: public");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 275; extoken.endpos = 280;
  extoken.str = "static";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: static");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 282; extoken.endpos = 286;
  extoken.str = "yield";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: yield");

  extoken.kind = TSNC_TOKEN_KIND_KEYWORD;
  extoken.startpos = 288; extoken.endpos = 291;
  extoken.str = "from";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: from");

  extoken.kind = TSNC_TOKEN_KIND_EOF;
  extoken.startpos = 291; extoken.endpos = 291; extoken.str = "";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: EOF");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_identifiers() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;
  struct tsnc_report report, exreport;

  tsnc_source_memory_create(&source, "nAme _ide$ntifier0", -1);
  tsnc_tokenize_source(&source);

  ok(tsnc_token_stream_size(&source.tokens) == 3,
      "identifier token stream size is 3");
  ok(tsnc_vector_size(&source.reportv,
      sizeof(struct tsnc_report)) == 0, "report vector size is 0");

  extoken.kind = TSNC_TOKEN_KIND_IDENTIFIER;
  extoken.startpos = 0; extoken.endpos = 3;
  extoken.str = "nAme";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: nAme");

  extoken.kind = TSNC_TOKEN_KIND_IDENTIFIER;
  extoken.startpos = 5; extoken.endpos = 17;
  extoken.str = "_ide$ntifier0";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: _ide$ntifier0");

  extoken.kind = TSNC_TOKEN_KIND_EOF;
  extoken.startpos = 17; extoken.endpos = 17; extoken.str = "";
  tsnc_token_stream_next(&token, &source.tokens);
  ok(tsnc_token_equal(&token, &extoken), "token: EOF");

  tsnc_source_free(&source);
}
