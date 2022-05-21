// gcc -Wall calc_unbounded_int.c && ./a.out ex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.c"
#include "unbounded_int.c"

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


static void free_unbounded_int_(unbounded_int ui){
    chiffre *tmp=ui.premier;
    if(ui.premier==NULL){
      return;
    }
    if(ui.premier==ui.dernier){
        free(ui.premier);
        return;
    }
    while (tmp!=NULL) {
      chiffre *tmp2=tmp;
      tmp=tmp->suivant;
      free(tmp2);
    }
}


static void freeValueMap() {
  free(map.keys);
  free(map.values);
  for(int i=0;i<map.count;i++){
    free_unbounded_int(map.values[i]);
  }
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
    if(map.keys[i].start==key.start){
      free_unbounded_int_(map.values[i]);
      map.values[i] = value;
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
    if(map.keys[i].start==key.start){
      return map.values[i];
    }
  }
  unbounded_int ui=ll2unbounded_int((long long)0);
  push(key,ui);
  return ui;
}

/*--------------------------------------Interpretation--------------------------------------*/


static void closeEverything(char* source){
  free(source);
  freeValueMap();
  freeKeyArray();
  exit(EXIT_FAILURE);
}

static void analyse(char* source,Key *point1,Key *point2,size_t length) {
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
        buff[point1[2].length+1]='\0';
        u1=string2unbounded_int((const char*)buff);
        free_1=1;
      }

      if(point1[4].type==VARIABLE){
        u2=get(point1[4]);
      }else{
        char buff[point1[4].length+1];
        memmove( buff, point1[4].start, point1[2].length);
        buff[point1[4].length+1]='\0';
        u2=string2unbounded_int((const char*)buff);
        free_2=1;
      }

      if(point1[3].type==MINUS){

      }else if(point1[3].type==PLUS){

      }else if(point1[3].type==STAR){

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
      buff[point1[2].length+1]='\0';
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
        printf("%c", point1[1].start[i]);
      }
      printf(" = ");
      unbounded_int ui=get(point1[1]);
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

  }else{
      fprintf(stderr, "%s %d %s\n","error at line",point1->line,"unrecognized sequence");
      closeEverything(source);
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
      closeEverything((char*)source);
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
    /////////////////////////////////----------------------------------------------------------------------------------------------------------------------------////////////////////////////////analyse((char*)source,point1,point2,length);
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
