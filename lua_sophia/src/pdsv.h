#ifndef __PDSV_H__
#define __PDSV_H__

// sv is semver

// Notes:
// 1. -1 is effectively a "*"
// 2. if major is "*" then minor / patch is assumed to be "*"
// 3. if minor is "*" then patch is assumed to be "*"

typedef struct pdsv_t {
  int major;
  int minor;
  int patch;
} pdsv_t;

static inline int 
pdsv_compare(const pdsv_t * a, const pdsv_t * b) {
  if((a->major == -1) || (b->major == -1))
    return 0;
  
  if(a->major != b->major)
    return a->major > b->major ? 1 : -1;
  
  if((a->minor == -1) || (b->minor == -1))
    return 0;
  
  if(a->minor != b->minor)
    return a->minor > b->minor ? 1 : -1;
  
  if((a->patch == -1) || (b->patch == -1))
    return 0;
  
  if(a->patch != b->patch)
    return a->patch > b->patch ? 1 : -1;
  
  return 0;
}

static inline pdsv_t 
pdsv_incpatch(const pdsv_t * v) {
  pdsv_t ret = *v;
  ret.patch  += 1;
  return ret;
}

static inline pdsv_t 
pdsv_incminor(const pdsv_t * v) {
  pdsv_t ret = *v;
  ret.minor  += 1;
  ret.patch   = 0;
  return ret;
}

static inline pdsv_t 
pdsv_incmajor(const pdsv_t * v) {
  pdsv_t ret = *v;
  ret.major  += 1;
  ret.minor   = 0;
  ret.patch   = 0;
  return ret;
}

#endif
