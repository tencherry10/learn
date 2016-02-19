
#include <stdio.h>

extern int foo(int x, int y);

int foo(int x, int y) {
  printf("x: %d y: %d x+y:%d\n", x, y, x + y);
  return x + y;
}
