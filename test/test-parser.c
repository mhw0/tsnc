#include <tap.h>
#include <tsnc/parser.h>
#include <tsnc/tokenizer.h>
#include <tsnc/ast.h>
#include "test-parser.h"

void tsnc_test_parser_import_decl() {
  struct tsnc_source source;
  struct tsnc_report report;
  struct tsnc_token token;
  struct tsnc_ast_node *decl, *exdecl, *exclause, *exmodspec;

  tsnc_source_memory_create(&source,
    "import 'module0'\n"
    "import * as test from 'module1'\n"
    "import def from 'module2'", -1);

  tsnc_tokenize_source(&source);
  tsnc_parse_source(&source);

  decl = source.node;
  tsnc_ast_strlit_create(&exmodspec, "module0", 7, 15);
  tsnc_ast_import_decl_create(&exdecl, NULL, exmodspec, 0, 15);
  ok(tsnc_ast_node_equal(decl, exdecl),
      "import declaration without import clause");

  decl = decl->next;
  tsnc_ast_ident_create(&exclause, "test", 24, 32);
  tsnc_ast_strlit_create(&exmodspec, "module1", 39, 47);
  tsnc_ast_import_decl_create(&exdecl, exclause, exmodspec, 17, 47);
  ok(tsnc_ast_node_equal(decl, exdecl),
      "import declaration with namespace import");

  decl = decl->next;
  tsnc_ast_ident_create(&exclause, "def", 56, 58);
  tsnc_ast_strlit_create(&exmodspec, "module2", 65, 73);
  tsnc_ast_import_decl_create(&exdecl, exclause, exmodspec, 49, 73);
  ok(tsnc_ast_node_equal(decl, exdecl),
      "import declaration with default import");

  tsnc_source_free(&source);
}
