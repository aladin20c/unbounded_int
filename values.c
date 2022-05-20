#ifndef values_c
#define values_c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.c"


typedef struct {
  int capacity;
  int count;
  Token token;
} Value;


typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;


ValueArray array;


void initValueArray();
void freeValueArray();


void initValueArray() {
  array->values = NULL;
  array->capacity = 0;
  array->count = 0;
}

void freeValueArray() {
  free(array->values);
  initValueArray(array);
}


static void writeValueArray(Value value) {
  if(array->values==NULL){
    array->values=(Value*)malloc(8 * sizeof(Value));
    array->capacity=8;
  }
  for(int i=0;i<array->count;i++){
    if(array->values[i]->token->start==value->token->start){
      //array->values[i]
      return;
    }
  }
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    int newCapacity = ((oldCapacity) < 8 ? 8 : (oldCapacity) * 2);
    array->values = (Value*)realloc(array->values,newCapacity+sizeof(Token));
    array->capacity =newCapacity;
  }
  array->values[array->count] = value;
  array->count++;
}


#endif
