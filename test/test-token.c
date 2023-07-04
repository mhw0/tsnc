#include <tap.h>
#include <tsnc/program.h>
#include <tsnc/token.h>
#include <tsnc/source.h>
#include "test-token.h"

void tsnc_test_tokenizer_single_character_tokens() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;

  tsnc_source_memory_create(&source, "#@(){}[],:;\\", -1);
  tsnc_source_compile(&source);

  extoken.kind = TSNC_TOKEN_KIND_HASHTAG;
  extoken.startpos = 0; extoken.endpos = 0; extoken.str = "#";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 0);
  ok(tsnc_token_equal(&token, &extoken), "token: #");

  extoken.kind = TSNC_TOKEN_KIND_ASPERAND;
  extoken.startpos = 1; extoken.endpos = 1; extoken.str = "@";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 1);
  ok(tsnc_token_equal(&token, &extoken), "token: @");

  extoken.kind = TSNC_TOKEN_KIND_ROUND_BRACKET_OPEN;
  extoken.startpos = 2; extoken.endpos = 2; extoken.str = "(";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 2);
  ok(tsnc_token_equal(&token, &extoken), "token: (");

  extoken.kind = TSNC_TOKEN_KIND_ROUND_BRACKET_CLOSE;
  extoken.startpos = 3; extoken.endpos = 3; extoken.str = ")";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 3);
  ok(tsnc_token_equal(&token, &extoken), "token: )");

  extoken.kind = TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN;
  extoken.startpos = 4; extoken.endpos = 4; extoken.str = "{";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 4);
  ok(tsnc_token_equal(&token, &extoken), "token: {");

  extoken.kind = TSNC_TOKEN_KIND_CURLY_BRACKET_CLOSE;
  extoken.startpos = 5; extoken.endpos = 5; extoken.str = "}";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 5);
  ok(tsnc_token_equal(&token, &extoken), "token: }");

  extoken.kind = TSNC_TOKEN_KIND_SQUARE_BRACKET_OPEN;
  extoken.startpos = 6; extoken.endpos = 6; extoken.str = "[";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 6);
  ok(tsnc_token_equal(&token, &extoken), "token: [");

  extoken.kind = TSNC_TOKEN_KIND_SQUARE_BRACKET_CLOSE;
  extoken.startpos = 7; extoken.endpos = 7; extoken.str = "]";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 7);
  ok(tsnc_token_equal(&token, &extoken), "token: ]");

  extoken.kind = TSNC_TOKEN_KIND_COMMA;
  extoken.startpos = 8; extoken.endpos = 8; extoken.str = ",";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 8);
  ok(tsnc_token_equal(&token, &extoken), "token: ,");

  extoken.kind = TSNC_TOKEN_KIND_COLON;
  extoken.startpos = 9; extoken.endpos = 9; extoken.str = ":";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 9);
  ok(tsnc_token_equal(&token, &extoken), "token: :");

  extoken.kind = TSNC_TOKEN_KIND_SEMICOLON;
  extoken.startpos = 10; extoken.endpos = 10; extoken.str = ";";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 10);
  ok(tsnc_token_equal(&token, &extoken), "token: ;");

  extoken.kind = TSNC_TOKEN_KIND_BACKSLASH;
  extoken.startpos = 11; extoken.endpos = 11; extoken.str = "\\";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 11);
  ok(tsnc_token_equal(&token, &extoken), "token: \\");

  tsnc_source_cleanup(&source);
}

