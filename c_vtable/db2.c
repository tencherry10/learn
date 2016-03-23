#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "db-priv.h"
#include "db2.h"

typedef struct db2_t {
  const db_vtable_t *vt;
  char * cfg;
  char * lastkey;
  char * lastval;
} db2_t;

const char* db2_db_get(void *self, const char *key);
const char* db2_db_set(void *self, const char *key, const char *val);
const char* db2_db_close(void *self);

const db_vtable_t db2_vtable = {
  db2_db_get,
  db2_db_set,
  db2_db_close
};

const char* db2_db_open(void **self, const char *cfg) {
  db2_t *this = malloc(sizeof(db2_t));
  this->vt = &db2_vtable;
  this->cfg = strdup(cfg);
  this->lastkey = NULL;
  this->lastval = NULL;
  *self = this;
  return NULL;
}

const char* db2_db_get(void *self, const char *key) {
  db2_t *this = self;
  printf("db2 db_get lastkey:%s lastval:%s\n", this->lastkey, this->lastval);
  free(this->lastkey); this->lastkey = strdup(key);
  free(this->lastval); this->lastval = NULL;
  printf("db2 db_get cfg:%s k:%s\n", this->cfg, key);
  return NULL;
}

const char* db2_db_set(void *self, const char *key, const char *val) {
  db2_t *this = self;
  printf("db2 db_set lastkey:%s lastval:%s\n", this->lastkey, this->lastval);
  free(this->lastkey); this->lastkey = strdup(key);
  free(this->lastval); this->lastval = strdup(val);  
  printf("db2 db_set cfg:%s k:%s v:%s\n", this->cfg, key, val);
  return NULL;
}

const char* db2_db_close(void *self) {
  db2_t *this = self;
  printf("db2 db_close cfg:%s\n", this->cfg);
  free(this->lastkey);
  free(this->lastval);
  free(this->cfg);
  free(this);
  return NULL;
}

