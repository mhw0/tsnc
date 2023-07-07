#include <tsnc/token-stream.h>

void tsnc_token_stream_init(struct tsnc_token_stream *tokens) {
  if (tokens == NULL)
    return;

  tsnc_vector_init(&tokens->tokenv);
  tokens->curs = 0;
}

size_t tsnc_token_stream_size(struct tsnc_token_stream *tokens) {
  if (tokens == NULL)
    return 0;

  return tsnc_vector_size(&tokens->tokenv, sizeof(struct tsnc_token));
}

int tsnc_token_stream_peek(struct tsnc_token *dest,
    struct tsnc_token_stream *tokens) {
  if (dest == NULL || tokens == NULL)
    return 0;

  return tsnc_vector_at(dest, &tokens->tokenv,
      sizeof(struct tsnc_token), tokens->curs);
}

int tsnc_token_stream_next(struct tsnc_token *dest,
    struct tsnc_token_stream *tokens) {
  if (dest == NULL || tokens == NULL)
    return 0;

  return tsnc_vector_at(dest, &tokens->tokenv,
      sizeof(struct tsnc_token), tokens->curs++);
}

int tsnc_token_stream_add(struct tsnc_token_stream *tokens,
    struct tsnc_token *token) {

  return tsnc_vector_push(&tokens->tokenv,
      sizeof(struct tsnc_token), token);
}

void tsnc_token_stream_free(struct tsnc_token_stream *tokens) {
  struct tsnc_token token;
  if (tokens == NULL)
    return;

  while (tsnc_vector_iter(&token, &tokens->tokenv,
      sizeof(struct tsnc_token))) {
    tsnc_token_free(&token);
  }
}