void tsnc_test_tokenizer_operators() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;

  tsnc_source_memory_create(&source,
      "+ ++ += - -- -= / /= * ** *= **= = == === "
      "! != !== < << <<= > >> >= >>> >>= >>>= "
      "^ ^= & && &= | || |= % %= ? ??", -1);

  tsnc_source_compile(&source);

  extoken.kind = TSNC_TOKEN_KIND_PLUS;
  extoken.startpos = 0; extoken.endpos = 0; extoken.str = "+";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 0);
  ok(tsnc_token_equal(&token, &extoken), "token: +");

  extoken.kind = TSNC_TOKEN_KIND_PLUS_PLUS;
  extoken.startpos = 2; extoken.endpos = 3; extoken.str = "++";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 1);
  ok(tsnc_token_equal(&token, &extoken), "token: ++");

  extoken.kind = TSNC_TOKEN_KIND_PLUS_EQUAL;
  extoken.startpos = 5; extoken.endpos = 6; extoken.str = "+=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 2);
  ok(tsnc_token_equal(&token, &extoken), "token: +=");

  extoken.kind = TSNC_TOKEN_KIND_MINUS;
  extoken.startpos = 8; extoken.endpos = 8; extoken.str = "-";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 3);
  ok(tsnc_token_equal(&token, &extoken), "token: -");

  extoken.kind = TSNC_TOKEN_KIND_MINUS_MINUS;
  extoken.startpos = 10; extoken.endpos = 11; extoken.str = "--";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 4);
  ok(tsnc_token_equal(&token, &extoken), "token: --");

  extoken.kind = TSNC_TOKEN_KIND_MINUS_EQUAL;
  extoken.startpos = 13; extoken.endpos = 14; extoken.str = "-=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 5);
  ok(tsnc_token_equal(&token, &extoken), "token: -=");

  extoken.kind = TSNC_TOKEN_KIND_SLASH;
  extoken.startpos = 16; extoken.endpos = 16; extoken.str = "/";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 6);
  ok(tsnc_token_equal(&token, &extoken), "token: /");

  extoken.kind = TSNC_TOKEN_KIND_SLASH_EQUAL;
  extoken.startpos = 18; extoken.endpos = 19; extoken.str = "/=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 7);
  ok(tsnc_token_equal(&token, &extoken), "token: /=");

  extoken.kind = TSNC_TOKEN_KIND_ASTERISK;
  extoken.startpos = 21; extoken.endpos = 21; extoken.str = "*";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 8);
  ok(tsnc_token_equal(&token, &extoken), "token: *");

  extoken.kind = TSNC_TOKEN_KIND_ASTERISK_ASTERISK;
  extoken.startpos = 23; extoken.endpos = 24; extoken.str = "**";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 9);
  ok(tsnc_token_equal(&token, &extoken), "token: **");

  extoken.kind = TSNC_TOKEN_KIND_ASTERISK_EQUAL;
  extoken.startpos = 26; extoken.endpos = 27; extoken.str = "*=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 10);
  ok(tsnc_token_equal(&token, &extoken), "token: *=");

  extoken.kind = TSNC_TOKEN_KIND_ASTERISK_ASTERISK_EQUAL;
  extoken.startpos = 29; extoken.endpos = 31; extoken.str = "**=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 11);
  ok(tsnc_token_equal(&token, &extoken), "token: **=");

  extoken.kind = TSNC_TOKEN_KIND_EQUAL;
  extoken.startpos = 33; extoken.endpos = 33; extoken.str = "=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 12);
  ok(tsnc_token_equal(&token, &extoken), "token: =");

  extoken.kind = TSNC_TOKEN_KIND_EQUAL_EQUAL;
  extoken.startpos = 35; extoken.endpos = 36; extoken.str = "==";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 13);
  ok(tsnc_token_equal(&token, &extoken), "token: ==");

  extoken.kind = TSNC_TOKEN_KIND_EQUAL_EQUAL_EQUAL;
  extoken.startpos = 38; extoken.endpos = 40; extoken.str = "===";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 14);
  ok(tsnc_token_equal(&token, &extoken), "token: ===");

  extoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK;
  extoken.startpos = 42; extoken.endpos = 42; extoken.str = "!";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 15);
  ok(tsnc_token_equal(&token, &extoken), "token: !");

  extoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL;
  extoken.startpos = 44; extoken.endpos = 45; extoken.str = "!=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 16);
  ok(tsnc_token_equal(&token, &extoken), "token: !=");

  extoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL_EQUAL;
  extoken.startpos = 47; extoken.endpos = 49; extoken.str = "!==";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 17);
  ok(tsnc_token_equal(&token, &extoken), "token: !==");

  extoken.kind = TSNC_TOKEN_KIND_LESS_THAN;
  extoken.startpos = 51; extoken.endpos = 51; extoken.str = "<";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 18);
  ok(tsnc_token_equal(&token, &extoken), "token: <");

  extoken.kind = TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN;
  extoken.startpos = 53; extoken.endpos = 54; extoken.str = "<<";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 19);
  ok(tsnc_token_equal(&token, &extoken), "token: <<");

  extoken.kind = TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN_EQUAL;
  extoken.startpos = 56; extoken.endpos = 58; extoken.str = "<<=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 20);
  ok(tsnc_token_equal(&token, &extoken), "token: <<=");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN;
  extoken.startpos = 60; extoken.endpos = 60; extoken.str = ">";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 21);
  ok(tsnc_token_equal(&token, &extoken), "token: >");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN;
  extoken.startpos = 62; extoken.endpos = 63; extoken.str = ">>";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 22);
  ok(tsnc_token_equal(&token, &extoken), "token: >>");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_EQUAL;
  extoken.startpos = 65; extoken.endpos = 66; extoken.str = ">=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 23);
  ok(tsnc_token_equal(&token, &extoken), "token: >=");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN;
  extoken.startpos = 68; extoken.endpos = 70; extoken.str = ">>>";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 24);
  ok(tsnc_token_equal(&token, &extoken), "token: >>>");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_EQUAL;
  extoken.startpos = 72; extoken.endpos = 74; extoken.str = ">>=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 25);
  ok(tsnc_token_equal(&token, &extoken), "token: >>=");

  extoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN_EQUAL;
  extoken.startpos = 76; extoken.endpos = 79; extoken.str = ">>>=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 26);
  ok(tsnc_token_equal(&token, &extoken), "token: >>>=");

  extoken.kind = TSNC_TOKEN_KIND_CARET;
  extoken.startpos = 81; extoken.endpos = 81; extoken.str = "^";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 27);
  ok(tsnc_token_equal(&token, &extoken), "token: ^");

  extoken.kind = TSNC_TOKEN_KIND_CARET_EQUAL;
  extoken.startpos = 83; extoken.endpos = 84; extoken.str = "^=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 28);
  ok(tsnc_token_equal(&token, &extoken), "token: ^=");

  extoken.kind = TSNC_TOKEN_KIND_AMPERSAND;
  extoken.startpos = 86; extoken.endpos = 86; extoken.str = "&";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 29);
  ok(tsnc_token_equal(&token, &extoken), "token: &");

  extoken.kind = TSNC_TOKEN_KIND_AMPERSAND_AMPERSAND;
  extoken.startpos = 88; extoken.endpos = 89; extoken.str = "&&";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 30);
  ok(tsnc_token_equal(&token, &extoken), "token: &&");

  extoken.kind = TSNC_TOKEN_KIND_AMPERSAND_EQUAL;
  extoken.startpos = 91; extoken.endpos = 92; extoken.str = "&=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 31);
  ok(tsnc_token_equal(&token, &extoken), "token: &=");

  extoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR;
  extoken.startpos = 94; extoken.endpos = 94; extoken.str = "|";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 32);
  ok(tsnc_token_equal(&token, &extoken), "token: |");

  extoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR_VERTICAL_BAR;
  extoken.startpos = 96; extoken.endpos = 97; extoken.str = "||";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 33);
  ok(tsnc_token_equal(&token, &extoken), "token: ||");

  extoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR_EQUAL;
  extoken.startpos = 99; extoken.endpos = 100; extoken.str = "|=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 34);
  ok(tsnc_token_equal(&token, &extoken), "token: |=");

  extoken.kind = TSNC_TOKEN_KIND_PERCENTAGE;
  extoken.startpos = 102; extoken.endpos = 102; extoken.str = "%";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 35);
  ok(tsnc_token_equal(&token, &extoken), "token: %%");

  extoken.kind = TSNC_TOKEN_KIND_PERCENTAGE_EQUAL;
  extoken.startpos = 104; extoken.endpos = 105; extoken.str = "%=";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 36);
  ok(tsnc_token_equal(&token, &extoken), "token: %%=");

  extoken.kind = TSNC_TOKEN_KIND_QUESTION_MARK;
  extoken.startpos = 107; extoken.endpos = 107; extoken.str = "?";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 37);
  ok(tsnc_token_equal(&token, &extoken), "token: ?");

  extoken.kind = TSNC_TOKEN_KIND_QUESTION_MARK_QUESTION_MARK;
  extoken.startpos = 109; extoken.endpos = 110; extoken.str = "??";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 38);
  ok(tsnc_token_equal(&token, &extoken), "token: ??");

  tsnc_source_cleanup(&source);
}

