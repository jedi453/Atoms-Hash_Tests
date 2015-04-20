

/* Includes - BEGIN */
#include "atom.h"
#include "assert.h"
#include <string.h>
#include <limits.h>
/* TODO - Use mem.h, replace malloc(), free() with ALLOC(), FREE() */
/* #include "mem.h" */
#include <stdlib.h>
#include <stddef.h>
/* Includes - END */


/* MACROS - BEGIN */
#define LONG_STR_LEN 43
#define ATOM_BUCKETS 2048
/* MACROS - END */


/* DATA - BEGIN */
static struct atom {
  struct atom *next;
  size_t len;
  char *str;
} *buckets[ATOM_BUCKETS];
/* DATA - END */


/* Functions - BEGIN */
/* FNV-1A Hash on Given String of Given Length 
 *  DON'T EXPOSE TO CLIENT! DON'T PUT IN atom.h
 */
unsigned long doHash( const char *str, size_t len ) {
  unsigned long hash = 0x811C9DC5UL;
  unsigned long prime = 0x01000193UL;
  unsigned char buf;
  size_t i;

  assert(str); /* Not Necessary - hash() not Provided to Client Code */
  /* Assume 32-bit unsigned long, but Use 64-bit if Detected */
  if ( (sizeof(unsigned long)*CHAR_BIT) == 64 ) {
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

const char *Atom_new( const char *str, size_t len ) {
  unsigned long hash;
  size_t i;
  struct atom *p;

  assert(str);
  assert(len >= 0);
  hash = doHash( str, len );
  hash %= ATOM_BUCKETS;

  /* Compare Strings with Equal Hash, Returning if Found */
  for (p = buckets[hash]; p; p = p->next) {
    if ( len == p->len ) {
      for (i=0; i < len && p->str[i] == str[i]; )
        ++i;
      if ( i == len )
        return p->str;
    }
  }

  /* No Equal String Found, Allocate new Entry */
  /* TODO - ALLOCATE NEW ENTRY */
  p = (struct atom*)malloc(sizeof(struct atom) + len+1);
  p->len = len;
  p->str = (char*)(p+1);      /* Get Location of Extra Memory */
  if ( len > 0 )
    memcpy(p->str, str, len); /* Copy Full String to p, if len > 0 */
  p->str[len] = '\0';         /* Null Terminate p->str */
  p->next = buckets[hash];
  buckets[hash] = p;

  return p->str;
}

const char *Atom_string(const char *str) {
  assert(str);
  return Atom_new(str, strlen(str));
}

const char *Atom_int(long n) {
  char str[LONG_STR_LEN]; /* Buffer to Hold Result */
  char *s = str + LONG_STR_LEN; /* Working char, Start at End */
  unsigned long m;

  /** Convert n to a Representing unsigned long, m
   *   This allows for Standardized Results from Division and Modulus
   *   Note that -LONG_MIN can not be held by a signed long,
   *    -> So m = -n isn't valid for n = LONG_MIN
   */
  if ( n == LONG_MIN )
    m = LONG_MAX + 1UL;
  else if ( n < 0 )
    m = -n;
  else
    m = n;

  do
    *--s = m%10 + '0';
  while ((m /= 10) > 10);

  if ( n < 0 )
    *--s = '-';

  return Atom_new(s, (str + LONG_STR_LEN) - s);
}
/* Functions - END */

