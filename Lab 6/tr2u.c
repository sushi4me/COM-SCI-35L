#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *charA = argv[1];
  char *charB = argv[2];

  // Check number of arguments
  if (argc > 3) {
    fprintf(stderr, "ERROR: Too many arguments\n");
    exit(1);
  }
  else if (argc == 1) {
    fprintf(stderr, "ERROR: Not enough arguments\n");
    exit(1);
  }
  else if (argc == 2) {
    fprintf(stderr, "ERROR: Missing file\n");
    exit(1);
  }

  int CONST = 1;
  int a, b;
	
  // Make sure they are not attempting to map one letter to many
  for(a = 0; a < strlen(argv[CONST]); a++)
    for(b = a+1; b < strlen(argv[CONST]); b++)
      if(argv[CONST][a] == argv[CONST][b]) {
	fprintf(stderr, "ERROR: You're mapping one letter to many\n");
	exit(1);
      }
	
  // Check argv for same length
  int i = 0;
  while(charA[i] != '\0' && charB[i] != '\0')
    i++;
  if((charA[i] != '\0' && charB[i] == '\0') || (charA[i] == '\0' && charB[i] != '\0')) {
    fprintf(stderr, "ERROR: Arguments must be the same length");
    exit(1);
  }

  // Replace characters in the file
  char buffer;
  while(read(0, &buffer, 1) > 0) {
    int k = 0;
    // Keep checking the character you got from the file with argv[1]
    while(charA[k] != '\0') {
      if(buffer == charA[k]) {
	buffer = charB[k];
	break;
      }
      k++;
    }
    // We replace putchar() here
    write(1, &buffer, 1);
  }
}
