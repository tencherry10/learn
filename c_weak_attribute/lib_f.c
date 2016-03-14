#include <stdio.h>
void __attribute__((weak)) f() {
  printf("lib f.. ");
}
