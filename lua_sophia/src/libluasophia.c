#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <sophia.h>
#include "pdsvl.h"

static int lua_sp_env(lua_State *L) {
  void  *env; 
  if( (env = sp_env()) == NULL ) {
    lua_pushnil(L);
    lua_pushliteral(L, "Failed to create Sophia environment");
  } else {
    lua_pushlightuserdata(L, env);
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_destroy(lua_State *L) {
  void *obj = lua_touserdata(L, 1);
  lua_pushnil(L);
  if(obj) {
    sp_destroy(obj);
    obj = NULL;
    lua_pushnil(L);    
  } else {
    lua_pushliteral(L, "userdata is NULL");
  }
  return 2;
}

static int lua_sp_setstring(lua_State *L) {
  int rc;
  int nargs = lua_gettop(L);
  if(nargs < 3) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_setstring invalid nargs");
    return 2;
  }
  void        *env  = lua_touserdata(L, 1);
  const char  *path = luaL_checkstring(L, 2);
  const char  *ptr  = luaL_checkstring(L, 3);
  lua_Integer size  = (nargs == 4) ? luaL_checkinteger(L, 4) : 0;
  
  lua_pushnil(L);
  if( (rc = sp_setstring(env, path, ptr, size)) == -1) {
    lua_pushliteral(L, "sp_setstring failed");
  } else {
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_setint(lua_State *L) {
  int rc;
  void        *env  = lua_touserdata(L, 1);
  const char  *path = luaL_checkstring(L, 2);
  lua_Integer value = luaL_checkinteger(L, 3);
  lua_pushnil(L);
  if( (rc = sp_setint(env, path, value)) == -1) {
    lua_pushliteral(L, "sp_setint failed");
  } else {
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_getstring(lua_State *L) {
  int size = 0;
  int nargs = lua_gettop(L);
  if(nargs < 2) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_getstring invalid nargs");
    return 2;
  }
  void        *env    = lua_touserdata(L, 1);
  const char  *path   = luaL_checkstring(L, 2);
  int         do_free = (nargs >= 3) ? luaL_checkinteger(L, 3) : 0;
  char        *res    = NULL;
  if( (res = sp_getstring(env, path, &size)) == NULL) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_getstring failed");
    lua_pushinteger(L, 0);
  } else {
    lua_pushlstring(L, res, size);
    lua_pushnil(L);
    lua_pushinteger(L, size);
  }
  if(do_free) free(res);
  return 3;
}

static int lua_sp_getint(lua_State *L) {
  void        *env  = lua_touserdata(L, 1);
  const char  *path = luaL_checkstring(L, 2);
  int64_t res   = 0;
  res = sp_getint(env, path); // how do you detect error?
  lua_pushinteger(L, res);
  lua_pushnil(L);
  return 2;
}

static int lua_sp_getobject(lua_State *L) {
  void        *env  = lua_touserdata(L, 1);
  const char  *path = luaL_checkstring(L, 2);
  void        *res  = NULL;
  if( (res = sp_getobject(env, path)) == NULL ) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_getobject failed");
  } else {
    lua_pushlightuserdata(L, res);
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_document(lua_State *L) {
  void  *obj  = lua_touserdata(L, 1);
  void  *res  = NULL;
  if( (res = sp_document(obj)) == NULL ) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_document failed");
  } else {
    lua_pushlightuserdata(L, res);
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_open(lua_State *L) {
  void  *obj  = lua_touserdata(L, 1);
  int   rc    = 0;
  lua_pushnil(L);
  if( (rc = sp_open(obj)) == -1 ) {
    lua_pushliteral(L, "sp_open failed");
  } else {
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_close(lua_State *L) {
  void  *obj  = lua_touserdata(L, 1);
  int   rc    = 0;
  lua_pushnil(L);
  if( (rc = sp_close(obj)) == -1 ) {
    lua_pushliteral(L, "sp_close failed");
  } else {
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_error(lua_State *L) {
  void  *env  = lua_touserdata(L, 1);
  int   rc    = sp_error(env);
  lua_pushinteger(L, rc);
  lua_pushnil(L);       // this func never errors
  return 2;
}

static int lua_sp_poll(lua_State *L) {
  void  *env  = lua_touserdata(L, 1);
  void  *res;
  if( (res = sp_poll(env)) == NULL ) {
    lua_pushnil(L);  
  } else {
    lua_pushlightuserdata(L, res);
  }
  lua_pushnil(L);       // this func never errors
  return 2;
}

static int lua_sp_drop(lua_State *L) {
  void  *obj  = lua_touserdata(L, 1);
  int   rc    = 0;
  lua_pushnil(L);
  if( (rc = sp_drop(obj)) == -1 ) {
    lua_pushliteral(L, "sp_drop failed");
  } else {
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_set(lua_State *L) {
  void  *obj  = lua_touserdata(L, 1);
  void  *doc  = lua_touserdata(L, 2);
  int   rc    = 0;
  lua_pushnil(L);
  if( (rc = sp_set(obj, doc)) == -1 ) {
    lua_pushliteral(L, "sp_set failed");
  } else {
    lua_pushnil(L);
  }
  return 2;
}

//{ lua_sp_upsert
// static int lua_sp_upsert(lua_State *L) {
//   void  *obj  = lua_touserdata(L, 1);
//   void  *doc  = lua_touserdata(L, 2);
//   void  *res;
//   lua_pushnil(L);
//   if( (rc = sp_upsert(obj, doc)) == -1 ) {
//     lua_pushliteral(L, "sp_set failed");
//   } else {
//     lua_pushnil(L);
//   }
//   return 2;
// }
//}

static int lua_sp_delete(lua_State *L) {
  void  *obj  = lua_touserdata(L, 1);
  void  *doc  = lua_touserdata(L, 2);
  int   rc    = 0;
  lua_pushnil(L);
  if( (rc = sp_delete(obj, doc)) == -1 ) {
    lua_pushliteral(L, "sp_delete failed");
  } else {
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_get(lua_State *L) {
  void  *obj  = lua_touserdata(L, 1);
  void  *doc  = lua_touserdata(L, 2);
  void  *res;
  if( (res = sp_get(obj, doc)) == NULL ) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_get failed / not found");
  } else {
    lua_pushlightuserdata(L, res);
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_cursor(lua_State *L) {
  void  *env  = lua_touserdata(L, 1);
  void  *res  = NULL;
  if( (res = sp_cursor(env)) == NULL ) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_cursor failed");
  } else {
    lua_pushlightuserdata(L, res);
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_begin(lua_State *L) {
  void  *env  = lua_touserdata(L, 1);
  void  *res  = NULL;
  if( (res = sp_begin(env)) == NULL ) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_begin failed");
  } else {
    lua_pushlightuserdata(L, res);
    lua_pushnil(L);
  }
  return 2;
}

static int lua_sp_commit(lua_State *L) {
  void  *obj  = lua_touserdata(L, 1);
  int   rc    = 0;
  
  if( (rc = sp_commit(obj)) == -1 ) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_commit failed");
  } else {
    lua_pushinteger(L,rc);
    lua_pushnil(L);
  }
  return 2;
}

static pdsvl_t lua_sophia_lib[] = {
  {{0,1,0}, "commit",     lua_sp_commit},
  {{0,1,0}, "begin",      lua_sp_begin},
  {{0,1,0}, "cursor",     lua_sp_cursor},
  {{0,1,0}, "get",        lua_sp_get},
  {{0,1,0}, "delete",     lua_sp_delete},
  {{0,1,0}, "set",        lua_sp_set},
  {{0,1,0}, "drop",       lua_sp_drop},
  {{0,1,0}, "poll",       lua_sp_poll},
  {{0,1,0}, "error",      lua_sp_error},
  {{0,1,0}, "close",      lua_sp_close},
  {{0,1,0}, "open",       lua_sp_open},
  {{0,1,0}, "document",   lua_sp_document},
  {{0,1,0}, "getobject",  lua_sp_getobject},
  {{0,1,0}, "getint",     lua_sp_getint},
  {{0,1,0}, "getstring",  lua_sp_getstring},
  {{0,1,0}, "setint",     lua_sp_setint},
  {{0,1,0}, "setstring",  lua_sp_setstring},
  {{0,1,0}, "destroy",    lua_sp_destroy},
  {{0,1,0}, "env",        lua_sp_env},
  {{0,0,0}, NULL,         NULL},  
};


LUAMOD_API int register_sophia(lua_State *L) {
  lua_Integer major = luaL_checkinteger(L, 1);
  lua_Integer minor = luaL_checkinteger(L, 2);
  lua_Integer patch = luaL_checkinteger(L, 3);
  struct luaL_Reg *sophia_lib = NULL;
  const char  *modname = "sophia";
  
  // add authorization checks here
  
  if( (sophia_lib = pdsvl_buildlib(lua_sophia_lib, major, minor, patch)) == NULL) {
    lua_pushnil(L);
    lua_pushliteral(L, "pdsvl_buildlib failed");
  } else {
    size_t  num_f = 0;
    for(struct luaL_Reg *p = sophia_lib; p->name != NULL ; p++) {
      ++num_f;
    }
    luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");
    lua_getfield(L, -1, modname);  /* _LOADED[modname] */    
    if (!lua_toboolean(L, -1)) {  /* package not already loaded? */
      lua_pop(L, 1);  /* remove field */
      luaL_checkversion(L);
      lua_createtable(L, 0, num_f);
      luaL_setfuncs(L, sophia_lib, 0);
      lua_pushvalue(L, -1);  /* make copy of module (call result) */
      lua_setfield(L, -3, modname);  /* _LOADED[modname] = module */      
    }
    lua_remove(L, -2);  /* remove _LOADED table */
    lua_setglobal(L, modname);  /* _G[modname] = module */
    lua_pushnil(L);
    lua_pushnil(L);
  }
  free(sophia_lib);
  return 2;
}

