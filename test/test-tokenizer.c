#include <tap.h>
#include <tsnc/program.h>
#include <tsnc/source.h>
#include <tsnc/tokenizer.h>
#include "test-tokenizer.h"

void tsnc_test_tokenizer_single_character_tokens() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;

  tsnc_source_memory_create(&source, "#@(){}[],:;", -1);
  tsnc_tokenize_source(&source);

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_HASHTAG;
  exptoken.startpos = 0; exptoken.endpos = 1; exptoken.str = "#";
  ok(tsnc_token_equal(token, &exptoken), "token: #");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_ASPERAND;
  exptoken.startpos = 1; exptoken.endpos = 2; exptoken.str = "@";
  ok(tsnc_token_equal(token, &exptoken), "token: @");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_ROUND_BRACKET_OPEN;
  exptoken.startpos = 2; exptoken.endpos = 3; exptoken.str = "(";
  ok(tsnc_token_equal(token, &exptoken), "token: (");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_ROUND_BRACKET_CLOSE;
  exptoken.startpos = 3; exptoken.endpos = 4; exptoken.str = ")";
  ok(tsnc_token_equal(token, &exptoken), "token: )");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_CURLY_BRACKET_OPEN;
  exptoken.startpos = 4; exptoken.endpos = 5; exptoken.str = "{";
  ok(tsnc_token_equal(token, &exptoken), "token: {");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_CURLY_BRACKET_CLOSE;
  exptoken.startpos = 5; exptoken.endpos = 6; exptoken.str = "}";
  ok(tsnc_token_equal(token, &exptoken), "token: }");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_SQUARE_BRACKET_OPEN;
  exptoken.startpos = 6; exptoken.endpos = 7; exptoken.str = "[";
  ok(tsnc_token_equal(token, &exptoken), "token: [");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_SQUARE_BRACKET_CLOSE;
  exptoken.startpos = 7; exptoken.endpos = 8; exptoken.str = "]";
  ok(tsnc_token_equal(token, &exptoken), "token: ]");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_COMMA;
  exptoken.startpos = 8; exptoken.endpos = 9; exptoken.str = ",";
  ok(tsnc_token_equal(token, &exptoken), "token: ,");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_COLON;
  exptoken.startpos = 9; exptoken.endpos = 10; exptoken.str = ":";
  ok(tsnc_token_equal(token, &exptoken), "token: :");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_SEMICOLON;
  exptoken.startpos = 10; exptoken.endpos = 11; exptoken.str = ";";
  ok(tsnc_token_equal(token, &exptoken), "token: ;");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 11; exptoken.endpos = 11; exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");

  token = token->next;
  ok(token == NULL, "no tokens left");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_operators() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;

  tsnc_source_memory_create(&source,
      "+ ++ += - -- -= / /= * ** *= **= = == === "
      "! != !== < << <<= > >> >= >>> >>= >>>= "
      "^ ^= & && &= | || |= % %= ? ??", -1);

  tsnc_tokenize_source(&source);

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_PLUS;
  exptoken.startpos = 0; exptoken.endpos = 1; exptoken.str = "+";
  ok(tsnc_token_equal(token, &exptoken), "token: +");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_PLUS_PLUS;
  exptoken.startpos = 2; exptoken.endpos = 4; exptoken.str = "++";
  ok(tsnc_token_equal(token, &exptoken), "token: ++");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_PLUS_EQUAL;
  exptoken.startpos = 5; exptoken.endpos = 7; exptoken.str = "+=";
  ok(tsnc_token_equal(token, &exptoken), "token: +=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_MINUS;
  exptoken.startpos = 8; exptoken.endpos = 9; exptoken.str = "-";
  ok(tsnc_token_equal(token, &exptoken), "token: -");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_MINUS_MINUS;
  exptoken.startpos = 10; exptoken.endpos = 12; exptoken.str = "--";
  ok(tsnc_token_equal(token, &exptoken), "token: --");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_MINUS_EQUAL;
  exptoken.startpos = 13; exptoken.endpos = 15; exptoken.str = "-=";
  ok(tsnc_token_equal(token, &exptoken), "token: -=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_SLASH;
  exptoken.startpos = 16; exptoken.endpos = 17; exptoken.str = "/";
  ok(tsnc_token_equal(token, &exptoken), "token: /");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_SLASH_EQUAL;
  exptoken.startpos = 18; exptoken.endpos = 20; exptoken.str = "/=";
  ok(tsnc_token_equal(token, &exptoken), "token: /=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_ASTERISK;
  exptoken.startpos = 21; exptoken.endpos = 22; exptoken.str = "*";
  ok(tsnc_token_equal(token, &exptoken), "token: *");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_ASTERISK_ASTERISK;
  exptoken.startpos = 23; exptoken.endpos = 25; exptoken.str = "**";
  ok(tsnc_token_equal(token, &exptoken), "token: **");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_ASTERISK_EQUAL;
  exptoken.startpos = 26; exptoken.endpos = 28; exptoken.str = "*=";
  ok(tsnc_token_equal(token, &exptoken), "token: *=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_ASTERISK_ASTERISK_EQUAL;
  exptoken.startpos = 29; exptoken.endpos = 32; exptoken.str = "**=";
  ok(tsnc_token_equal(token, &exptoken), "token: **=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EQUAL;
  exptoken.startpos = 33; exptoken.endpos = 34; exptoken.str = "=";
  ok(tsnc_token_equal(token, &exptoken), "token: =");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EQUAL_EQUAL;
  exptoken.startpos = 35; exptoken.endpos = 37; exptoken.str = "==";
  ok(tsnc_token_equal(token, &exptoken), "token: ==");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EQUAL_EQUAL_EQUAL;
  exptoken.startpos = 38; exptoken.endpos = 41; exptoken.str = "===";
  ok(tsnc_token_equal(token, &exptoken), "token: ===");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK;
  exptoken.startpos = 42; exptoken.endpos = 43; exptoken.str = "!";
  ok(tsnc_token_equal(token, &exptoken), "token: !");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL;
  exptoken.startpos = 44; exptoken.endpos = 46; exptoken.str = "!=";
  ok(tsnc_token_equal(token, &exptoken), "token: !=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EXCLAMATION_MARK_EQUAL_EQUAL;
  exptoken.startpos = 47; exptoken.endpos = 50; exptoken.str = "!==";
  ok(tsnc_token_equal(token, &exptoken), "token: !==");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_LESS_THAN;
  exptoken.startpos = 51; exptoken.endpos = 52; exptoken.str = "<";
  ok(tsnc_token_equal(token, &exptoken), "token: <");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN;
  exptoken.startpos = 53; exptoken.endpos = 55; exptoken.str = "<<";
  ok(tsnc_token_equal(token, &exptoken), "token: <<");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN_EQUAL;
  exptoken.startpos = 56; exptoken.endpos = 59; exptoken.str = "<<=";
  ok(tsnc_token_equal(token, &exptoken), "token: <<=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_LESS_THAN_LESS_THAN_EQUAL;
  exptoken.kind = TSNC_TOKEN_KIND_GREATER_THAN;
  exptoken.startpos = 60; exptoken.endpos = 61; exptoken.str = ">";
  ok(tsnc_token_equal(token, &exptoken), "token: >");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN;
  exptoken.startpos = 62; exptoken.endpos = 64; exptoken.str = ">>";
  ok(tsnc_token_equal(token, &exptoken), "token: >>");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_EQUAL;
  exptoken.startpos = 65; exptoken.endpos = 67; exptoken.str = ">=";
  ok(tsnc_token_equal(token, &exptoken), "token: >=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN;
  exptoken.startpos = 68; exptoken.endpos = 71; exptoken.str = ">>>";
  ok(tsnc_token_equal(token, &exptoken), "token: >>>");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_EQUAL;
  exptoken.startpos = 72; exptoken.endpos = 75; exptoken.str = ">>=";
  ok(tsnc_token_equal(token, &exptoken), "token: >>=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_GREATER_THAN_GREATER_THAN_GREATER_THAN_EQUAL;
  exptoken.startpos = 76; exptoken.endpos = 80; exptoken.str = ">>>=";
  ok(tsnc_token_equal(token, &exptoken), "token: >>>=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_CARET;
  exptoken.startpos = 82; exptoken.endpos = 83; exptoken.str = "^";
  ok(tsnc_token_equal(token, &exptoken), "token: ^");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_CARET_EQUAL;
  exptoken.startpos = 84; exptoken.endpos = 86; exptoken.str = "^=";
  ok(tsnc_token_equal(token, &exptoken), "token: ^=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_AMPERSAND;
  exptoken.startpos = 87; exptoken.endpos = 88; exptoken.str = "&";
  ok(tsnc_token_equal(token, &exptoken), "token: &");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_AMPERSAND_AMPERSAND;
  exptoken.startpos = 89; exptoken.endpos = 91; exptoken.str = "&&";
  ok(tsnc_token_equal(token, &exptoken), "token: &&");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_AMPERSAND_EQUAL;
  exptoken.startpos = 92; exptoken.endpos = 94; exptoken.str = "&=";
  ok(tsnc_token_equal(token, &exptoken), "token: &=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR;
  exptoken.startpos = 95; exptoken.endpos = 96; exptoken.str = "|";
  ok(tsnc_token_equal(token, &exptoken), "token: |");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR_VERTICAL_BAR;
  exptoken.startpos = 97; exptoken.endpos = 99; exptoken.str = "||";
  ok(tsnc_token_equal(token, &exptoken), "token: ||");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_VERTICAL_BAR_EQUAL;
  exptoken.startpos = 100; exptoken.endpos = 102; exptoken.str = "|=";
  ok(tsnc_token_equal(token, &exptoken), "token: |=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_PERCENTAGE;
  exptoken.startpos = 103; exptoken.endpos = 104; exptoken.str = "%";
  ok(tsnc_token_equal(token, &exptoken), "token: %%");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_PERCENTAGE_EQUAL;
  exptoken.startpos = 105; exptoken.endpos = 107; exptoken.str = "%=";
  ok(tsnc_token_equal(token, &exptoken), "token: %%=");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_QUESTION_MARK;
  exptoken.startpos = 108; exptoken.endpos = 109; exptoken.str = "?";
  ok(tsnc_token_equal(token, &exptoken), "token: ?");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_QUESTION_MARK_QUESTION_MARK;
  exptoken.startpos = 110; exptoken.endpos = 112; exptoken.str = "??";
  ok(tsnc_token_equal(token, &exptoken), "token: ??");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 112; exptoken.endpos = 112; exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");

  token = token->next;
  ok(token == NULL, "no tokens left");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_string() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;
  struct tsnc_report *report, expreport;

  tsnc_source_memory_create(&source,
      "'text' \"текст\" \"文本\" 'test", -1);

  tsnc_tokenize_source(&source);

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_STRING;
  exptoken.startpos = 0; exptoken.endpos = 6; exptoken.str = "text";
  ok(tsnc_token_equal(token, &exptoken), "token: \"text\"");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_STRING;
  exptoken.startpos = 7; exptoken.endpos = 14; exptoken.str = "текст";
  ok(tsnc_token_equal(token, &exptoken), "token: \"текст\"");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_STRING;
  exptoken.startpos = 15; exptoken.endpos = 19; exptoken.str = "文本";
  ok(tsnc_token_equal(token, &exptoken), "token: \"文本\"");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 25; exptoken.endpos = 25; exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");

  ok(token->next == NULL, "no tokens left");

  report = source.report;
  expreport.kind = TSNC_REPORT_KIND_ERROR;
  expreport.startpos = 20; expreport.endpos = 25;
  expreport.message = "Unterminated string literal";
  ok(tsnc_report_equal(report, &expreport),
      "Invalid hexadecimal literal error for: 'test");

  ok(report->next == NULL, "no reports left");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_number_hex() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;
  struct tsnc_report *report, expreport;

  tsnc_source_memory_create(&source,
      "0xabCdeF0123456789 0XFF 0x", -1);

  tsnc_tokenize_source(&source);

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_NUMBER;
  exptoken.startpos = 0; exptoken.endpos = 18;
  exptoken.str = "0xabCdeF0123456789";
  ok(tsnc_token_equal(token, &exptoken), "token: 0xabcdef0123456789");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_NUMBER;
  exptoken.startpos = 19; exptoken.endpos = 23;
  exptoken.str = "0XFF";
  ok(tsnc_token_equal(token, &exptoken), "token: 0XFF");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 26; exptoken.endpos = 26;
  exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");

  ok(token->next == NULL, "no tokens left");

  report = source.report;
  expreport.kind = TSNC_REPORT_KIND_ERROR;
  expreport.startpos = 24; expreport.endpos = 26;
  expreport.message = "Invalid hexadecimal literal";
  ok(tsnc_report_equal(report, &expreport),
      "Invalid hexadecimal literal error for: 0x");

  ok(report->next == NULL, "no reports left");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_number_bin() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;
  struct tsnc_report *report, expreport;

  tsnc_source_memory_create(&source,
      "0b1100101 0B1001 0b", -1);
  tsnc_tokenize_source(&source);

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_NUMBER;
  exptoken.startpos = 0; exptoken.endpos = 9;
  exptoken.str = "0b1100101";
  ok(tsnc_token_equal(token, &exptoken), "token: 0b1100101");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_NUMBER;
  exptoken.startpos = 10; exptoken.endpos = 16;
  exptoken.str = "0B1001";
  ok(tsnc_token_equal(token, &exptoken), "token: 0B1001");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 19; exptoken.endpos = 19;
  exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");
  
  ok(token->next == NULL, "no tokens left");

  report = source.report;
  expreport.kind = TSNC_REPORT_KIND_ERROR;
  expreport.startpos = 17; expreport.endpos = 19;
  expreport.message = "Invalid binary number literal";
  ok(tsnc_report_equal(report, &expreport),
      "Invalid binary number literal error for: 0b");

  ok(report->next == NULL, "no reports left");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_number_octal() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;
  struct tsnc_report *report, expreport;

  tsnc_source_memory_create(&source,
      "0o1234567 0O1234 0o", -1);

  tsnc_tokenize_source(&source);

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_NUMBER;
  exptoken.startpos = 0; exptoken.endpos = 9;
  exptoken.str = "0o1234567";
  ok(tsnc_token_equal(token, &exptoken), "token: 0o1234567");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_NUMBER;
  exptoken.startpos = 10; exptoken.endpos = 16;
  exptoken.str = "0O1234";
  ok(tsnc_token_equal(token, &exptoken), "token: 0O1234");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 19; exptoken.endpos = 19;
  exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");

  ok(token->next == NULL, "no tokens left");

  report = source.report;
  expreport.kind = TSNC_REPORT_KIND_ERROR;
  expreport.startpos = 17; expreport.endpos = 19;
  expreport.message = "Invalid octal number literal";
  ok(tsnc_report_equal(report, &expreport),
      "Invalid octal number literal error for: 0o");

  ok(token->next == NULL, "no reports left");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_keywords() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;
  struct tsnc_report report, expreport;

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

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_BREAK;
  exptoken.startpos = 0; exptoken.endpos = 5;
  exptoken.str = "break";
  ok(tsnc_token_equal(token, &exptoken), "token: break");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_CASE;
  exptoken.startpos = 6; exptoken.endpos = 10;
  exptoken.str = "case";
  ok(tsnc_token_equal(token, &exptoken), "token: case");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_CATCH;
  exptoken.startpos = 11; exptoken.endpos = 16;
  exptoken.str = "catch";
  ok(tsnc_token_equal(token, &exptoken), "token: catch");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_CLASS;
  exptoken.startpos = 17; exptoken.endpos = 22;
  exptoken.str = "class";
  ok(tsnc_token_equal(token, &exptoken), "token: class");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_CONST;
  exptoken.startpos = 23; exptoken.endpos = 28;
  exptoken.str = "const";
  ok(tsnc_token_equal(token, &exptoken), "token: const");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_CONTINUE;
  exptoken.startpos = 29; exptoken.endpos = 37;
  exptoken.str = "continue";
  ok(tsnc_token_equal(token, &exptoken), "token: continue");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_DEBUGGER;
  exptoken.startpos = 38; exptoken.endpos = 46;
  exptoken.str = "debugger";
  ok(tsnc_token_equal(token, &exptoken), "token: debugger");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_DEFAULT;
  exptoken.startpos = 47; exptoken.endpos = 54;
  exptoken.str = "default";
  ok(tsnc_token_equal(token, &exptoken), "token: default");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_DELETE;
  exptoken.startpos = 55; exptoken.endpos = 61;
  exptoken.str = "delete";
  ok(tsnc_token_equal(token, &exptoken), "token: delete");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_DO;
  exptoken.startpos = 62; exptoken.endpos = 64;
  exptoken.str = "do";
  ok(tsnc_token_equal(token, &exptoken), "token: do");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_ELSE;
  exptoken.startpos = 65; exptoken.endpos = 69;
  exptoken.str = "else";
  ok(tsnc_token_equal(token, &exptoken), "token: else");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_ENUM;
  exptoken.startpos = 70; exptoken.endpos = 74;
  exptoken.str = "enum";
  ok(tsnc_token_equal(token, &exptoken), "token: enum");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_EXPORT;
  exptoken.startpos = 75; exptoken.endpos = 81;
  exptoken.str = "export";
  ok(tsnc_token_equal(token, &exptoken), "token: export");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_EXTENDS;
  exptoken.startpos = 82; exptoken.endpos = 89;
  exptoken.str = "extends";
  ok(tsnc_token_equal(token, &exptoken), "token: extends");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_FALSE;
  exptoken.startpos = 90; exptoken.endpos = 95;
  exptoken.str = "false";
  ok(tsnc_token_equal(token, &exptoken), "token: false");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_FINALLY;
  exptoken.startpos = 96; exptoken.endpos = 103;
  exptoken.str = "finally";
  ok(tsnc_token_equal(token, &exptoken), "token: finally");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_FOR;
  exptoken.startpos = 104; exptoken.endpos = 107;
  exptoken.str = "for";
  ok(tsnc_token_equal(token, &exptoken), "token: for");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_FUNCTION;
  exptoken.startpos = 108; exptoken.endpos = 116;
  exptoken.str = "function";
  ok(tsnc_token_equal(token, &exptoken), "token: function");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_IF;
  exptoken.startpos = 117; exptoken.endpos = 119;
  exptoken.str = "if";
  ok(tsnc_token_equal(token, &exptoken), "token: if");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_IMPORT;
  exptoken.startpos = 120; exptoken.endpos = 126;
  exptoken.str = "import";
  ok(tsnc_token_equal(token, &exptoken), "token: import");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_IN;
  exptoken.startpos = 127; exptoken.endpos = 129;
  exptoken.str = "in";
  ok(tsnc_token_equal(token, &exptoken), "token: in");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_INSTANCEOF;
  exptoken.startpos = 130; exptoken.endpos = 140;
  exptoken.str = "instanceof";
  ok(tsnc_token_equal(token, &exptoken), "token: instanceof");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_NEW;
  exptoken.startpos = 141; exptoken.endpos = 144;
  exptoken.str = "new";
  ok(tsnc_token_equal(token, &exptoken), "token: new");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_NULL;
  exptoken.startpos = 145; exptoken.endpos = 149;
  exptoken.str = "null";
  ok(tsnc_token_equal(token, &exptoken), "token: null");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_RETURN;
  exptoken.startpos = 150; exptoken.endpos = 156;
  exptoken.str = "return";
  ok(tsnc_token_equal(token, &exptoken), "token: return");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_SUPER;
  exptoken.startpos = 157; exptoken.endpos = 162;
  exptoken.str = "super";
  ok(tsnc_token_equal(token, &exptoken), "token: super");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_SWITCH;
  exptoken.startpos = 163; exptoken.endpos = 169;
  exptoken.str = "switch";
  ok(tsnc_token_equal(token, &exptoken), "token: switch");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_THIS;
  exptoken.startpos = 170; exptoken.endpos = 174;
  exptoken.str = "this";
  ok(tsnc_token_equal(token, &exptoken), "token: this");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_THROW;
  exptoken.startpos = 175; exptoken.endpos = 180;
  exptoken.str = "throw";
  ok(tsnc_token_equal(token, &exptoken), "token: throw");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_TRUE;
  exptoken.startpos = 181; exptoken.endpos = 185;
  exptoken.str = "true";
  ok(tsnc_token_equal(token, &exptoken), "token: true");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_TRY;
  exptoken.startpos = 186; exptoken.endpos = 189;
  exptoken.str = "try";
  ok(tsnc_token_equal(token, &exptoken), "token: try");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_TYPEOF;
  exptoken.startpos = 190; exptoken.endpos = 196;
  exptoken.str = "typeof";
  ok(tsnc_token_equal(token, &exptoken), "token: typeof");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_VAR;
  exptoken.startpos = 197; exptoken.endpos = 200;
  exptoken.str = "var";
  ok(tsnc_token_equal(token, &exptoken), "token: var");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_VOID;
  exptoken.startpos = 201; exptoken.endpos = 205;
  exptoken.str = "void";
  ok(tsnc_token_equal(token, &exptoken), "token: void");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_WHILE;
  exptoken.startpos = 206; exptoken.endpos = 211;
  exptoken.str = "while";
  ok(tsnc_token_equal(token, &exptoken), "token: while");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_WITH;
  exptoken.startpos = 212; exptoken.endpos = 216;
  exptoken.str = "with";
  ok(tsnc_token_equal(token, &exptoken), "token: with");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_IMPLEMENTS;
  exptoken.startpos = 217; exptoken.endpos = 227;
  exptoken.str = "implements";
  ok(tsnc_token_equal(token, &exptoken), "token: implements");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_INTERFACE;
  exptoken.startpos = 228; exptoken.endpos = 237;
  exptoken.str = "interface";
  ok(tsnc_token_equal(token, &exptoken), "token: interface");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_LET;
  exptoken.startpos = 238; exptoken.endpos = 241;
  exptoken.str = "let";
  ok(tsnc_token_equal(token, &exptoken), "token: let");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_PACKAGE;
  exptoken.startpos = 242; exptoken.endpos = 249;
  exptoken.str = "package";
  ok(tsnc_token_equal(token, &exptoken), "token: package");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_PRIVATE;
  exptoken.startpos = 250; exptoken.endpos = 257;
  exptoken.str = "private";
  ok(tsnc_token_equal(token, &exptoken), "token: private");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_PROTECTED;
  exptoken.startpos = 258; exptoken.endpos = 267;
  exptoken.str = "protected";
  ok(tsnc_token_equal(token, &exptoken), "token: protected");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_PUBLIC;
  exptoken.startpos = 268; exptoken.endpos = 274;
  exptoken.str = "public";
  ok(tsnc_token_equal(token, &exptoken), "token: public");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_STATIC;
  exptoken.startpos = 275; exptoken.endpos = 281;
  exptoken.str = "static";
  ok(tsnc_token_equal(token, &exptoken), "token: static");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_YIELD;
  exptoken.startpos = 282; exptoken.endpos = 287;
  exptoken.str = "yield";
  ok(tsnc_token_equal(token, &exptoken), "token: yield");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_KEYWORD_FROM;
  exptoken.startpos = 288; exptoken.endpos = 292;
  exptoken.str = "from";
  ok(tsnc_token_equal(token, &exptoken), "token: from");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 292; exptoken.endpos = 292; exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");

  ok(token->next == NULL, "no tokens left");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_identifiers() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;
  struct tsnc_report report, expreport;

  tsnc_source_memory_create(&source, "identifier $идентификатор _标识符", -1);
  tsnc_tokenize_source(&source);

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_IDENTIFIER;
  exptoken.startpos = 0; exptoken.endpos = 10;
  exptoken.str = "identifier";
  ok(tsnc_token_equal(token, &exptoken), "token: identifier");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_IDENTIFIER;
  exptoken.startpos = 11; exptoken.endpos = 25;
  exptoken.str = "$идентификатор";
  ok(tsnc_token_equal(token, &exptoken), "token: $идентификатор");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_IDENTIFIER;
  exptoken.startpos = 26; exptoken.endpos = 30;
  exptoken.str = "_标识符";
  ok(tsnc_token_equal(token, &exptoken), "token: _标识符");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 30; exptoken.endpos = 30;
  exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");

  ok(token->next == NULL, "no tokens left");

  tsnc_source_free(&source);
}

void tsnc_test_tokenizer_end_of_line() {
  struct tsnc_source source;
  struct tsnc_token *token, exptoken;

  tsnc_source_memory_create(&source, "ab\n\ncd", -1);
  tsnc_tokenize_source(&source);

  token = source.token;
  exptoken.kind = TSNC_TOKEN_KIND_IDENTIFIER;
  exptoken.startpos = 0; exptoken.endpos = 2; exptoken.str = "ab";
  ok(tsnc_token_equal(token, &exptoken), "token: ab");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOL;
  exptoken.startpos = 2; exptoken.endpos = 3; exptoken.str = "<eol>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eol>");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_IDENTIFIER;
  exptoken.startpos = 4; exptoken.endpos = 6; exptoken.str = "cd";
  ok(tsnc_token_equal(token, &exptoken), "token: cd");

  token = token->next;
  exptoken.kind = TSNC_TOKEN_KIND_EOF;
  exptoken.startpos = 6; exptoken.endpos = 6; exptoken.str = "<eof>";
  ok(tsnc_token_equal(token, &exptoken), "token: <eof>");

  ok(token->next == NULL, "no tokens left");

  tsnc_source_free(&source);
}
