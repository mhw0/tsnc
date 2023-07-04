#ifndef TSNC_VECTOR_H
#define TSNC_VECTOR_H
#include <stdlib.h>
#include <string.h>

/* TODO: isn't this too much? */
#define TSNC_VECTOR_INITIAL_CAPACITY 4194304

/* holds vector fields */
struct tsnc_vector {
  /* points to start of internal buffer */
  void *buf;
  /* vector capacity */
  size_t cap;
  /* vector cursor */
  size_t curs;
  /* vector iterator cursor */
  size_t icurs;
};

int tsnc_vector_create(struct tsnc_vector *dest);
int tsnc_vector_iter(void *dest, struct tsnc_vector *vector,
    size_t size);
int tsnc_vector_at(void *dest, struct tsnc_vector *vector,
		size_t size, size_t index);
int tsnc_vector_push(struct tsnc_vector *vector,
    size_t size, void *data);
int tsnc_vector_size(struct tsnc_vector *vector, size_t size);

#endif
