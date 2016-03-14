#include <stdio.h>
void __attribute__((weak)) f();
int main(void) {
  if (f)
    f();
  printf("main done\n");
  return 0;
}
