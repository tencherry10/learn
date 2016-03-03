#include <stdio.h>
#include <assert.h>
#include "pdsvl.h"

int foo_0_1_0(lua_State* L) {
  printf("foo_0_1_0\n");
}

int foo_0_2_0(lua_State* L) {
  printf("foo_0_2_0\n");
}

int bar_0_1_0(lua_State* L) {
  printf("bar_0_1_0\n");
}

int bar_0_3_0(lua_State* L) {
  printf("bar_0_3_0\n");
}

int baz_0_1_0(lua_State* L) {
  printf("baz_0_1_0\n");
}

pdsvl_t foo_lib[] = {
  {{0,2,0}, "foo", foo_0_2_0},
  {{0,1,0}, "foo", foo_0_1_0},
  {{0,3,0}, "bar", bar_0_3_0},  
  {{0,1,0}, "bar", bar_0_1_0},
  {{0,1,0}, "baz", baz_0_1_0},    
  {{0,0,0}, NULL, NULL},
};

int main(void) {
  struct luaL_Reg *foo_lua_lib = NULL;
  struct luaL_Reg *p = NULL;
  foo_lua_lib = pdsvl_buildlib(foo_lib, 0, 4, -1);
  assert(foo_lua_lib);
  p = foo_lua_lib;
  while( p->name != NULL) {
    printf("fname: %s\n", p->name);
    (*p->func)(NULL);
    ++p;
  }
  
  free(foo_lua_lib);
	return 0;
}
