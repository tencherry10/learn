#include <assert.h>

#define TSC_POOL_FIRST_FIT
#define TSC_DEFINE
#include "tsc.h"

tsc_vec_define(vi, int)
typedef tsc_vec_type(vi) vec_int_t;

tsc_pool_block  heap1[2600];

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
  
  vec_int_t a, b, c;
  tsc_vec_init(a);
  tsc_vec_init(b);
  tsc_vec_init(c);
  
  tsc_vec_push(vi, a, 0);
  tsc_vec_push(vi, a, 1);
  tsc_vec_push(vi, a, 2);
  tsc_vec_push(vi, a, 3);
  tsc_vec_push(vi, a, 4);
  
  
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
  printf("\n\n");  
  
  tsc_vec_clear(b);
  tsc_vec_push(vi, b, 0);
  tsc_vec_push(vi, b, 1);
  tsc_vec_push(vi, b, 2);
  tsc_vec_push(vi, b, 3);
  tsc_vec_push(vi, b, 4);  

  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);
  printf("\n\n");  
  
  tsc_vec_pushfront(vi, b, 1000);

  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);
  printf("\n\n");  
  
  tsc_vec_splice(b, 1, 1);

  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);
  printf("\n\n");  

  tsc_vec_insert(vi, b, 4, 2000);
  
  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);
  printf("\n\n");  
  
  tsc_vec_insert(vi, b, tsc_vec_size(b), -1);

  tsc_vec_foreach(b, v, i)
    printf("v[%zu] = %d\n", i, v);
  printf("\n\n");  
  
  tsc_vec_subvec(vi, c, b, 1, 3);

  tsc_vec_foreach(b, v, i)
    printf("b[%zu] = %d\n", i, v);
  tsc_vec_foreach(c, v, i)
    printf("c[%zu] = %d\n", i, v);    
  printf("\n\n");  
  
  tsc_vec_copy(vi, a, b);
  tsc_vec_foreach(a, v, i)
    printf("a[%zu] = %d\n", i, v);    
  printf("\n\n");  

  tsc_vec_copy(vi, a, a);
  tsc_vec_foreach(a, v, i)
    printf("a[%zu] = %d\n", i, v);    
  printf("\n\n");  

  tsc_vec_extend(vi, a, a);
  tsc_vec_foreach(a, v, i)
    printf("a[%zu] = %d\n", i, v);    
  printf("\n\n");  
  
  tsc_vec_copy(vi, c, b);
  
  tsc_vec_foreach_ptr(b, vp, i)
    *vp = *vp < 0;  
  tsc_vec_foreach(b, v, i)
    printf("b[%zu] = %d\n", i, v);    
  printf("all: %d\n", tsc_vec_all(vi, b));
  printf("any: %d\n", tsc_vec_any(vi, b));
  printf("\n\n");  
  
  
  
  tsc_vec_pop(c);
  tsc_vec_foreach_ptr(c, vp, i)
    *vp = *vp >= 0;  
  tsc_vec_foreach(c, v, i)
    printf("c[%zu] = %d\n", i, v);    
  printf("all: %d\n", tsc_vec_all(vi, c));
  printf("any: %d\n", tsc_vec_any(vi, c));
  printf("\n\n");  
  
  
  

  tsc_vec_destroy(a);
  tsc_vec_destroy(b);
  tsc_vec_destroy(c);
  
  char *ss = "Hello World";
  char *s; 
  tsc_strupper(&s, ss);
  printf("upper: %s\n", s);
  free(s);
  tsc_strlower(&s, ss);
  printf("lower: %s\n", s);
  free(s);
  
  {
    void *      ptr_array[256];
    size_t      i;
    int         idx;
    tsc_pool_t  pool1;
    
    tsc_pool_init(&pool1, heap1, sizeof(heap1)/sizeof(heap1[0]));
    //~ tsc_pool_init(&pool1, NULL, 2600);

    //~ printf( "Size of umm_heap is %zu\n", sizeof(heap1)       );
    //~ printf( "Size of header   is %zu\n", sizeof(heap1[0])    );
    //~ printf( "Size of nblock   is %zu\n", sizeof(heap1[0].header.used.next) );
    //~ printf( "Size of pblock   is %zu\n", sizeof(heap1[0].header.used.prev) );
    //~ printf( "Size of nfree    is %zu\n", sizeof(heap1[0].body.free.next)   );
    //~ printf( "Size of pfree    is %zu\n", sizeof(heap1[0].body.free.prev)   );
    
    //~ tsc_pool_info(&pool1, NULL);
    
    #define realloc(...)  tsc_pool_realloc(&pool1,__VA_ARGS__)
    #define malloc(...)   tsc_pool_malloc(&pool1,__VA_ARGS__)
    #define free(...)     tsc_pool_free(&pool1,__VA_ARGS__)

    for( idx=0; idx<256; ++idx )
      ptr_array[idx] = NULL;
    
    size_t sz = 0;
    for( idx=0; idx<6553500; ++idx ) {
      i = rand()%256;
 
      switch( rand() % 16 ) {
      case  0:
      case  1:
      case  2:
      case  3:
      case  4:
      case  5:
      case  6:  ptr_array[i] = realloc(ptr_array[i], 0);
                //printf("free ptr[%zu]\n", i);
                break;
      case  7:
      case  8:  sz = rand()%40;
                ptr_array[i] = realloc(ptr_array[i], sz );
                //printf("alloc ptr[%zu] w/ %zu bytes\n", i, sz);
                break;

      case  9:
      case 10:
      case 11:
      case 12:  sz = rand()%100;
                ptr_array[i] = realloc(ptr_array[i], sz );
                //printf("alloc ptr[%zu] w/ %zu bytes\n", i, sz);
                break;

      case 13:
      case 14:  sz = rand()%200;
                ptr_array[i] = realloc(ptr_array[i], sz );
                //printf("alloc ptr[%zu] w/ %zu bytes\n", i, sz);
                break;

      default:  sz = rand()%400;
                ptr_array[i] = realloc(ptr_array[i], sz );
                //printf("alloc ptr[%zu] w/ %zu bytes\n", i, sz);
                break;
      }
      
      //tsc_pool_info(&pool1, NULL); printf("\n\n");
    }

    tsc_pool_freeall(&pool1);
    
    //~ for( idx=0; idx<256; ++idx )
      //~ free(ptr_array[idx]);
    
    tsc_pool_info(&pool1, NULL);
    
    tsc_pool_deinit(&pool1);
    
    #undef realloc
    #undef malloc
    #undef free
  }
  
  return 0;
}
