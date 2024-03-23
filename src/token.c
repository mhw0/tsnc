#include <string.h>
#include <tsnc/token.h>
#include <tsnc/source.h>

int tsnc_token_equal(struct tsnc_token *left,
    struct tsnc_token *right) {
  if (left == NULL || right == NULL)
    return 0;

  if (left->kind != right->kind)
    return 0;

  if (left->begpos != right->begpos)
    return 0;

  if (left->endpos != right->endpos)
    return 0;

  if (strcmp(left->str, right->str) != 0)
    return 0;

  return 1;
}

void tsnc_token_free(struct tsnc_token *token) {
  if (token == NULL)
    return;

  free(token->str);
}