void tsnc_test_tokenizer_string() {
  struct tsnc_source source;
  struct tsnc_report report, exreport;
  struct tsnc_token token, extoken;

  tsnc_source_memory_create(&source,
      "\"str1\" \'str2\' \"str3\n'str4", -1);

  tsnc_source_compile(&source);

  ok(tsnc_vector_size(&source.tokenv,
        sizeof(struct tsnc_report)) == 2, "token vector size is 2");

  extoken.kind = TSNC_TOKEN_KIND_STRING;
  extoken.startpos = 0; extoken.endpos = 5; extoken.str = "str1";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 0);
  ok(tsnc_token_equal(&token, &extoken), "token: \"str1\"");

  extoken.kind = TSNC_TOKEN_KIND_STRING;
  extoken.startpos = 7; extoken.endpos = 12; extoken.str = "str2";
  tsnc_vector_at(&token, &source.tokenv, sizeof(struct tsnc_token), 1);
  ok(tsnc_token_equal(&token, &extoken), "token: 'str2'");

  ok(tsnc_vector_size(&source.reportv,
        sizeof(struct tsnc_report)) == 2, "report vector size is 2");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 14; exreport.endpos = 18;
  exreport.message = "Invalid or unexpected token";
  tsnc_vector_at(&report, &source.reportv,
      sizeof(struct tsnc_report), 0);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid or unexpected token error for: \"str3");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 20; exreport.endpos = 24;
  exreport.message = "Invalid or unexpected token";
  tsnc_vector_at(&report, &source.reportv,
      sizeof(struct tsnc_report), 1);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid or unexpected token error for: 'str4");

  tsnc_source_cleanup(&source);
}

