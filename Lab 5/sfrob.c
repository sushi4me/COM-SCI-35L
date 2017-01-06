#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// The ASCII for [space] is 32
int frobcmp(char const *a, char const *b) {
  // Check if the two are either equal or substrings
  while ((*a != ' ') && (*b != ' ')) {
    // frob is XOR with decimal 42
    if ((*a^42) > (*b^42)) {
      return 1;
    }
    else if ((*a^42) < (*b^42)) {
      return -1;
    }
    a++; b++;
  }
  // A is a substring of B
  if ((*a == ' ') && (*b != ' ')) {
    return -1;
  }
  // B is a substring of A
  else if ((*a != ' ') && (*b == ' ')) {
    return 1;
  }
  // The two are equal
  else
    return 0;
}

// Wrapper function for frobcmp
int frobcmp_wrapper(const void *a, const void *b) {
  char const *firstWord = *(char const **)a;
  char const *secondWord = *(char const **)b;
  return frobcmp(firstWord, secondWord);
}

int main() {
  int allocate_size = 1000, wordCount = 0, char_get;
  char *arr = (char *)malloc(sizeof(char)*allocate_size);

  if(arr == NULL) {
    fprintf(stderr,"arr malloc failed");
    exit(1);
  }
  
  int i;
  for(i = 0; (char_get = getchar()) != EOF; ++i) {
    // Check if you have enough space
    if(i == allocate_size) {
      arr = (char *)realloc(arr, allocate_size*2);
      allocate_size  *= 2;

      if (arr == NULL) {
	fprintf(stderr, "realloc failed");
	exit(1);
      }
    }
      
    // Start of a new word
    if((char)char_get == ' ') {
      wordCount++;
    }

    arr[i] = (char)char_get;
  }

  // Add a [space] character at the end
  if(arr[i] != ' ') {
    wordCount++;
    arr[i] = ' ';
  }

  int arraySize = i;

  if (ferror(stdin)) {
    fprintf(stderr, "failed to read from stdin");
    exit(1);
  }

  char **ptrToWord = (char**)malloc(sizeof(char*)*wordCount);

  if (ptrToWord == NULL) {
      fprintf(stderr, "malloc failed");
      exit(1);
  }

  // Store start of word in array of pointers
  char *ptrWord = arr; 
  int j;
  for (j = 0; j < wordCount; j++) {
    ptrToWord[j] = ptrWord;

    if (j == (wordCount - 1)) {
      break;
    }
    
    while(*ptrWord != ' ') {
      ptrWord++;
    }
    ptrWord++;
  }

  qsort(ptrToWord, wordCount, sizeof(char*), frobcmp_wrapper);

  // Output the frob words
  char *ptrGet;
  int k;
  for (k = 0; k < wordCount; ++k) {
      ptrGet = ptrToWord[k];
      putchar(*ptrGet);
      while (*ptrGet != ' ') {
	ptrGet++;
	putchar(*ptrGet);
      }
  }

  if (ferror(stdout)) {
    fprintf(stderr, "putchar failed");
  }

  free(arr);
  free(ptrToWord);
}
