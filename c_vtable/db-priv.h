#ifndef DB_PRIV_H
#define DB_PRIV_H

typedef struct db_vtable_t {
  const char* (*db_get)    (void *self, const char * key);
  const char* (*db_set)    (void *self, const char * key, const char * val);
  const char* (*db_close)  (void *self);
} db_vtable_t;


#endif
