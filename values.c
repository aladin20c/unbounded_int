#ifndef values_c
#define values_c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.c"

typedef struct {
  Key key;
  Key val;
} Value;


typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueMap;


ValueMap map;


void initValueMap();
void freeValueMap();
void addValue(Value value);

void initValueMap() {
  map.values = NULL;
  map.capacity = 0;
  map.count = 0;
}

void freeValueMap() {
  free(map.values);
  initValueMap();
}


void addValue(Value value) {
  if(map.values==NULL){
    map.values=(Value*)malloc(8 * sizeof(Value));
    map.capacity=8;
  }

  //research value
  for(int i=0;i<map.count;i++){
    if(map.values[i].key.start==value.key.start){
      map.values[i].val = value.val;
      return;
    }
  }

  if (map.capacity <= map.count + 1) {
    int oldCapacity = map.capacity;
    int newCapacity = ((oldCapacity) < 8 ? 8 : (oldCapacity) * 2);
    map.values = (Value*)realloc(map.values,newCapacity+sizeof(Key));
    map.capacity =newCapacity;
  }
  map.values[map.count] = value;
  map.count++;
}


#endif
