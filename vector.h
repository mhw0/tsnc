#ifndef TSNC_VECTOR_H
#define TSNC_VECTOR_H

struct tsnc_vector {
  void *data;
  size_t size;
  size_t curs;
  size_t icurs;
};

void tsnc_vector_init(struct tsnc_vector *vec);
void tsnc_vector_add(struct tsnc_vector *vec, void *data, size_t size);
void *tsnc_vector_iter(struct tsnc_vector *vec, size_t size);
void tsnc_vector_free(struct tsnc_vector *vec);

#endif
