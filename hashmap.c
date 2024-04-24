#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  long index = hash(key, map->capacity);
  Pair *new_pair = createPair(key, value);
  while (map->buckets[index] != NULL) {
      index = (index + 1) % map->capacity;
  }
  map->buckets[index] = new_pair;
  map->size++;

}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  long nuevaCapacidad = map->capacity*2;
  Pair **newbucket=(Pair**)malloc(nuevaCapacidad*sizeof(Pair *));
  for(int i=0;i<map->capacity;i++){
    if (map->buckets[i]!=NULL){
      long index=hash(map->buckets[i]->key,nuevaCapacidad);
      newbucket[index]=map->buckets[i];
      
    }else{
      newbucket[i]=NULL;
    }
  }
  free(map->buckets);
  map->buckets=newbucket;
  map->capacity=nuevaCapacidad;

}


HashMap * createMap(long capacity) {
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));
  map->capacity = capacity;
  map->size = 0;
  map->current = -1;
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  return map;
}

void eraseMap(HashMap * map,  char * key) {    
  Pair *pair = searchMap(map, key);
  if (pair != NULL) {
      free(pair->key);
      free(pair->value);
      free(pair);
      map->buckets[map->current] = NULL;
      map->size--; 
  }
  
}

Pair * searchMap(HashMap * map,  char * key) {   
long index=hash(key, map->capacity);
while(map->buckets[index]!=NULL){
  if(is_equal(map->buckets[index]->key,key)){
    map->current=index;
    return map->buckets[index];
  }
  index = (index + 1) % map->capacity;
}
    return NULL;
}

Pair * firstMap(HashMap * map) {
  for (long i = 0; i < map->capacity; i++) {
    if (map->buckets[i] != NULL) {
        map->current = i;
        return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for (long i = map->current ; i < map->capacity; i++) {
    if (map->buckets[i] != NULL) {
        map->current = i;
        return map->buckets[i];
    }
  }
  return NULL;
}
