#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct db_vtable_t {
  const char* (*db_get)    (void *self, const char * key);
  const char* (*db_set)    (void *self, const char * key, const char * val);
  const char* (*db_close)  (void *self);
} db_vtable_t;

typedef struct db_t {
  const db_vtable_t *vt;
} db_t;

typedef struct db1_t {
  const db_vtable_t *vt;
  char * cfg;
} db1_t;

typedef struct db2_t {
  const db_vtable_t *vt;
  char * cfg;
  char * lastkey;
  char * lastval;
} db2_t;

const char* db1_db_open(void **self, const char *cfg);
const char* db1_db_get(void *self, const char *key);
const char* db1_db_set(void *self, const char *key, const char *val);
const char* db1_db_close(void *self);

const char* db2_db_open(void **self, const char *cfg);
const char* db2_db_get(void *self, const char *key);
const char* db2_db_set(void *self, const char *key, const char *val);
const char* db2_db_close(void *self);

const db_vtable_t db2_vtable = {
  db2_db_get,
  db2_db_set,
  db2_db_close
};

const db_vtable_t db1_vtable = {
  db1_db_get,
  db1_db_set,
  db1_db_close
};

static inline const char* db_get(void *self, const char * key) {
  db_t *this = self;
  return this->vt->db_get(self, key);
}

static inline const char* db_set(void *self, const char * key, const char * val) {
  db_t *this = self;
  return this->vt->db_set(self, key, val);
}

static inline const char* db_close(void *self) {
  db_t *this = self;
  return this->vt->db_close(self);
}

int main(void) {
  void *db1, *db2;
  db1_db_open(&db1, "test");
  db2_db_open(&db2, "test2");
  
  db_get(db1,"hello");
  db_get(db2,"hello");
  db_set(db1,"hello", "world");
  db_set(db2,"hello", "world");
  db_set(db2,"goodbye", "humanity");
  db_get(db2,"goodbye");
  db_set(db2,"sorry", "i am back");
  db_close(db1);
  db_close(db2);
  
  return 0;
}

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
