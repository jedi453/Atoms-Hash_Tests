
#ifndef HASH_INCLUDED
#define HASH_INCLUDED

#include <stddef.h>

#if 0
#define HASH_TYPE size_t
#endif
#define HASH_TYPE unsigned int

HASH_TYPE doHash(const char* str, size_t len);

#endif
