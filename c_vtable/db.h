#ifndef DB_H
#define DB_H

const char* db_get(void *self, const char * key);
const char* db_set(void *self, const char * key, const char * val);
const char* db_close(void *self);

#endif
