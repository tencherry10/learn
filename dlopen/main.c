
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>

//~ void *dlopen(const char *filename, int flag); 

int main(int argc, char ** argv) {
  char * error;
  int (*foo)(int, int);
  
  assert(argc == 2);
  
  printf("start of main argv[1]:%s\n", argv[1]);
  if((error = dlerror()) != NULL) {
    fprintf(stderr, "at start: dlerror has failed with: %s\n", error);
    exit(EXIT_FAILURE);
  }
  
  void * dll = dlopen(argv[1], RTLD_NOW); 
  if((error = dlerror()) != NULL) {
    fprintf(stderr, "after dlopen: dlerror has failed with: %s\n", error);
    exit(EXIT_FAILURE);
  }
  
  *(void **) (&foo) = dlsym(dll, "foo");
  if((error = dlerror()) != NULL)  {
    fprintf(stderr, "after dlsym: dlerror has failed with: %s\n", error);
    exit(EXIT_FAILURE);
  }
  
  printf("foo located at address=%p\n", foo);
  
  printf("calling foo:\n");
  printf("foo: %d\n", foo(100, 200));

  dlclose(dll);
  return 0;
}
