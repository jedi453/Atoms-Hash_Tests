

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "atom.h"

#include "hash.h"
#include <string.h>

#define BUF_LEN 50
#if 0
#define WORDS_FILE "wordsEn.txt"
#define WORDS_FILE "word-list.txt"

#define DEBUG_HASH_TESTS
#endif
#define WORDS_FILE "aspell-word-list-en-95.txt"

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
  char *strs[] = {  "", "test", "Test", "teSt", "tesT",
                    "this is a sentence" };
  FILE *inFile;
  size_t i;


  inFile = fopen( WORDS_FILE, "r" );
  assert(inFile);

  addWords(inFile);

  printf("Number of Buckets:                      %lu\n", Atom_buckets());
  printf("Number of Elements Added:               %d\n", word_count);
  printf("Number of Elements in Atom List:        %lu\n", Atom_count());
  printf("Average Number of Elements Per Bucket:  %g\n", 
            ((double)Atom_count())/((double)Atom_buckets()));
  printf("Standard Deviation of Elements/Bucket:  %g\n", Atom_std_dev());

#ifdef DEBUG_HASH_TESTS
  /* Test Some Hashes */
  printf("%20s | %20s:\n", "Text", "Hash");
  for ( i=0; i < sizeof(strs)/sizeof(char*); ++i ) {
    printf("%20s | %20lx\n", strs[i], doHash(strs[i], strlen(strs[i])));
  }
#endif

  return 0;
}
