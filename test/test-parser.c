#include <tap.h>
#include <tsnc/parser.h>
#include <tsnc/tokenizer.h>
#include <tsnc/ast.h>
#include <tsnc/utf8.h>
#include "test-parser.h"

void tsnc_test_parser_import_decl(void) {
  struct tsnc_source source;
  struct tsnc_report report;
  struct tsnc_ast_node *node0, *node1, *node2, *node3, *node4,
      *node5, *node6, *node7, *node8, *node9, *node10, *node11,
      *node12;

  tsnc_source_memory_create(&source,
      "import 'test';\n"
      "import def from 'abc';"
      "import * as alias from 'aliasmod';"
      "import {} from 'clause0';"
      "import {}, def from 'clause1';"
      "import def, {} from 'clause2';"
      "import {default as a, b as c, d} from 'clause3';"
      , -1);

  tsnc_tokenize_source(&source);
  tsnc_parse_source(&source);

  node0 = source.node;
  node1 = tsnc_ast_strlit_create("test", 7, 13);
  node2 = tsnc_ast_import_decl_create(NULL, node1, 0, 14);
  ok(tsnc_ast_node_equal(node2, node0),
      "import declaration with only module specifier");
  tsnc_ast_node_free(node2);


  node0 = node0->next;
  node1 = tsnc_ast_ident_create("def", 22, 25);
  node2 = tsnc_ast_import_clause_create(NULL, node1,
      node1->startpos, node1->endpos);
  node3 = tsnc_ast_strlit_create("abc", 31, 36);
  node4 = tsnc_ast_import_decl_create(node2, node3, 15, 37);
  ok(tsnc_ast_node_equal(node4, node0),
      "import declaration with default import and module specifier");
  tsnc_ast_node_free(node4);

  node0 = node0->next;
  node1 = tsnc_ast_ident_create("alias", 49, 54);
  node2 = tsnc_ast_import_namesp_create(node1, 44, 54);
  node3 = tsnc_ast_import_clause_create(node2, NULL, node2->startpos, node2->endpos);
  node4 = tsnc_ast_strlit_create("aliasmod", 60, 70);
  node5 = tsnc_ast_import_decl_create(node3, node4, 37, 71);
  ok(tsnc_ast_node_equal(node5, node0),
      "import declaration with namespace import and module specifier");
  tsnc_ast_node_free(node5);

  node0 = node0->next;
  node1 = tsnc_ast_named_imports_create(NULL, 78, 80);
  node2 = tsnc_ast_import_clause_create(node1, NULL, 78, 80);
  node3 = tsnc_ast_strlit_create("clause0", 86, 95);
  node4 = tsnc_ast_import_decl_create(node2, node3, 71, 96);
  ok(tsnc_ast_node_equal(node4, node0),
      "import declaration with empty named imports clause");
  tsnc_ast_node_free(node4);

  node0 = node0->next;
  node1 = tsnc_ast_named_imports_create(NULL, 103, 105);
  node2 = tsnc_ast_ident_create("def", 107, 110);
  node3 = tsnc_ast_import_clause_create(node1, node2, 103, 110);
  node4 = tsnc_ast_strlit_create("clause1", 116, 125);
  node5 = tsnc_ast_import_decl_create(node3, node4, 96, 126);
  ok(tsnc_ast_node_equal(node5, node0),
      "import declaration with empty named imports and default import clause");
  tsnc_ast_node_free(node5);

  node0 = node0->next;
  node1 = tsnc_ast_ident_create("def", 133, 136);
  node2 = tsnc_ast_named_imports_create(NULL, 138, 140);
  node3 = tsnc_ast_import_clause_create(node2, node1, 133, 140);
  node4 = tsnc_ast_strlit_create("clause2", 146, 155);
  node5 = tsnc_ast_import_decl_create(node3, node4, 126, 156);
  ok(tsnc_ast_node_equal(node5, node0),
      "import declaration with default import and empty named imports clause");
  tsnc_ast_node_free(node5);

  node0 = node0->next;
  node1 = tsnc_ast_ident_create("d", 186, 187);
  node2 = tsnc_ast_named_import_create(node1, NULL, 186, 187);
  node2->next = NULL;
  node3 = tsnc_ast_ident_create("b", 178, 179);
  node4 = tsnc_ast_ident_create("c", 183, 184);
  node5 = tsnc_ast_named_import_create(node3, node4, 178, 184);
  node5->next = node2;
  node6 = tsnc_ast_ident_create("default", 164, 171);
  node7 = tsnc_ast_ident_create("a", 175, 176);
  node8 = tsnc_ast_named_import_create(node6, node7, 164, 176);
  node8->next = node5;
  node9 = tsnc_ast_named_imports_create(node8, 163, 188);
  node10 = tsnc_ast_import_clause_create(node9, NULL, 163, 188);
  node11 = tsnc_ast_strlit_create("clause3", 194, 203);
  node12 = tsnc_ast_import_decl_create(node10, node11, 156, 204);
  ok(tsnc_ast_node_equal(node12, node0),
      "import declaration with named imports");
  tsnc_ast_node_free(node12);

  // TODO: add report test cases

  tsnc_source_free(&source);
}
