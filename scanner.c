//> Scanning on Demand scanner-c
#ifndef scanner_c
#define scanner_c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
  MINUS, PLUS,SLASH, PERCENTAGE ,STAR,EQUAL,
  VARIABLE,NUMBER,
  PRINT,
  ERROR, END
} Type;


typedef struct {
  Type type;
  char* start;
  int length;
  int line;
} Key;


void initScanner(const char* source);
void print(Key t);
Key getNext();


/*initialising scanner*/
typedef struct {
  char* start;
  char* current;
  int line;
} Scanner;


Scanner scanner;


void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}


void print(Key t){
  printf("Type: ");
  switch (t.type) {
    case 0:printf("%s\n","minus" );break;
    case 1:printf("%s\n","plus" );break;
    case 2:printf("%s\n", "slash");break;
    case 3:printf("%s\n", "percentage");break;
    case 4:printf("%s\n", "star");break;
    case 5:printf("%s\n", "equal");break;
    case 6:printf("%s\n","variable" );break;
    case 7:printf("%s\n","number" );break;
    case 8:printf("%s\n", "print");break;
    case 9:printf("%s\n", "Error");break;
    case 10:printf("%s\n", "end");break;
  }
  printf("Length: %d\n", t.length);
  printf("Line: %d\n", t.line);
  for(int i=0;i<t.length;i++){
    printf("%c", t.start[i]);
  }
  printf("\n-----------------------------------------------------------------------\n");
}




static int lettre(char c) {
  return (c>='a' && c<='z')||(c>='A' && c<='Z');
}

static int chiffre(char c) {
  return c>='0' && c<='9';
}

static int fin() {
  return *scanner.current == '\0';
}


static char avance() {
  scanner.current++;
  return scanner.current[-1];
}

static char peek() {
  return *scanner.current;
}

static char peekNext() {
  if (fin()) return '\0';
  return scanner.current[1];
}

static int match(char expected) {
  if (fin()) return 0;
  if (*scanner.current != expected) return 0;
  scanner.current++;
  return 1;
}



static Key makeKey(Type type) {
  Key key;
  key.type = type;
  key.start = scanner.start;
  key.length = (int)(scanner.current - scanner.start);
  key.line = scanner.line;
  return key;
}



static void skip() {
  while (1) {
    char c = peek();
    if(c==' '||c=='\r'||c=='\t'){
      avance();
    }else if(c=='\n'){
      scanner.line++;avance();
    }else{
      return;
    }
  }
}





static Type checkKeyword(int start, int length,const char* rest, Type type) {
  if (scanner.current - scanner.start == start + length &&
      memcmp(scanner.start + start, rest, length) == 0) {
    return type;
  }
  return VARIABLE;
}


static Key variable() {
  while (lettre(peek())) avance();
  Type type=VARIABLE;

  if(scanner.start[0]=='p'){
    type=checkKeyword(1, 4, "rint", PRINT);
  }

  char e=peek();
  if(e==' '||e=='\r'||e=='\t'||e=='\n'|| (type==VARIABLE && e=='=')){
    return makeKey(type);
  }else{
    return makeKey(ERROR);
  }

}


static Key nombre() {
  while (chiffre(peek())) avance();
  char e=peek();
  if(e==' '||e=='\r'||e=='\t'||e=='\n'){
    return makeKey(NUMBER);
  }else{
    return makeKey(ERROR);
  }
}




Key getNext(){
  skip();
  scanner.start = scanner.current;
  if (fin()) return makeKey(END);
  char c = avance();
  if (lettre(c)) return variable();
  if (chiffre(c)) return nombre();

  char e=peek();
  if(c=='='){
    return makeKey(EQUAL);
  }else if(c=='-'){
    if(e==' '||e=='\r'||e=='\t') return makeKey(MINUS);
    if(chiffre(e)) return nombre();
  }else if(c=='+'){
    if(e==' '||e=='\r'||e=='\t') return  makeKey(PLUS);
    if(chiffre(e)) return nombre();
  }else if(c=='/'){
    if(e==' '||e=='\r'||e=='\t') return makeKey(SLASH);
  }else if(c=='%'){
    if(e==' '||e=='\r'||e=='\t') return makeKey(PERCENTAGE);
  }else if(c=='*'){
    if(e==' '||e=='\r'||e=='\t') return makeKey(STAR);
  }else{
    return makeKey(ERROR);
  }
}

#endif
