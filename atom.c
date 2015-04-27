
/* USE -lm */

/* Includes - BEGIN */
#include "atom.h"
#include "hash.h"
#include "assert.h"
#include <string.h>
#include <limits.h>
/* TODO - Use mem.h, replace malloc(), free() with ALLOC(), FREE() */
/* #include "mem.h" */
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h> /* offsetof */
/* Includes - END */


/* MACROS - BEGIN */
#define LONG_STR_LEN 43
#if 0
#define ATOM_BUCKETS 4096
#define ATOM_BUCKETS 4093
#define ATOM_BUCKETS 2048
#define ATOM_BUCKETS 2039
#endif
#define ATOM_BUCKETS 2048
/* MACROS - END */


/* DATA - BEGIN */
/* MUST EDIT Implementation of Atom_length After Editing this! */
static struct atom {
  struct atom *next;
  size_t len;
  char *str;
} *buckets[ATOM_BUCKETS];
static size_t entries = 0UL;
static size_t maxAtomLen = 0UL;
/* DATA - END */


const char *Atom_new( const char *str, size_t len ) {
  HASH_TYPE hash;
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
  entries++; /* Add 1 to Entry Count */

  if ( len > maxAtomLen )
    maxAtomLen = len;
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

/* Must be Given Actual Atom, not Random String */
size_t Atom_length( const char *str ) {
  struct atom *p; 
  struct atom *theAtom; 
  HASH_TYPE hash;

  assert(str);

  /* Magically Get the struct atom* from the const char* */
  /* p = ((struct atom*)(((char*) str)+1)) - 1; */
  /* theAtom = (struct atom*)(((char*)str)-sizeof(size_t)-sizeof(struct atom*)); */
  /* theAtom = (struct atom*)((char*)str+sizeof(p->str)) - 1; */
  /* theAtom = (struct atom*)((char*)str-sizeof(p->len)); */
  /* theAtom = (struct atom*)((char*)&str-offsetof(struct atom,str)); */ /* Close, but Need Address of str Within Atom, this just gives address of Copy of Pointer */
  theAtom = ((struct atom*)str) - 1; /* str in struct Atom Actually Points to Memory Directly After struct Atom's Internals, See Atom_new */


  /* Try to Catch Runtime Error / Bad Given Atom */
  assert(theAtom);
  assert( theAtom->len <= maxAtomLen );

  hash = doHash( theAtom->str, theAtom->len );
  hash %= ATOM_BUCKETS;
  for(p=buckets[ hash ]; p; p = p->next)
    if (p->str == str) return p->len;

  fprintf(stderr, "RUNTIME ERROR: Atom_length(): Argument 1 Must Be From Atom");
  assert(0);

  return 0U;
}
/* Functions - END */
/* ADDED FUNCTIONS */
size_t Atom_count() {
  return (size_t)entries;
}

size_t Atom_buckets() {
  return ATOM_BUCKETS;
}

size_t Atom_bucket_max() {
  struct atom *p;
  size_t i, n;

  n = 0;
  for( i=0; i < ATOM_BUCKETS; ++i )
    for( p=buckets[i]; p; p = p->next )
      ++n;

  return n;
}

double Atom_std_dev() {
  size_t i;
  struct atom *p;
  double dev = 0.0;
  double avg = ((double)entries) / ((double)ATOM_BUCKETS);
  double n;

  for( i=0; i<ATOM_BUCKETS; ++i ) {
    n = 0.0;
    for( p=buckets[i]; p; p = p->next )
      ++n;
    dev += pow((n-avg), 2.0);
  }

  return sqrt(dev/((double)entries));
}
/* ADDED FUNCTIONS - END */

