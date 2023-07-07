#include <tsnc/token.h>
#include <tsnc/source.h>

int tsnc_token_create(struct tsnc_token *dest,
    enum tsnc_token_kind kind, char *token, size_t len,
    size_t startpos, size_t endpos) {
  char *buf;

  if (dest == NULL)
    return 0;

  buf = malloc(len + 1);
  if (buf == NULL)
    return 0;

  memcpy(buf, token, len);
  buf[len] = '\0';

  dest->kind = kind;
  dest->str = buf;
  dest->startpos = startpos;
  dest->endpos = endpos;
  
  return 1;
}

int tsnc_token_equal(struct tsnc_token *left,
    struct tsnc_token *right) {
  if (left == NULL || right == NULL)
    return 0;

  if (left->kind != right->kind)
    return 0;

  if (left->startpos != right->startpos)
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
