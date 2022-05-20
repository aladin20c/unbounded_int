
// gcc -Wall main.c && ./a.out ex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.c"

/*--------------------------------------interpretation--------------------------------------*/



static void interpret(const char* source) {
  initScanner(source);

  Token t=scanToken();
  while (t.type!=TOKEN_EOF && t.type!=TOKEN_ERROR) {
    print_token(t);
    t=scanToken();
  }

  printf("\n----------src-------------\n");
  printf("%s\n",source);
}
/*-------------------------------------------------------------------------------------------*/


/*reading file*/
static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(74);
  }
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(74);
  }
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }
  buffer[bytesRead] = '\0';
  fclose(file);
  return buffer;
}

/*running program*/
static void runFile(const char* path) {
  char* source = readFile(path);
  interpret(source);
  free(source);
}


/*reading from stdin line by line*/
static void repl() {
  char line[1024];
  for (;;) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    interpret(line);
  }
}


/*-----------main-----------*/
int main(int argc, const char* argv[]) {
  /*if (argc == 0) {
    repl();
  } else if (argc == 2) {*/
    runFile(argv[1]);
  /*} else {
    fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
  }*/
  return 0;
}