void tsnc_test_tokenizer_number_hex() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;
  struct tsnc_report report, exreport;

  tsnc_source_memory_create(&source,
      "0xabcdef0123456789 0xkk 0x", -1);

  tsnc_source_compile(&source);

  ok(tsnc_vector_size(&source.tokenv,
      sizeof(struct tsnc_token)) == 1, "Hex token vector size is 1");

  ok(tsnc_vector_size(&source.reportv,
      sizeof(struct tsnc_report)) == 2, "Hex report vector size is 2");

  extoken.kind = TSNC_TOKEN_KIND_NUMBER;
  extoken.startpos = 0; extoken.endpos = 17;
  extoken.str = "0xabcdef0123456789";
  tsnc_vector_at(&token, &source.tokenv,
      sizeof(struct tsnc_token), 0);
  ok(tsnc_token_equal(&token, &extoken), "token: 0xabcdef0123456789");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 19; exreport.endpos = 22;
  exreport.message = "Invalid hexadecimal literal";
  tsnc_vector_at(&report, &source.reportv,
      sizeof(struct tsnc_report), 0);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid hexadecimal literal error for: 0xkk");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 24; exreport.endpos = 25;
  exreport.message = "Invalid hexadecimal literal";
  tsnc_vector_at(&report, &source.reportv,
      sizeof(struct tsnc_report), 1);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid hexadecimal literal error for: 0x");

  tsnc_source_cleanup(&source);
}

void tsnc_test_tokenizer_number_bin() {
  struct tsnc_source source;
  struct tsnc_token token, extoken;
  struct tsnc_report report, exreport;

  tsnc_source_memory_create(&source,
      "0b1100101 0b123abc 0b", -1);

  tsnc_source_compile(&source);

  ok(tsnc_vector_size(&source.tokenv,
      sizeof(struct tsnc_token)) == 1, "Bin token vector size is 1");

  ok(tsnc_vector_size(&source.reportv,
      sizeof(struct tsnc_report)) == 2, "Bin report vector size is 2");

  extoken.kind = TSNC_TOKEN_KIND_NUMBER;
  extoken.startpos = 0; extoken.endpos = 8;
  extoken.str = "0b1100101";
  tsnc_vector_at(&token, &source.tokenv,
      sizeof(struct tsnc_token), 0);
  ok(tsnc_token_equal(&token, &extoken), "token: 0b1100101");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 10; exreport.endpos = 17;
  exreport.message = "Invalid binary number literal";
  tsnc_vector_at(&report, &source.reportv,
      sizeof(struct tsnc_report), 0);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid binary number literal error for: 0b123abc");

  exreport.kind = TSNC_REPORT_KIND_ERROR;
  exreport.startpos = 19; exreport.endpos = 20;
  exreport.message = "Invalid binary number literal";
  tsnc_vector_at(&report, &source.reportv,
      sizeof(struct tsnc_report), 1);
  ok(tsnc_report_equal(&report, &exreport),
      "Invalid binary number literal: 0b");

  tsnc_source_cleanup(&source);
}
