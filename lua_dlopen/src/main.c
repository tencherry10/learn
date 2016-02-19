#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <assert.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static void stackDump (lua_State *L) {
  int i;
  int top = lua_gettop(L);
  for (i = 1; i <= top; i++) {  /* repeat for each level */
    int t = lua_type(L, i);
    switch (t) {

      case LUA_TSTRING:  /* strings */
        printf("`%s'", lua_tostring(L, i));
        break;

      case LUA_TBOOLEAN:  /* booleans */
        printf(lua_toboolean(L, i) ? "true" : "false");
        break;

      case LUA_TNUMBER:  /* numbers */
        printf("%g", lua_tonumber(L, i));
        break;

      default:  /* other values */
        printf("%s", lua_typename(L, t));
        break;

    }
    printf("  ");  /* put a separator */
  }
  printf("\n");  /* end the listing */
}

int main (void) {
  //~ char buff[256];
  //~ int error;
  int errint;
  int (*register_foo)(lua_State *L);  
  
  char * errstr;
  if((errstr = dlerror()) != NULL) {
    fprintf(stderr, "at start: dlerror has failed with: %s\n", errstr);
    exit(EXIT_FAILURE);
  }
  
  void * dll = dlopen("./libfoo.so", RTLD_NOW); 
  if((errstr = dlerror()) != NULL) {
    fprintf(stderr, "after dlopen: dlerror has failed with: %s\n", errstr);
    exit(EXIT_FAILURE);
  }

  *(void **) (&register_foo) = dlsym(dll, "register_foo");
  if((errstr = dlerror()) != NULL)  {
    fprintf(stderr, "after dlsym: dlerror has failed with: %s\n", errstr);
    exit(EXIT_FAILURE);
  }
  
  lua_State *L = luaL_newstate();   /* opens Lua */
  luaL_openlibs(L);
  register_foo(L);
  //~ printf("after register_foo lua_gettop = %d\n", lua_gettop(L));
  //~ stackDump(L);
  
  errint = luaL_dostring(L, "print 'hello world'\n");  
  if(errint) {
    fprintf(stderr, "%s", lua_tostring(L, -1));
    lua_pop(L, 1);  /* pop error message from the stack */    
    exit(EXIT_FAILURE);
  }  
  errint = luaL_dostring(L, "print(foo.foo(1,2))\n");  
  if(errint) {
    fprintf(stderr, "%s", lua_tostring(L, -1));
    lua_pop(L, 1);  /* pop error message from the stack */    
    exit(EXIT_FAILURE);
  }
  
  lua_close(L);
  return 0;
}

