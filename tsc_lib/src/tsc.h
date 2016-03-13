#ifndef TSC__INCLUDE_TSC_H
#define TSC__INCLUDE_TSC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef __cplusplus
  #define TSC_EXTERN extern "C"
#else
  #define TSC_EXTERN extern
#endif

#define tsc_roundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

// CAUTION, if OOM happens in resize / copy / push, you are responsible for cleaning up dst
// Not possible to do it here b/c we don't have info on the resources pointed by a[i]

#define tsc_vec_resize(name, v, s) tsc_vec_##name##_resize(&(v),(s))
#define tsc_vec_copy(name, d, s) tsc_vec_##name##_copy(&(d),&(s))
#define tsc_vec_push(name, v, x) tsc_vec_##name##_push(&(v), (x))
#define tsc_vec_compact(name, v) tsc_vec_##name##_resize(&(v), (v).n)
#define tsc_vec_extend(name, d, s) tsc_vec_##name##_extend(&(d),&(s))
#define tsc_vec_reverse(name, v) tsc_vec_##name##_reverse(&(v))
#define tsc_vec_subvec(name, d, s, start, count) tsc_vec_##name##_subvec(&(d), &(s), start, count)

#define tsc_vec_init(v) ((v).n = (v).m = 0, (v).a = 0)
#define tsc_vec_destroy(v) free((v).a)
#define tsc_vec_A(v, i) ((v).a[(i)])
#define tsc_vec_pop(v) ((v).a[--(v).n])
#define tsc_vec_size(v) ((v).n)
#define tsc_vec_max(v) ((v).m)
#define tsc_vec_ptr(v) ((v).a)
#define tsc_vec_clear(v) ((v).n = 0)
#define tsc_vec_first(v) ((v).a[0])
#define tsc_vec_last(v) ((v).a[(v).n-1])
#define tsc_vec_type(name) tsc_vec_##name##_t
#define tsc_vec_define(name, type)                                                      \
  typedef struct { size_t n, m; type *a; } tsc_vec_##name##_t;                          \
  static inline const char* tsc_vec_##name##_resize(tsc_vec_##name##_t *v, size_t s) {  \
    type *tmp;                                                                          \
    if( (tmp = (type*)realloc(v->a, sizeof(type) * s)) == NULL )                        \
      return "OOM";                                                                     \
    v->m = s; v->a = tmp; return NULL;                                                  \
  }                                                                                     \
  static inline const char*                                                             \
  tsc_vec_##name##_copy(tsc_vec_##name##_t *dst, tsc_vec_##name##_t *src) {             \
    const char *estr;                                                                   \
    if (dst->m < src->n)                                                                \
      if( (estr = tsc_vec_##name##_resize(dst, src->n)) != NULL )                       \
        return estr;                                                                    \
    dst->n = src->n;                                                                    \
    memcpy(dst->a, src->a, sizeof(type) * src->n); return NULL;                         \
  }                                                                                     \
  static inline const char* tsc_vec_##name##_push(tsc_vec_##name##_t *v, type x) {      \
    const char *estr;                                                                   \
    if(v->n == v->m) {                                                                  \
      size_t m = v->m ? v->m << 1 : 4;                                                  \
      if( (estr = tsc_vec_##name##_resize(v, m)) != NULL )                              \
        return estr;                                                                    \
    }                                                                                   \
    v->a[v->n++]= x; return NULL;                                                       \
  }                                                                                     \
  static inline const char*                                                             \
  tsc_vec_##name##_extend(tsc_vec_##name##_t *dst, tsc_vec_##name##_t *src) {           \
    const char *estr;                                                                   \
    size_t n = dst->n + src->n;                                                         \
    if(dst->m < n)                                                                      \
      if( (estr = tsc_vec_##name##_resize(dst, n)) != NULL )                            \
        return estr;                                                                    \
    memcpy(dst->a + dst->n, src->a, sizeof(type) * src->n);                             \
    dst->n = n; return NULL;                                                            \
  }                                                                                     \
  static inline const char * tsc_vec_##name##_reverse(tsc_vec_##name##_t *v) {          \
    type tmp;                                                                           \
    for( size_t i = (v->n-1) >> 1 ; (i + 1) > 0 ; --i) {                                \
      tmp = v->a[i]; v->a[i] = v->a[v->n - i - 1]; v->a[v->n - i - 1] = tmp;            \
    }                                                                                   \
    return NULL;                                                                        \
  }                                                                                     \
  static inline const char* tsc_vec_##name##_subvec(tsc_vec_##name##_t *dst,            \
    tsc_vec_##name##_t *src, size_t start, size_t count)                                \
  {                                                                                     \
    const char *estr;                                                                   \
    size_t n;                                                                           \
    if(start > src->n) {                                                                \
      dst->n = 0;                                                                       \
      return NULL;                                                                      \
    }                                                                                   \
    if(start + count > src->n) {                                                        \
      n = src->n - start;                                                               \
    } else {                                                                            \
      n = count;                                                                        \
    }                                                                                   \
    if( (estr = tsc_vec_##name##_resize(dst, n)) != NULL )                              \
      return estr;                                                                      \
    memmove(dst->a, src->a + start, sizeof(type) * n);                                  \
    dst->n = n; return NULL;                                                            \
  }
#define tsc_vec_splice(v, start, count) do {                                            \
    if(count == 0) break;                                                               \
    if(start > (v).n) {                                                                 \
      (v).n = 0;                                                                        \
      break;                                                                            \
    }                                                                                   \
    if(start + count > (v).n) {                                                         \
      (v).n = start;                                                                    \
      break;                                                                            \
    }                                                                                   \
    memmove((v).a + start, (v).a + start + count,                                       \
      sizeof((v).a[0]) * ((v).n - start - count));                                      \
    (v).n -= count;                                                                     \
  } while(0)
#define tsc_vec_foreach(v, var, iter)                                                   \
  if( (v).n > 0 )                                                                       \
    for ( (iter) = 0; ((iter) < (v).n) && (((var) = (v).a[(iter)]), 1); ++(iter))
#define tsc_vec_foreach_rev(v, var, iter)                                               \
  if( (v).n > 0 )                                                                       \
    for ( (iter) = v.n - 1 ; ((iter + 1) > 0) && (((var) = (v).a[(iter)]), 1); --(iter))
#define tsc_vec_foreach_ptr(v, var, iter)                                               \
  if( (v).n > 0 )                                                                       \
    for ( (iter) = 0; ((iter) < (v).n) && (((var) = &((v).a[(iter)])), 1); ++(iter))
#define tsc_vec_foreach_ptr_rev(v, var, iter)                                           \
  if( (v).n > 0 )                                                                       \
    for ( (iter) = v.n - 1 ; ((iter + 1) > 0) && (((var) = &((v).a[(iter)]) ), 1); --(iter))


/// helper that never return error

TSC_EXTERN int    tsc_strstartswith(const char *s, const char *start);
TSC_EXTERN int    tsc_strendswith(const char *s, const char *end);
TSC_EXTERN char * tsc_strtrimleft_inplace(char *s);
TSC_EXTERN char * tsc_strtrimright_inplace(char *s);
TSC_EXTERN char * tsc_strtrim_inplace(char *s);


/// general helper

TSC_EXTERN const char * tsc_strtrim(char **ret, const char *s);
TSC_EXTERN const char * tsc_strdup(char **ret, const char *s);
TSC_EXTERN const char * tsc_strndup(char **ret, const char *s, size_t n);
TSC_EXTERN const char * tsc_strflatten(char **ret, char ** str_array, size_t n, const char * sep, size_t sep_sz);
TSC_EXTERN inline const char * tsc_strtrunc(char **ret, const char *s, size_t n);

/// files

TSC_EXTERN const char * tsc_freadline(char **ret, FILE *stream);
TSC_EXTERN const char * tsc_freadlines(char ***ret, int *nlines, FILE *stream);
TSC_EXTERN const char * tsc_freadlinesfree(char **ret, int nlines);

TSC_EXTERN const char * tsc_freadline0(char **ret, int *sz, FILE *stream);
// TSC_EXTERN const char * tsc_freadline0(char **ret, int *sz, FILE *stream);

#define TSC_DEFINE
#ifdef TSC_DEFINE

int tsc_strstartswith(const char *s, const char *start) {
  for(;; s++, start++)
    if(!*start)
      return 1;
    else if(*s != *start) 
      return 0;
}

int tsc_strendswith(const char *s, const char *end) {
  int end_len;
  int str_len;
  
  if (NULL == s || NULL == end) return 0;

  end_len = strlen(end);
  str_len = strlen(s);

  return str_len < end_len ? 1 : !strcmp(s + str_len - end_len, end);
}

char* tsc_strtrimleft_inplace(char *str) {
  int len = strlen(str);
  if(len == 0) return str;
  char *cur = str;
  while (*cur && isspace(*cur)) {
    ++cur;
    --len;
  }
  if (str != cur) memmove(str, cur, len + 1);
  return str;
}

char* tsc_strtrimright_inplace(char *str) {
  int len = strlen(str);
  if(len == 0) return str;
  char *cur = str + len - 1;
  while (cur != str && isspace(*cur)) --cur;
  cur[isspace(*cur) ? 0 : 1] = '\0';
  return str;
}

char* tsc_strtrim_inplace(char *s) {
  tsc_strtrimright_inplace(s);
  tsc_strtrimleft_inplace(s);
  return s;
}

const char * tsc_strflatten(char **ret, char ** str_array, size_t n, const char * sep, size_t sep_sz) {
  size_t lens[n];
  size_t size   = 0;
  size_t pos    = 0;
  
  for(size_t i = 0 ; i < n ; i++) {
    lens[i] = strlen(str_array[i]);
    size   += lens[i];
  }
  
  if( (*ret = (char *) malloc(size + 1 + (n-1)*sep_sz)) == NULL )
    return "OOM";
    
  (*ret)[size + (n-1)*sep_sz] = '\0';
  
  for(size_t i = 0 ; i < (n - 1) ; i++) {
    memcpy(*ret + pos, str_array[i], lens[i]);
    for(size_t j = 0 ; j < sep_sz ; j++)
      (*ret)[pos + lens[i]] = sep[j];
    pos += lens[i] + sep_sz;
  }
  
  memcpy(*ret + pos, str_array[n-1], lens[n - 1]);
  
  return NULL;
}


inline const char * tsc_strtrunc(char **ret, const char *s, size_t n) {
  return tsc_strndup(ret, s, n);
}


const char * tsc_strtrim(char **ret, const char *s) {
  const char *end;
  *ret = NULL;
  
  while (isspace(*s))
    s++;

  if (*s == 0) // only spaces
    return NULL;

  // rtrim
  end = s + strlen(s) - 1;
  while (end > s && isspace(*end))
    end--;
  
  return tsc_strndup(ret, s, end - s + 1);
}

const char * tsc_strdup(char **ret, const char *s) {
  if(s == NULL) { *ret = NULL; return NULL; }
  size_t len = strlen(s) + 1;
  if( (*ret = (char *) malloc(len)) == NULL )
    return "OOM";
  memcpy(*ret, s, len);
  return NULL;
}

const char * tsc_strndup(char **ret, const char *s, size_t n) {
  if(s == NULL) { *ret = NULL; return NULL; }
  if( (*ret = (char *) malloc(n+1)) == NULL )
    return "OOM";
  strncpy(*ret, s, n);  // use strncpy to ensure *ret is properly padded with zero
  (*ret)[n] = '\0';
  return NULL;
}


// this function models after python's readline function
// it will take care of all the corner cases of fgets
// but it assumes there are no \0 in the file/stream
//
// returns NULL on success, otherwise it return a error string

const char * tsc_freadline(char **ret, FILE *stream) {
  char        *tmp  = NULL;
  int         avail = 32;
  int         n     = 0;
  int         nlast;
  
  if( (*ret = (char *) calloc(avail,1)) == NULL )
    return "OOM";
  
  while(1) {
    if( fgets(*ret + n, avail - n, stream) == NULL ) {
      if(ferror(stream) != 0) {
        free(*ret); *ret = NULL; return "FGETS FAILED";
      } else {                  // normal condition
        return "EOF";
      }
    }
    
    nlast = strlen(*ret)-1;
    
    if( (*ret)[nlast] == '\n') { // if fgets ended b/c of newline, properly end function
      return NULL;
    }
    
    if( feof(stream) != 0 ) {
      if(nlast == (avail - 2)) {  // bad luck, eof at boundary of buffer (need to grow it by 1)
        if( (tmp = (char *) realloc(*ret, ++avail)) == NULL ) {
          free(*ret); *ret = NULL; return "OOM";
        }
      }
      return "EOF";
    }
    
    if(nlast == (avail - 2)) {  // not eof and no new line, buffer size must be not big enough
      n     = avail - 1;        // avail - 1 b/c we want to reuse the old \0 spot
      avail = avail << 1;
      if( (tmp = (char *) realloc(*ret, avail)) == NULL ) {
        free(*ret); *ret = NULL; return "OOM";
      }
      *ret = tmp;
    }
  }
}

const char * tsc_freadlines(char ***ret, int *nlines, FILE *stream) {
  const char  *estr   = NULL;
  char        *line   = NULL;
  int         avail   = 8;
  char        **tmp   = NULL;
  
  *nlines = 0;
  
  if( (*ret = (char **) calloc(avail,sizeof(char *))) == NULL )
    return "OOM";
  
  while(1) {
    if( (estr = tsc_freadline(&line, stream)) != NULL) {
      if(!strcmp("EOF", estr)) {
        (*ret)[(*nlines)++]=line;
        return NULL;
      }
      return estr;
    } else {
      (*ret)[(*nlines)++]=line;
      line = NULL;
      if(avail == *nlines) {
        if( (tmp = (char **) realloc(*ret, avail << 1)) == NULL ) {
          for(int i = 0 ; i < avail ; i++) 
            free((*ret)[i]);
          free((*ret)); *ret = NULL; return "OOM";
        }
        *ret = tmp;
        avail = avail << 1;
      }
    }
  }
}

const char * tsc_freadlinesfree(char **ret, int nlines) {
  for(int i = 0 ; i < nlines ; i++) 
    free(ret[i]);
  free(ret);
  return NULL;
}

// same as tsc_freadline, but will handle \0
// which is why sz parameter is needed. When \0 exist *sz != strlen(ret)

const char * tsc_freadline0(char **ret, int *sz, FILE *stream) {
  char        *tmp  = NULL;
  int         avail = 32;
  int         n     = 0;
  int         nlast;
  
  if( (*ret = (char *) calloc(avail, 1)) == NULL )
    return "OOM";
    
  while(1) {
    if( fgets(*ret + n, avail - n, stream) == NULL ) {
      if(ferror(stream) != 0) {
        if(sz) *sz = 0;
        free(*ret); *ret = NULL; return "FGETS FAILED";
      } else {                  // normal condition
        if(sz) *sz = n;
        return "EOF";
      }
    }
    
    for(nlast = avail - 1 ; nlast >= 0 ; nlast--) {
      if((*ret)[nlast] != '\0') {
        break;
      }
    }
    
    if( (*ret)[nlast] == '\n') { // if fgets ended b/c of newline, properly end function
      if(sz) *sz = nlast+1;
      return NULL;
    }
    
    if( feof(stream) != 0 ) {
      if(nlast == (avail - 2)) {  // bad luck, eof at boundary of buffer (need to grow it by 1)
        if( (tmp = (char *) realloc(*ret, ++avail)) == NULL ) {
          if(sz) *sz = 0; 
          free(*ret); *ret = NULL; return "OOM";
        }
      }
      if(sz) *sz = nlast+1;
      return "EOF";
    }
    
    if(nlast == (avail - 2)) {  // not eof and no new line, buffer size must be not big enough
      n     = avail - 1;        // avail - 1 b/c we want to reuse the old \0 spot
      avail = avail << 1;
      if( (tmp = (char *) realloc(*ret, avail)) == NULL ) {
        if(sz) *sz = 0;
        free(*ret); *ret = NULL; return "OOM";
      }
      *ret = tmp;
      memset(*ret + n, 0, avail - n);
    }
  }
}

#endif
#endif
