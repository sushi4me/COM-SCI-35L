#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

int frobcmp_count = 0;

// The ASCII for [space] is 32
int frobcmp(char const *a, char const *b) {
  frobcmp_count++;
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
  int allocate_size = 1000, wordCount = 0, i = 0,  char_get;
  char *arr;

  struct stat fileInfo;
  fstat(0, &fileInfo);
  int file_size = fileInfo.st_size;

  if(!S_ISREG(fileInfo.st_mode)) {
    arr = (char *)malloc(sizeof(char)*allocate_size);
  
    if(arr == NULL) {
      fprintf(stderr,"ERROR: malloc allocate_size failed");
      exit(1);
    }
  
    while(read(0, &char_get, 1) > 0) { 
      // Check if you have enough space
      if(i == allocate_size) {
	arr = (char *)realloc(arr, allocate_size*2);
	allocate_size  *= 2;

	if (arr == NULL) {
	  fprintf(stderr, "ERROR: realloc allocate_size failed");
	  exit(1);
	}
      }
      
      // Start of a new word
      if((char)char_get == ' ')
	wordCount++;

      arr[i] = (char)char_get;
      i++;
    }
  }
  else if(S_ISREG(fileInfo.st_mode)) {
    arr = (char *)malloc(sizeof(char)*file_size);

    if(arr == NULL) {
      fprintf(stderr, "ERROR: maalloc file_size faled");
      exit(1);
    }
    // Recall that /proc/self/maps does not give a valid size!
    // Therefore, multiplying by 2 (which will still be zero) is incorrect.
    while(read(0, &char_get, 1) > 0) {
      if(i == file_size) {
	arr = (char *)realloc(arr, file_size + 1);
	file_size += 1;
	
	if(arr == NULL) {
	  fprintf(stderr, "ERROR: realloc file_size failed");
	  exit(1);
	}
      }

      if((char)char_get == ' ')
	wordCount++;

      arr[i++] = (char)char_get;
    }
  }

  // Add a [space] character at the end
  if(arr[i] != ' ') {
    wordCount++;
    arr[i] = ' ';
  }

  int arraySize = i;

  if (ferror(stdin)) {
    fprintf(stderr, "ERROR: failed to read from stdin");
    exit(1);
  }

  char **ptrToWord = (char**)malloc(sizeof(char*)*wordCount);

  if (ptrToWord == NULL) {
    fprintf(stderr, "ERROR: malloc wordCount failed");
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
      write(1, &(*ptrGet), 1);
      while (*ptrGet != ' ') {
	ptrGet++;
	write(1, &(*ptrGet), 1);
      }
  }

  if (ferror(stdout)) {
    fprintf(stderr, "ERROR: write failed");
  }

  free(arr);
  free(ptrToWord);
  fprintf(stderr, "Comparisons: %d\n", frobcmp_count);
}
