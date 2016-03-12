#include <assert.h>

#define TSC_DEFINE
#include "tsc.h"

int main(int argc, const char ** argv) {
  (void)argc;
  (void)argv;
  int         line    = 0;
  const char  *estr   = NULL;
  char        *res    = NULL;
  char        **lines = NULL;
  int         nlines  = 0;  
  int         sz;
  FILE        *fp;
  
  if( (fp = fopen("../test/t1.txt", "r")) == NULL)
    return 1;
  
  while(1) {
    if( (estr = tsc_freadline0(&res, &sz, fp)) != NULL) {
      if(!strcmp("EOF", estr)) {
        printf("line %d/sz=%d : %s", line++, sz, res);
      } else {
        printf("tsc_fgets failed with %s\n",  estr);
      }
      free(res);
      break;
    } else {
      printf("line %d/sz=%d : %s", line++, sz, res);
      if(res == NULL)
        break;      
      free(res);
      res = NULL;      
    }
  }
  
  if(fp) fclose(fp);
  
  printf("\n\n");
  
  line = 0;
  
  if( (fp = fopen("../test/t2.txt", "r")) == NULL)
    return 1;
  
  while(1) {
    if( (estr = tsc_freadline(&res, fp)) != NULL) {
      sz = strlen(res);
      if(!strcmp("EOF", estr)) {
        printf("line %d/sz=%d : %s", line++, sz, res);
        free(res);
      } else {
        printf("tsc_fgets failed with %s\n",  estr);
      }
      break;
    } else {
      sz = strlen(res);
      printf("line %d/sz=%d : %s", line++, sz, res);
      if(res == NULL)
        break;      
      free(res);
      res = NULL;      
    }
  }
  if(fp) fclose(fp);
  printf("\n\n");
  
  if( (fp = fopen("../test/t2.txt", "r")) == NULL)
    return 1;
  
  
  if( (estr = tsc_freadlines(&lines, &nlines, fp)) == NULL) {
    for(int i = 0 ; i < nlines ; i++) {
      printf("line %d/sz=%zu : %s", i, strlen(lines[i]), lines[i]);
      tsc_strtrim_inplace(lines[i]);
    }
    printf("\n\n");
    
    if( (estr = tsc_strflatten(&res, lines, nlines, ",", 1)) == NULL ) {
      printf("res: %s", res);
    }
      
    free(res);
    tsc_freadlinesfree(lines, nlines);
  }
  if(fp) fclose(fp);
  
  printf("\n\n");
  
  assert(tsc_strstartswith("abcd123", "ab"));
  assert(!tsc_strstartswith("abcd123", "bb"));
  
  
  
  
  
  
  return 0;
}
