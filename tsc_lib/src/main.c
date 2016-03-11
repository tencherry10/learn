
#define TSC_DEFINE
#include "tsc.h"

int main(int argc, const char ** argv) {
  (void)argc;
  (void)argv;
  int line = 0;
  const char * estr = NULL;
  char * res = NULL;
  int  sz;
  FILE * fp;
  if( (fp = fopen("../test/t1.txt", "r")) == NULL)
    return 1;
  
  while(1) {
    if( (estr = tsc_freadline0(&res, &sz, fp)) != NULL) {
      if(!strcmp("EOF", estr)) {
        printf("line %d/sz=%d : %s", line++, sz, res);
      } else {
        printf("tsc_fgets failed with %s\n",  estr);
      }
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

  char ** lines = NULL;
  int nlines = 0;
  
  if( (fp = fopen("../test/t2.txt", "r")) == NULL)
    return 1;
  
  if( (estr = tsc_freadlines(&lines, &nlines, fp)) != NULL) {
    for(int i = 0 ; i < nlines ; i++) {
      printf("line %d/sz=%zu : %s", i, strlen(lines[i]), lines[i]);
    }
  }
  
  if(fp) fclose(fp);
  
  free(res);
  return 0;
}
