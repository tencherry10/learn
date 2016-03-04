#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
  char      *res;
  uint8_t   err;
} ret_with_err_t;

ret_with_err_t foo(char * s) {
  ret_with_err_t ret;
  ret.err = 1;
  ret.res = s;
  return ret;
}

int main(int argc, char ** argv) {
  ret_with_err_t  ret;
  char *s = "hello world\n";
  if( (ret = foo(s)).err ) {
    printf("error");
    printf("%s", ret.res);
  }
  return 0;
}
