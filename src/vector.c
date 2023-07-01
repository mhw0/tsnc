#include <tsnc/vector.h>

int tsnc_vector_create(struct tsnc_vector *dest) {
  void *buf;
  if (dest == NULL)
    return 0;

  buf = malloc(TSNC_VECTOR_INITIAL_CAPACITY);
  if (buf == NULL)
    return 0;

  dest->buf = buf;
  dest->cap = TSNC_VECTOR_INITIAL_CAPACITY;
  dest->curs = 0;
  dest->icurs = 0;
  return 1;
}

int tsnc_vector_push(struct tsnc_vector *vector,
    size_t size, void *data) {

	if (vector == NULL || data == NULL)
		return 0;

  // TODO: check if we have enough memory space
  memcpy(((uint8_t*)vector->buf) + vector->curs, data, size);
  vector->curs += size;
	return 1;
}

void *tsnc_vector_iter(struct tsnc_vector *vector, size_t size) {
  void *ret;

  if (vector->icurs * size < vector->curs) {
		ret = ((uint8_t*)vector->buf) + vector->icurs * size;
    vector->icurs++;
    return ret;
  }

  vector->icurs = 0;
  return NULL;
}

int tsnc_vector_at(void *dest, struct tsnc_vector *vector,
		size_t size, size_t index) {
	if (dest == NULL || vector == NULL)
		return 0;

	memcpy(dest, ((uint8_t*)vector->buf) + (index * size), size);
	return 1;
}

int tsnc_vector_size(struct tsnc_vector *vector, size_t size) {
  if (vector == NULL)
    return 0;

  return vector->curs / size;
}
