#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <sophia.h>

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
  void        *env  = lua_touserdata(L, 1);
  const char  *path = luaL_checkstring(L, 2);
  const char  *ptr  = luaL_checkstring(L, 3);
  lua_pushnil(L);
  if( (rc = sp_setstring(env, path, ptr, 0)) == -1) {
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
  void        *env  = lua_touserdata(L, 1);
  const char  *path = luaL_checkstring(L, 2);
  char        *res  = NULL;
  if( (res = sp_getstring(env, path, 0)) == NULL) {
    lua_pushnil(L);
    lua_pushliteral(L, "sp_getstring failed");
  } else {
    lua_pushstring(L, res);
    lua_pushnil(L);
  }
  free(res);
  return 2;
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


