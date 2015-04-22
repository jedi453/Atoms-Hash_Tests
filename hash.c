
#include "hash.h"
#include "assert.h"
#include <limits.h>

HASH_TYPE Hash_FNV1A(const char*, size_t);
HASH_TYPE Hash_LOSE(const char*, size_t);
unsigned int Hash_MURMUR2(const char*, size_t);
unsigned int Hash_MURMUR3(const char*, size_t, unsigned int);
HASH_TYPE Hash_DJB2(const char*, size_t);
HASH_TYPE Hash_SDBM(const char*, size_t);

#define MURMUR3_SEED 0x93C467E3


HASH_TYPE doHash(const char* str, size_t len) {
#if 0
  return Hash_LOSE(str, len);
  return Hash_MURMUR2(str, len);
  return Hash_MURMUR3(str, len, MURMUR3_SEED);
  return Hash_FNV1A(str, len);
  return Hash_DJB2(str, len);
#endif
  return Hash_SDBM(str, len);
}



/* Functions - BEGIN */
/* FNV-1A Hash on Given String of Given Length 
 *  DON'T EXPOSE TO CLIENT! DON'T PUT IN atom.h
 */
HASH_TYPE Hash_FNV1A( const char *str, size_t len ) {
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

/** Lose-Lose - FOR TESTING ONLY - VERY SUCK */
HASH_TYPE Hash_LOSE( const char *str, size_t len ) {
  size_t i;
  HASH_TYPE hash = 0;

  for ( i=0; i<len; ++i )
    hash += str[i];

  return hash;
}


/****************** BEGIN Murmur2 Hash *******************/
/** Murmur 2 Mix Constants that Work Well, Thanks to Austin Appleby */
#define MURMUR2_MIX1  0x5BD1E995u
#define MURMUR2_MIX2  24

/** Murmur 2 Random Seed, floor ( 2^32 * gamma )
 *    See Here:
 *      http://en.wikipedia.org/wiki/Euler-Mascheroni_constant
 */
#define MURMUR2_SEED  0x93C467E3u


unsigned int Hash_MURMUR2( const char* str, size_t len )
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




/****************** BEGIN Murmur3 Hash *******************/
unsigned int Hash_MURMUR3(const char* str, size_t len, unsigned int seed) {
  unsigned int c1 =   0xCC9E2D51U;
  unsigned int c2 =   0x1B873593U;
  unsigned char r1 =  15u;
  unsigned char r2 =  13u;
  unsigned int m =    5u;
  unsigned int n =    0xE6546B64U;
  unsigned int hash = seed;
  const unsigned int* data;
  unsigned int tmp;  /* Temporary Holder for Intermediate Hashing Results */
  size_t wlen = len; /* The Working Length, For Main Loop, Full len Used Later*/

  /* Make Some (Probably Good) Assumptions About Integer Types,
   *  to Avoid Needing stdint.h and C99
   */
  assert(CHAR_BIT == 8);
  assert((sizeof(unsigned int)/sizeof(unsigned char)) == 4);

  /* Begin Hashing */
  for( data = (const unsigned int*)str; wlen >= 4; wlen -= 4, ++data ) {
    tmp = *data;

    tmp *= c1;
    tmp = (tmp << r1) | (tmp >> (32-r1)); /* Rotate tmp by r1 to Left */
    tmp *= c2;

    hash ^= tmp;
    hash = (hash << r2) | (hash >> (32-r2));
    hash = hash*m + n;
  }

  /* Reverse Byte order of Remaining Bytes
   *  Then Perform Regular Mix on it
   */
  tmp = 0u;
  switch (wlen) {
    case 3:
      tmp ^= (0x0000FF00u & *data) << 8;
    case 2:
      tmp ^= (0x00FF0000u & *data) >> 8;
    case 1:
      tmp ^= (0xFF000000u & *data) >> 24;

      /* Same Mix as Main Loop */
      tmp *= c1;
      tmp = (tmp<<r1) | (tmp>>(32-r1));
      tmp *= c2;

      hash ^= tmp;
      break;
  }

  /* Final Mixing */
  hash ^= len;
  hash ^= (hash >> 16);
  hash *= 0x85EBCA6B;
  hash ^= (hash >> 13);
  hash *= 0xC2B2AE35;
  hash ^= (hash >> 16);

  return hash;
}

/****************** END Murmur3 Hash *******************/


/****************** BEGIN DJB2 Hash ******************/
HASH_TYPE Hash_DJB2(const char *str, size_t len) {
  HASH_TYPE hash = 5381;
  size_t i;

  for( i=0; i<len; ++i )
    hash = ((hash<<5)+hash) + str[i];

  return hash;
}
/****************** END DJB2 Hash ******************/

/****************** BEGIN SDBM Hash ******************/
HASH_TYPE Hash_SDBM(const char* str, size_t len) {
  HASH_TYPE hash = 0;
  size_t i;

  for( i=0; i<len; ++i )
    hash = str[i] + (hash<<6) + (hash<<16) - hash;

  return hash;
}
/****************** END SDBM Hash ******************/
