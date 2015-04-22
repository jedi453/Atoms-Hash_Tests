
#include "hash.h"
#include "assert.h"
#include <limits.h>


/* Functions - BEGIN */
/* FNV-1A Hash on Given String of Given Length 
 *  DON'T EXPOSE TO CLIENT! DON'T PUT IN atom.h
 */
HASH_TYPE doHash( const char *str, size_t len ) {
  HASH_TYPE hash = 0x811C9DC5UL;
  HASH_TYPE prime = 0x01000193UL;
  unsigned char buf;
  size_t i;

  assert(str); /* Not Necessary - hash() not Provided to Client Code */
  /* Assume 32-bit unsigned long, but Use 64-bit if Detected */
  if ( (sizeof(HASH_TYPE)*CHAR_BIT) == 64 ) {
    hash = 14695981039346656037UL;
    prime = 0x100000001b3UL;
  }
  for ( i=0; i<len; ++i ) {
    buf = str[i];
#if CHAR_BIT != 8
    buf = buf & 0xFF;
#endif
    hash ^= buf;
    hash *= prime;
  }

  return hash;
}

#if 0
/** TODO - REMOVE TEST - Lose-lose */
HASH_TYPE doHash( const char *str, size_t len ) {
  size_t i;
  HASH_TYPE hash = 0;

  for ( i=0; i<len; ++i )
    hash += str[i];

  return hash;
}
#endif
