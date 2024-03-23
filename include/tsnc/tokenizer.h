#ifndef TSNC_TOKENIZER_H
#define TSNC_TOKENIZER_H
#include <tsnc/token.h>
#include <tsnc/source.h>

enum tsnc_token_kind tsnc_tokenize_next(struct tsnc_token *dest, struct tsnc_source *source);

#endif
