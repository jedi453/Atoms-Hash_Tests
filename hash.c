
#include "hash.h"
#include "assert.h"
#include <limits.h>


#if 0
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
#endif

#if 0
/** Lose-Lose - FOR TESTING ONLY - VERY SUCK */
HASH_TYPE doHash( const char *str, size_t len ) {
  size_t i;
  HASH_TYPE hash = 0;

  for ( i=0; i<len; ++i )
    hash += str[i];

  return hash;
}
#endif

/****************** BEGIN Murmur2 Hash *******************/
/** Murmur 2 Mix Constants that Work Well, Thanks to Austin Appleby */
#define MURMUR2_MIX1  0x5BD1E995u
#define MURMUR2_MIX2  24

/** Murmur 2 Random Seed, floor ( 2^32 * gamma )
 *    See Here:
 *      http://en.wikipedia.org/wiki/Euler-Mascheroni_constant
 */
#define MURMUR2_SEED  0x93C467E3u


unsigned int doHash( const char* str, size_t len )
{
  assert(sizeof(unsigned int)*CHAR_BIT == 32);
  unsigned int hash;

  
  /** Set Initial Values */
  hash = MURMUR2_SEED ^ len; /**< Start off with Pseudo-Random Number */

  /** Hash 4 Bytes at a Time */
  while ( len >= 4 )
  {
    unsigned int tmp = *(unsigned int*)str;

    tmp *= MURMUR2_MIX1;
    tmp ^= tmp >> MURMUR2_MIX2;
    tmp *= MURMUR2_MIX1;

    hash *= MURMUR2_MIX1;
    hash ^= tmp;

    str += 4;
    len -= 4;
  }

  /** Handle Remaining Bytes ( if Len wasn't Divisible by 4 ) */
  /** Note there are no break Statements, eg. if len==3, all statements execute */
  /** Things aren't Really Mixed Well Here, so There's More Later */
  switch( len )
  {
    case 3: hash ^= str[2] << 16;
    case 2: hash ^= str[1] << 8;
    case 1: hash ^= *str;
            hash ^= MURMUR2_MIX1;
  };

  /** You can Never Overmix a Hash!  MOAR MIXIN' */
  /** Incase len wasn't Divisible by 4, Make Sure Everything is Mixed well */
  hash ^= hash >> 13;
  hash *= MURMUR2_MIX1;
  hash ^= hash >> 15;

  return hash;
}
/****************** END Murmur2 Hash *******************/
