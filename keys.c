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


void initKeyArray() {
  array.keys = NULL;
  array.capacity = 0;
  array.count = 0;
}

void freeKeyArray() {
  free(array.keys);
  initKeyArray(array);
}


void addKey(Key key){
  if(array.keys==NULL){
    array.keys=(Key*)malloc(8 * sizeof(Key));
    array.capacity=8;
  }
  if (array.capacity < array.count + 1) {
    int oldCapacity = array.capacity;
    int newCapacity = ((oldCapacity) < 8 ? 8 : (oldCapacity) * 2);
    array.keys = (Key*)realloc(array.keys,newCapacity+sizeof(Key));
    array.capacity =newCapacity;
  }
  array.keys[array.count] = key;
  array.count++;
}


#endif
