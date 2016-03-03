#ifndef __PDSVL_H__
#define __PDSVL_H__

#include <string.h>
#include "kvec/kvec.h"
#include "lua.h"
#include "lauxlib.h"
#include "pdsv.h"

// svl is semver for lua func

typedef struct {
  pdsv_t        version;
  const char    *name;
  lua_CFunction func;  
} pdsvl_t;

typedef kvec_t(const char *) cstr_set_t;

// assumes pd_lua_lib is sorted by decreasing version

static inline struct luaL_Reg* 
pdsvl_buildlib(pdsvl_t* pd_lua_lib, int major, int minor, int patch) {
  cstr_set_t        name_set;
  pdsvl_t           *svl;  
  size_t            num_f = 0;
  struct luaL_Reg*  lua_lib = NULL;
  size_t            lua_lib_idx = 0;
  pdsv_t            search = {.major = major, .minor=minor, .patch=patch};
  
  kv_init(name_set);
  svl = pd_lua_lib;
  while( svl->name != NULL ) {
    ++num_f;
    ++svl;
  }
  kv_resize(const char *, name_set, num_f);
  if(name_set.a == NULL)
    goto cleanup;
  
  if( (lua_lib = malloc(sizeof(struct luaL_Reg)*num_f)) == NULL )
    goto cleanup;
  
  svl = pd_lua_lib;
  while( svl->name != NULL ) {
   
    int found = 0;
    for(size_t i = 0 ; i < kv_size(name_set) ; i++) {
      if(!strcmp(kv_A(name_set, i), svl->name)) {
        found = 1;
        break;
      }
    }
    if(!found) {
      kv_push(const char *, name_set, svl->name);
    }
    ++svl;
  }
    
  for(size_t i = 0 ; i < kv_size(name_set) ; i++) {
    svl = pd_lua_lib;
    while( svl->name != NULL ) {
      
      if(strcmp(kv_A(name_set, i), svl->name)) {
        ++svl;
        continue;
      }
      
      if(pdsv_compare(&svl->version, &search) <= 0) {
        lua_lib[lua_lib_idx].name = svl->name;
        lua_lib[lua_lib_idx].func = svl->func;
        lua_lib_idx++;
        break;
      }
      ++svl;
    }
  }
  lua_lib[lua_lib_idx].name = NULL;
  lua_lib[lua_lib_idx].func = NULL;
  
cleanup:
  kv_destroy(name_set);
  return lua_lib;
}

#endif
