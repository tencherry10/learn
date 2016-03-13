#include <assert.h>

#define TSC_DEFINE
#include "tsc.h"

tsc_vec_define(vi, int)
typedef tsc_vec_type(vi) vec_int_t;

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
  
  vec_int_t a;
  tsc_vec_init(a);
  tsc_vec_push(vi, a, 0);
  tsc_vec_push(vi, a, 1);
  tsc_vec_push(vi, a, 2);
  tsc_vec_push(vi, a, 3);
  tsc_vec_push(vi, a, 4);
  
  vec_int_t b;
  tsc_vec_init(b);
  tsc_vec_push(vi, b, 10);
  tsc_vec_push(vi, b, 20);
  tsc_vec_push(vi, b, 30);
  tsc_vec_push(vi, b, 40);
  
  printf("first: %d\n", tsc_vec_first(a));
  printf("last:  %d\n", tsc_vec_last(a));
  
  size_t i = 0;
  int v;
  tsc_vec_foreach(a, v, i)
    printf("v[%zu] = %d\n", i, v);
  int * vp;
  tsc_vec_foreach_ptr(a, vp, i)
    *vp = i * *vp;
  tsc_vec_foreach_rev(a, v, i)
    printf("v[%zu] = %d\n", i, v);
  tsc_vec_foreach_ptr_rev(a, vp, i) {
    *vp = *vp + i;
    printf("v[%zu] = %d\n", i, *vp);
  }
  
  printf("\n\n");
  
  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);
  printf("\n\n");
  tsc_vec_extend(vi, b, a);

  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);  
  printf("\n\n");
  tsc_vec_pop(b);
  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);    
  printf("\n\n");
  
  tsc_vec_reverse(vi, b);
  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);

  printf("\n\n");  
  tsc_vec_splice(b, 2, 2);
  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);
    
  tsc_vec_destroy(a);
  tsc_vec_destroy(b);
  
  return 0;
}
