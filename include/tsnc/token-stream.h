#ifndef TSNC_TOKEN_STREAM_H
#define TSNC_TOKEN_STREAM_H
#include <tsnc/vector.h>
#include <tsnc/token.h>

/* holds token stream fields */
struct tsnc_token_stream {
  /* token vector */
  struct tsnc_vector tokenv;
  /* cursor */
  size_t curs;
};

void tsnc_token_stream_init(struct tsnc_token_stream *tokens);
size_t tsnc_token_stream_size(struct tsnc_token_stream *tokens);
int tsnc_token_stream_peek(struct tsnc_token *dest,
    struct tsnc_token_stream *tokens);
int tsnc_token_stream_next(struct tsnc_token *dest,
    struct tsnc_token_stream *tokens);
int tsnc_token_stream_add(struct tsnc_token_stream *tokens,
    struct tsnc_token *token);
void tsnc_token_stream_free(struct tsnc_token_stream *tokens);

#endif
