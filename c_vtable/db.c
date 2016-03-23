#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "db-priv.h"

typedef struct db_t {
  const db_vtable_t *vt;
} db_t;

const char* db_get(void *self, const char * key) {
  db_t *this = self;
  return this->vt->db_get(self, key);
}

const char* db_set(void *self, const char * key, const char * val) {
  db_t *this = self;
  return this->vt->db_set(self, key, val);
}

const char* db_close(void *self) {
  db_t *this = self;
  return this->vt->db_close(self);
}

