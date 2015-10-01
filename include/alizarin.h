#ifndef ALIZARIN_H
#define ALIZARIN_H

#include <stddef.h>

void *mallocOrDie(size_t size);
void *reallocOrDie(void *old, size_t size);

#endif	// ALIZARIN_H
