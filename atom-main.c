

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "atom.h"

#define BUF_LEN 50

static int word_count = 0;

/**
 * Get a Line from stream, up to max chars, store in str
 *
 * @param[out] str  char* - The Output Location
 * @param[in] max  int - The Maximum Length Allowed
 * @param[in] stream  FILE* - The File to Read From
 *
 * @return char* - str
 *
 * @TODO - Return NULL if \n isn't Found ?
 */
char *fgetln( char* str, int max, FILE* stream ) {
  size_t i = strlen(str)-1;
  fgets(str, max, stream);
  if (str[i] == '\n')
    str[i] = '\0';
  return str;
}


/** 
 * Adds Lines from Given FILE into atom list
 */
void addWords(FILE* inFile) {
  char buf[BUF_LEN];

  while ( fgets(buf, BUF_LEN, inFile ) ) {
    Atom_string(buf);
    word_count++;
  }
}

int main(void) {
  FILE *inFile;

  inFile = fopen( "word-list.txt", "r" );
  assert(inFile);

  addWords(inFile);

  printf("Number of Elements Added:               %d\n", word_count);
  printf("Number of Elements in Atom List:        %lu\n", Atom_count());
  printf("Average Number of Elements Per Bucket:  %g\n", 
            ((double)Atom_bucket_max())/((double)Atom_count()));
  printf("Standard Deviation of Elements/Bucket:  %g\n", Atom_std_dev());

  return 0;
}
