#ifndef ATOM_INCLUDED
#define ATOM_INCLUDED

#include <stddef.h>

/* All Functions Requiring const char* (Except Atom_new) must be given an Atom from 
 * Atom_new, Atom_string or Atom_int, not a Custom Defined String
 */
extern        size_t Atom_length(const char *str);
extern const  char  *Atom_new(const char *str, size_t len);
extern const  char  *Atom_string(const char *str);
extern const  char  *Atom_int(long n);

/* Added Functionality */
extern	      size_t Atom_count();
extern	      size_t Atom_buckets();
extern	      size_t Atom_bucket_max();
extern	      double Atom_std_dev();

#endif
