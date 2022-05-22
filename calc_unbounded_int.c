// gcc -Wall calc_unbounded_int.c && ./a.out -i ex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unbounded_int.c"


/*-----------------------------------------------------------Scanner------------------------------------------------------------*/


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

/*initialising scanner*/
typedef struct {
  const char* start;
  const char* current;
  int line;
} Scanner;


Scanner scanner;


static void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}


static void print(Key t){
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




static int estLettre(char c) {
  return (c>='a' && c<='z')||(c>='A' && c<='Z');
}

static int estChiffre(char c) {
  return c>='0' && c<='9';
}

static int estFin() {
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
  while (estLettre(peek())) avance();
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
  while (estChiffre(peek())) avance();
  char e=peek();
  if(e==' '||e=='\r'||e=='\t'||e=='\n' || e=='\0'){
    return makeKey(NUMBER);
  }else{
    return makeKey(ERROR);
  }
}




static Key getNext(){
  skip();
  scanner.start = scanner.current;
  if (estFin()) return makeKey(END);
  char c = avance();
  if (estLettre(c)) return variable();
  if (estChiffre(c)) return nombre();

  char e=peek();
  if(c=='='){
    return makeKey(EQUAL);
  }else if(c=='-'){
    if(e==' '||e=='\r'||e=='\t') return makeKey(MINUS);
    if(estChiffre(e)) return nombre();
  }else if(c=='+'){
    if(e==' '||e=='\r'||e=='\t') return  makeKey(PLUS);
    if(estChiffre(e)) return nombre();
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

/*-----------------------------------------------------------aux------------------------------------------------------------*/

static void free_unbounded_int_(unbounded_int ui){
    chiffre *tmp=ui.premier;
    if(ui.premier==NULL){
      return;
    }
    if(ui.premier==ui.dernier){
        free(ui.premier);
        ui.premier=NULL;
        return;
    }
    while (tmp!=NULL) {
      chiffre *tmp2=tmp;
      tmp=tmp->suivant;
      free(tmp2);
    }
    ui.premier=NULL;
}

static void afficher_unbounded_int_(unbounded_int ui){
  if(ui.premier==NULL){
    printf("\n");return;
  }
  printf("%c",ui.signe);
  if(ui.premier==ui.dernier){
    printf("%c\n",(ui.premier)->c);
    return;
  }
  chiffre *tmp=ui.premier;
  while (tmp!=NULL) {
    printf("%c",tmp->c);
    tmp=tmp->suivant;
  }
  printf("\n");
}

static int min_(Key k1,Key k2){
  if(k2.length<k1.length){
    return k2.length;
  }
  return k1.length;
}

/*-----------------------------------------------------------ArrayList------------------------------------------------------------*/

typedef struct {
  int capacity;
  int count;
  Key* keys;
} KeyArray;

KeyArray array;

static void initKeyArray() {
  array.keys = NULL;
  array.capacity = 0;
  array.count = 0;
}

static void freeKeyArray() {
  free(array.keys);
  initKeyArray();
}

static void addKey(Key key){
  if(array.keys==NULL){
    array.keys=(Key*)malloc(2 * sizeof(Key));
    array.capacity=2;
  }
  if (array.capacity <= array.count + 1) {
    int oldCapacity = array.capacity;
    int newCapacity = oldCapacity * 2;
    array.keys = (Key*)realloc(array.keys,newCapacity*sizeof(Key));
    array.capacity =newCapacity;
  }
  array.keys[array.count] = key;
  array.count++;
}


/*-----------------------------------------------------------Map------------------------------------------------------------*/

typedef struct{
  int count; /* longueur de la liste */
  int capacity; /* longueur de la liste */
  Key *keys;/* pointeur vers le premier élément des cles*/
  unbounded_int *values; /*pointeur vers le premier élément des valeurs*/
} ValueMap;

ValueMap map;


static void initValueMap() {
  map.keys=NULL;
  map.values = NULL;
  map.count = 0;
  map.capacity=0;
}

static void freeValueMap() {
  free(map.keys);
  for(int i=0;i<map.count;i++){
    free_unbounded_int_(map.values[i]);
  }
  free(map.values);
  initValueMap();
}

static void push(Key key,unbounded_int value) {
  if(map.keys==NULL){
    map.keys=(Key*)malloc(8 * sizeof(Key));
    map.values=(unbounded_int*)malloc(8 * sizeof(unbounded_int));
    map.capacity=8;
  }

  //research value
  for(int i=0;i<map.count;i++){
    if(strncmp(map.keys[i].start,key.start,min_(map.keys[i],key))==0){
      free_unbounded_int_(map.values[i]);
      map.values[i].premier = value.premier;
      map.values[i].dernier = value.dernier;
      map.values[i].signe = value.signe;
      map.values[i].len = value.len;
      return;
    }
  }
  if (map.capacity <= map.count + 1) {
    int oldCapacity = map.capacity;
    int newCapacity = oldCapacity * 2;
    map.keys = (Key*)realloc(map.keys,newCapacity*sizeof(Key));
    map.values = (unbounded_int*)realloc(map.values,newCapacity*sizeof(unbounded_int));
    map.capacity =newCapacity;
  }
  map.keys[map.count]=key;
  map.values[map.count] = value;
  map.count++;
}


static unbounded_int get(Key key) {
  //research value
  for(int i=0;i<map.count;i++){
    if(strncmp(map.keys[i].start,key.start,min_(map.keys[i],key))==0){
      return map.values[i];
    }
  }
  unbounded_int ui=ll2unbounded_int((long long)0);
  return ui;
}

/*--------------------------------------Interpretation--------------------------------------*/


static void closeEverything(char* source){
  free(source);
  freeValueMap();
  freeKeyArray();
  exit(EXIT_FAILURE);
}

static void analyse(char* source,Key *point1,Key *point2,size_t length,FILE* fileout) {
  if(length==5){
    //variable = entier_ou_variable op entier_ou_variable

    if(point1[0].type!=VARIABLE || point1[1].type!=EQUAL || (point1[2].type!=VARIABLE && point1[2].type!=NUMBER) || (point1[3].type!=MINUS && point1[3].type!=PLUS && point1[3].type!=STAR) || (point1[4].type!=VARIABLE && point1[4].type!=NUMBER)){
        fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
        closeEverything(source);
    }else{
      int free_1=0;
      int free_2=0;
      unbounded_int u1;
      unbounded_int u2;
      unbounded_int u3;

      if(point1[2].type==VARIABLE){
        u1=get(point1[2]);
      }else{
        char buff[point1[2].length+1];
        memmove( buff, point1[2].start, point1[2].length);
        buff[point1[2].length]='\0';
        u1=string2unbounded_int((const char*)buff);
        free_1=1;
      }

      if(point1[4].type==VARIABLE){
        u2=get(point1[4]);
      }else{
        char buff[point1[4].length+1];
        memmove( buff, point1[4].start, point1[2].length);
        buff[point1[4].length]='\0';
        u2=string2unbounded_int((const char*)buff);
        free_2=1;
      }

      if(point1[3].type==MINUS){
        u3=unbounded_int_difference(u1,u2);
        push(point1[0],u3);
      }else if(point1[3].type==PLUS){
        u3=unbounded_int_somme(u1,u2);
        push(point1[0],u3);
      }else if(point1[3].type==STAR){
        u3=unbounded_int_produit(u1,u2);
        push(point1[0],u3);
      }

      if(free_1){
        free_unbounded_int_(u1);
      }
      if(free_2){
        free_unbounded_int_(u2);
      }
    }

  }else if(length==3){
    //variable = entier
    if(point1[0].type!=VARIABLE || point1[1].type!=EQUAL || point1[2].type!=NUMBER){
        fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
        closeEverything(source);
    }else{
      char buff[point1[2].length+1];
      memmove( buff, point1[2].start, point1[2].length);
      buff[point1[2].length]='\0';
      unbounded_int u1=string2unbounded_int((const char*)buff);
      push(point1[0],u1);
    }

  }else if(length==2){
    //print variable
    if(point1[0].type!=PRINT || point1[1].type!=VARIABLE){
        fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
        closeEverything(source);
    }else{
      for(int i=0;i<point1[1].length;i++){
        fputc(point1[1].start[i],fileout);
        //printf("%c", point1[1].start[i]);
      }
      fputc('=',fileout);
      //printf(" = ");

      unbounded_int ui=get(point1[1]);

      if(ui.premier==NULL){
        fputc('\n',fileout);
        //printf("\n");
        return;
      }
      fputc(ui.signe,fileout);
      //printf("%c",ui.signe);
      if(ui.premier==ui.dernier){
        fputc((ui.premier)->c,fileout);
        fputc('\n',fileout);
        //printf("%c\n",(ui.premier)->c);
        return;
      }
      chiffre *tmp=ui.premier;
      while (tmp!=NULL) {
        fputc(tmp->c,fileout);
        //printf("%c",tmp->c);
        tmp=tmp->suivant;
      }
      fputc('\n',fileout);
      //printf("\n");
    }
  }else{
      fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
      closeEverything(source);
  }
}


static void interpret(const char* source,FILE* fileout) {
  initScanner(source);
  initKeyArray();
  initValueMap();
  Key t;
  do{
    t=getNext();
    if(t.type==ERROR){
      fprintf(stderr, "Wrong syntaxe at line %d\n", t.line);
      closeEverything((char*)source);
    }else if(t.type==VARIABLE){
      push(t,ll2unbounded_int((long long)0));
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
    analyse((char*)source,point1,point2,length,fileout);
    point1=point2;
  }
  /*for(int i=0;i<array.count;i++){
    print(array.keys[i]);
  }

  printf("%s\n","map");
  for(int i=0;i<map.count;i++){
    print(map.keys[i]);
    afficher_unbounded_int_(map.values[i]);
  }*/
  freeKeyArray();
  freeValueMap();
}



/*-----------------------------------readingFiles--------------------------------------------------------*/




/*reading file*/
static char* readFile(FILE* file) {
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory to read\n");
    exit(EXIT_FAILURE);
  }
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    fprintf(stderr, "Could not read file\n");
    free(buffer);
    exit(EXIT_FAILURE);
  }
  buffer[bytesRead] = '\0';
  fclose(file);
  return buffer;
}


/*running program*/
static void runFile(FILE* filein,FILE* fileout) {
  char* source = readFile(filein);
  interpret(source,fileout);
  free(source);
}

static FILE* replin() {
  FILE* tmp = tmpfile();
  if (tmp == NULL){
    fprintf(stderr, "%s\n","unable to crate tmp file");
    exit(EXIT_FAILURE);
  }

  char ch;
  while ((ch = fgetc(stdin)) != EOF) fputc(ch, tmp);

  rewind(tmp);
  return tmp;
}


/*-----------main-----------*/
int main(int argc, const char* argv[]) {
  if (argc==1) {

    FILE* flot=replin();
    runFile(flot,stdout);

  }else if (argc == 3 || argc == 5 ) {

    FILE* flotin=stdin;
    FILE* flotout=stdout;

    if(strcmp(argv[1],"-i")==0){
      flotin = fopen(argv[2], "rb");
      if (flotin == NULL) {
        fprintf(stderr, "Could not open file in\n");
        exit(EXIT_FAILURE);
      }
    }else if(strcmp(argv[1],"-o")==0){
      flotout = fopen(argv[2], "w");
      if (flotout == NULL) {
        fprintf(stderr, "Could not open file out\n");
        exit(EXIT_FAILURE);
      }
    }

    if(argc == 4){
      if(strcmp(argv[3],"-i")==0){
        flotin = fopen(argv[4], "rb");
        if (flotin == NULL) {
          fprintf(stderr, "Could not open file in\n");
          exit(EXIT_FAILURE);
        }
      }else if(strcmp(argv[3],"-o")==0){
        flotout = fopen(argv[4], "w");
        if (flotout == NULL) {
          fprintf(stderr, "Could not open file out\n");
          exit(EXIT_FAILURE);
        }
      }
    }

    runFile(flotin,flotout);

  }else {
    fprintf(stderr, "wrong number of parameters\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}
