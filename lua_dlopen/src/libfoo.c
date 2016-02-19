
#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"

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

static int lua_foo(lua_State *L);

static const struct luaL_Reg foo_lib[] = {
  {"foo", lua_foo},
  {NULL,  NULL}
};

static
int foo(int x, int y) {
  printf("x: %d y: %d x+y:%d\n", x, y, x + y);
  return x + y;
}

static
int lua_foo(lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  int r = foo(x, y);
  lua_pushinteger(L, r);
  return 1;
}

static 
int luaopen_foo (lua_State *L) {
  printf("inside luaopen_foo\n");
  printf("before newlib lua_gettop = %d\n", lua_gettop(L));
  luaL_newlib(L, foo_lib);
  printf("after  newlib lua_gettop = %d\n", lua_gettop(L));
  return 1;
}

LUAMOD_API int register_foo (lua_State *L) {
  printf("inside register_foo\n");
  luaL_requiref(L, "foo", luaopen_foo, 1);
  return 1;
}


