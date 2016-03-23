#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db1.h"
#include "db2.h"

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

