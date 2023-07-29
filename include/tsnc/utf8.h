#ifndef TSNC_UTF8_H
#define TSNC_UTF8_H
#include <stdlib.h>
#include <stdio.h>

int tsnc_utf8_getch(FILE *srcfp);
int tsnc_utf8_ungetch(int ch, FILE *srcfp);

#endif
