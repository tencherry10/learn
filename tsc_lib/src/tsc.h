#ifndef TSC__INCLUDE_TSC_H
#define TSC__INCLUDE_TSC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
  #define TSC_EXTERN extern "C"
#else
  #define TSC_EXTERN extern
#endif

TSC_EXTERN const char * tsc_freadline(char **ret, FILE *stream);
TSC_EXTERN const char * tsc_freadlines(char ***ret, int *nlines, FILE *stream);

TSC_EXTERN const char * tsc_freadline0(char **ret, int *sz, FILE *stream);
// TSC_EXTERN const char * tsc_freadline0(char **ret, int *sz, FILE *stream);

#ifdef TSC_DEFINE

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
          *ret = NULL;
          return "OOM";
        }
        *ret = tmp;
        avail = avail << 1;
      }
    }
  }
}

// same as tsc_freadline, but will handle \0
// which is why sz parameter is needed. When \0 exist *sz != strlen(ret)

const char * tsc_freadline0(char **ret, int *sz, FILE *stream) {
  char        *tmp  = NULL;
  int         avail = 32;
  int         n     = 0;
  int         nlast;
  
  if( (*ret = (char *) malloc(avail)) == NULL )
    return "OOM";
  
  memset(*ret, 0, avail);
  
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
