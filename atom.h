#ifndef ATOM_INCLUDED
#define ATOM_INCLUDED

#include <stddef.h>

extern        int	   Atom_length(const char *str);
extern const  char  *Atom_new(const char *str, size_t len);
extern const  char  *Atom_string(const char *str);
extern const  char  *Atom_int(long n);

#endif
