#ifndef keys_c
#define keys_c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.c"

typedef struct {
  int capacity;
  int count;
  Key* keys;
} KeyArray;


KeyArray array;

void initKeyArray();
void freeKeyArray();
void addKey(Key key);


void initValueArray() {
  array->keys = NULL;
  array->capacity = 0;
  array->count = 0;
}

void freeValueArray() {
  free(array->keys);
  initValueArray(array);
}


  void addKey(Key key){
  if(array->keys==NULL){
    array->values=(Value*)malloc(8 * sizeof(Value));
    array->capacity=8;
  }
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    int newCapacity = ((oldCapacity) < 8 ? 8 : (oldCapacity) * 2);
    array->keys = (Key*)realloc(array->keys,newCapacity+sizeof(Token));
    array->capacity =newCapacity;
  }
  array->values[array->count] = key;
  array->count++;
}


#endif
