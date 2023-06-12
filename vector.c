#include <assert.h> // assert
#include <stdlib.h> // size_t
#include <string.h> // memcpy
#include "vector.h"

#define TSNC_VECTOR_INIT_SIZE 4194304

void tsnc_vector_init(struct tsnc_vector *vec) {
  vec->data = malloc(TSNC_VECTOR_INIT_SIZE);
  assert(vec->data && "Unable to allocate memory for vector");

  vec->size = TSNC_VECTOR_INIT_SIZE;
  vec->curs = 0;
}

void tsnc_vector_add(struct tsnc_vector *vec, void *data, size_t size) {
  // TODO: check if we have enough memory space
  memcpy(&vec->data[vec->curs], data, size);
  vec->curs += size;
  vec->icurs = 0;
}

void *tsnc_vector_iter(struct tsnc_vector *vec, size_t size) {
  void *ret;

  if (vec->icurs * size < vec->curs) {
    ret = &(vec->data[vec->icurs * size]);
    vec->icurs++;
    return ret;
  }

  vec->icurs = 0;
  return NULL;
}

void tsnc_vector_free(struct tsnc_vector *vec) {
  free(vec->data);
}
