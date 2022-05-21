#ifndef unbounded_int_c
#define unbounded_int_c

// gcc -Wall calc_unbounded_int.c && ./a.out ex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.c"
#include "keys.c"
#include "values.c"
/*--------------------------------------interpretation--------------------------------------*/


static void analyse(char* source,Key *point1,Key *point2,size_t length) {
  if(length==5){

    //variable = entier_ou_variable op entier_ou_variable
    if(point1[0].type!=VARIABLE || point1[1].type!=EQUAL || (point1[2].type!=VARIABLE && point1[2].type!=NUMBER) || (point1[3].type!=MINUS && point1[3].type!=PLUS && point1[3].type!=STAR) || (point1[4].type!=VARIABLE && point1[4].type!=NUMBER)){
        fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
        free(source);
        freeValueMap();
        freeKeyArray();
        exit(EXIT_FAILURE);
    }



  }else if(length==3){

    //variable = entier
    if(point1[0].type!=VARIABLE || point1[1].type!=EQUAL || point1[2].type!=NUMBER){
        fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
        free(source);
        freeValueMap();
        freeKeyArray();
        exit(EXIT_FAILURE);
    }


  }else if(length==2){
    //print variable
    if(point1[0].type!=PRINT || point1[1].type!=VARIABLE){
        fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
        free(source);
        freeValueMap();
        freeKeyArray();
        exit(EXIT_FAILURE);
    }

  }else{
      fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
      free(source);
      freeValueMap();
      freeKeyArray();
      exit(EXIT_FAILURE);
  }
}



static void interpret(const char* source) {
  initScanner(source);
  initKeyArray();
  initValueMap();
  Key t;
  do{
    t=getNext();
    if(t.type==ERROR){
      fprintf(stderr, "Wrong syntaxe at line %d\n", t.line);
      free((char*)source);
      freeValueMap();
      freeKeyArray();
      exit(EXIT_FAILURE);
    }
    addKey(t);
  }while (t.type!=END && t.type!=ERROR);
  //analysing
  Key* point1=array.keys;
  Key* point2=array.keys;
  while (point1->type!=END) {
    while (point1->line==point2->line && point2->type!=END ) {
      point2+=1;
    }
    size_t length=point2-point1;
    analyse((char*)source,point1,point2,length);
    point1=point2;
  }
  for(int i=0;i<array.count;i++){
    print(array.keys[i]);
  }
  freeValueMap();
  freeKeyArray();
}



/*-------------------------------------------------------------------------------------------*/

/*reading file*/
static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(EXIT_FAILURE);
  }
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(EXIT_FAILURE);
  }
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    free(buffer);
    exit(EXIT_FAILURE);
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
#endif
