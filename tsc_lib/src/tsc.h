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

/// helper that never return error

TSC_EXTERN int    tsc_strstartswith(const char *s, const char *start);
TSC_EXTERN char * tsc_strtrimleft_inplace(char *s);
TSC_EXTERN char * tsc_strtrimright_inplace(char *s);
TSC_EXTERN char * tsc_strtrim_inplace(char *s);


/// general helper

TSC_EXTERN const char * tsc_strtrim(char **ret, const char *s);
TSC_EXTERN const char * tsc_strdup(char **ret, const char *s);
TSC_EXTERN const char * tsc_strndup(char **ret, const char *s, size_t n);
TSC_EXTERN const char * tsc_strflatten(char **ret, char ** str_array, size_t n, const char * sep, size_t sep_sz);

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

const char * tsc_strtrim(char **ret, const char *s) {
  const char *end;
  *ret = NULL;
  
  while (isspace(*s))
    s++;

  if (*s == 0) // only spaces
    return NULL;

  // rtrim
  end = s + strlen(s) - 1;
  while (end > s && isspace(*end)) {
    end--;
  }
  
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
  (*ret)[n] = 0;
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
        free(*ret);
        *ret = NULL;
        return "FGETS FAILED";
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
          free(*ret);
          *ret = NULL;
          return "OOM";
        }
      }
      return "EOF";
    }
    
    if(nlast == (avail - 2)) {  // not eof and no new line, buffer size must be not big enough
      n     = avail - 1;        // avail - 1 b/c we want to reuse the old \0 spot
      avail = avail << 1;
      if( (tmp = (char *) realloc(*ret, avail)) == NULL ) {
        free(*ret);
        *ret = NULL;
        return "OOM";
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
          free((*ret));
          *ret = NULL;
          return "OOM";
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
        free(*ret);
        *ret = NULL;
        if(sz) *sz = 0;
        return "FGETS FAILED";
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
          free(*ret);
          *ret = NULL;
          if(sz) *sz = 0;
          return "OOM";
        }
      }
      if(sz) *sz = nlast+1;
      return "EOF";
    }
    
    if(nlast == (avail - 2)) {  // not eof and no new line, buffer size must be not big enough
      n     = avail - 1;        // avail - 1 b/c we want to reuse the old \0 spot
      avail = avail << 1;
      if( (tmp = (char *) realloc(*ret, avail)) == NULL ) {
        free(*ret);
        *ret = NULL;
        if(sz) *sz = 0;
        return "OOM";
      }
      *ret = tmp;
      memset(*ret + n, 0, avail - n);
    }
  }
}

#endif
#endif
