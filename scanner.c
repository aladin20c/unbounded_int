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
  const char* start;
  int length;
  int line;
} Key;


void initScanner(const char* source);
void print(Key t);
Key getNext();


/*initialising scanner*/
typedef struct {
  const char* start;
  const char* current;
  int line;
} Scanner;


Scanner scanner;


void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}


void print(Key t){
  switch (t.type) {
    case 0:printf("{type: %s,length: %d, line:%d}   :","minus",t.length,t.line);break;
    case 1:printf("{type: %s,length: %d, line:%d}   :","plus",t.length,t.line);break;
    case 2:printf("{type: %s,length: %d, line:%d}   :", "slash",t.length,t.line);break;
    case 3:printf("{type: %s,length: %d, line:%d}   :", "percentage",t.length,t.line);break;
    case 4:printf("{type: %s,length: %d, line:%d}   :", "star",t.length,t.line);break;
    case 5:printf("{type: %s,length: %d, line:%d}   :", "equal",t.length,t.line);break;
    case 6:printf("{type: %s,length: %d, line:%d}   :","variable",t.length,t.line);break;
    case 7:printf("{type: %s,length: %d, line:%d}   :","number",t.length,t.line);break;
    case 8:printf("{type: %s,length: %d, line:%d}   :", "print",t.length,t.line);break;
    case 9:printf("{type: %s,length: %d, line:%d}   :", "Error",t.length,t.line);break;
    case 10:printf("{type: %s,length:%d, line:%d}   \\0:", "end",t.length,t.line);break;
  }
  for(int i=0;i<t.length;i++){
    printf("%c", t.start[i]);
  }
  printf("\n");
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
  if(e==' '||e=='\r'||e=='\t'||e=='\n'|| e=='\0' || (type==VARIABLE && e=='=')){
    return makeKey(type);
  }else{
    return makeKey(ERROR);
  }
}


static Key nombre() {
  while (chiffre(peek())) avance();
  char e=peek();
  if(e==' '||e=='\r'||e=='\t'||e=='\n' || e=='\0'){
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
  return makeKey(ERROR);
}

#endif
