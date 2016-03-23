#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "db-priv.h"
#include "db1.h"

typedef struct db1_t {
  const db_vtable_t *vt;
  char * cfg;
} db1_t;

const char* db1_db_get(void *self, const char *key);
const char* db1_db_set(void *self, const char *key, const char *val);
const char* db1_db_close(void *self);

const db_vtable_t db1_vtable = {
  db1_db_get,
  db1_db_set,
  db1_db_close
};

const char* db1_db_get(void *self, const char *key) {
  db1_t *this = self;
  printf("db1 db_get cfg:%s k:%s\n", this->cfg, key);
  return NULL;
}

const char* db1_db_set(void *self, const char *key, const char *val) {
  db1_t *this = self;
  printf("db1 db_set cfg:%s k:%s v:%s\n", this->cfg, key, val);
  return NULL;
}

const char* db1_db_close(void *self) {
  db1_t *this = self;
  printf("db1 db_close cfg:%s\n", this->cfg);
  free(this->cfg);
  free(this);
  return NULL;
}

const char* db1_db_open(void **self, const char *cfg) {
  db1_t *ret = malloc(sizeof(db1_t));
  ret->vt = &db1_vtable;
  size_t slen = strlen(cfg);
  ret->cfg = malloc(slen+1);
  ret->cfg[slen] = '\0';
  strncpy(ret->cfg, cfg, slen);
  *self = ret;
  return NULL;
}

