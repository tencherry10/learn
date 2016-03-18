

#define TSC_POOL_FIRST_FIT
#define TSC_DEFINE
#include "tsc.h"
#include <assert.h>
#include <malloc.h>


tsc_vec_define(vi, int)
typedef tsc_vec_type(vi) vec_int_t;

tsc_pool_block  heap1[2600];

int main(int argc, const char ** argv) {
  (void)argc;
  (void)argv;
  if(0) {
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
  }

  if(0) {
    void *      ptr_array[256];
    size_t      i;
    int         idx;
    tsc_pool_t  pool1;
    
    //~ tsc_pool_init(&pool1, heap1, sizeof(heap1)/sizeof(heap1[0]));
    tsc_pool_init(&pool1, NULL, 2600);

    #define realloc(...)  tsc_pool_realloc(&pool1,__VA_ARGS__)
    #define malloc(...)   tsc_pool_malloc(&pool1,__VA_ARGS__)
    #define free(...)     tsc_pool_free(&pool1,__VA_ARGS__)

    for( idx=0; idx<256; ++idx )
      ptr_array[idx] = NULL;
    
    size_t sz = 0;
    for( idx=0; idx<1024; ++idx ) {
      i = rand()%256;
 
      switch( rand() % 16 ) {
      case  0:
      case  1:
      case  2:
      case  3:
      case  4:
      case  5:
      case  6:  ptr_array[i] = realloc(ptr_array[i], 0);
                printf("free ptr[%zu]\n", i);
                break;
      case  7:
      case  8:  sz = rand()%40;
                ptr_array[i] = realloc(ptr_array[i], sz );
                printf("alloc ptr[%zu] w/ %zu bytes\n", i, sz);
                break;

      case  9:
      case 10:
      case 11:
      case 12:  sz = rand()%100;
                ptr_array[i] = realloc(ptr_array[i], sz );
                printf("alloc ptr[%zu] w/ %zu bytes\n", i, sz);
                break;

      case 13:
      case 14:  sz = rand()%200;
                ptr_array[i] = realloc(ptr_array[i], sz );
                printf("alloc ptr[%zu] w/ %zu bytes\n", i, sz);
                break;

      default:  sz = rand()%400;
                ptr_array[i] = realloc(ptr_array[i], sz );
                printf("alloc ptr[%zu] w/ %zu bytes\n", i, sz);
                break;
      }
      
      tsc_pool_info(&pool1, NULL); printf("\n\n");
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
  
  if(0) {
    tsc_hpool_t pool1;    
    tsc_hpool_init(&pool1, NULL, 64);

    #define realloc(...)  tsc_hpool_realloc(&pool1,__VA_ARGS__)
    #define malloc(...)   tsc_hpool_malloc(&pool1,__VA_ARGS__)
    #define calloc(...)   tsc_hpool_calloc(&pool1,__VA_ARGS__)
    #define free(...)     tsc_hpool_free(&pool1,__VA_ARGS__)
    #define attach(...)   tsc_hpool_attach(&pool1,__VA_ARGS__)
    
    void * ra   = malloc(36);
    void * c1a  = malloc(36); attach(c1a, ra);
    void * c1b  = malloc(36); attach(c1b, ra);
    void * c2a  = malloc(2);  attach(c2a, c1a);
    void * c2b  = malloc(4);  attach(c2b, c1a);
    void * c2c  = malloc(20); attach(c2c, c1b);
    void * c2d  = malloc(11); attach(c2d, c1b);
    void * c1c  = malloc(1);  attach(c1c, ra);
    
    tsc_hpool_info(&pool1, NULL);
    
    c1a = realloc(c1a, 66);
    
    tsc_hpool_info(&pool1, NULL);
    
    c1b = realloc(c1b, 66);
    
    tsc_hpool_info(&pool1, NULL);
    
    attach(c1b, c1c);
    
    tsc_hpool_info(&pool1, NULL);
    
    attach(c1c, c1a);
    
    tsc_hpool_info(&pool1, NULL);
    
    free(c1c);
    
    tsc_hpool_info(&pool1, NULL);
    
    void * rb   = malloc(5);
    
    tsc_hpool_freeall(&pool1);
    tsc_hpool_deinit(&pool1);

    #undef realloc
    #undef malloc
    #undef calloc
    #undef free
    #undef attach
    
  }

  if(0) {
    tsc_pool_t pool1;    
    tsc_pool_init(&pool1, NULL, 64);

    #define realloc(...)  tsc_pool_realloc(&pool1,__VA_ARGS__)
    #define malloc(...)   tsc_pool_malloc(&pool1,__VA_ARGS__)
    #define calloc(...)   tsc_pool_calloc(&pool1,__VA_ARGS__)
    #define free(...)     tsc_pool_free(&pool1,__VA_ARGS__)
    #define attach(...)   tsc_pool_attach(&pool1,__VA_ARGS__)
    
    uint64_t * m1   = malloc(8);
    uint64_t * m2   = malloc(8);
    uint64_t * m3   = malloc(8);
    uint64_t * m4   = malloc(8);
    
    tsc_pool_info(&pool1, NULL);

    printf("m1:%p\n", (void *)m1);
    printf("m2:%p\n", (void *)m2);
    printf("m3:%p\n", (void *)m3);    
    printf("m4:%p\n", (void *)m4);    
    
    *m1 = 1;
    *m2 = 2;
    *m3 = 3;
    *m4 = 4;
    
    tsc_pool_freeall(&pool1);
    tsc_pool_deinit(&pool1);

    #undef realloc
    #undef malloc
    #undef calloc
    #undef free
    #undef attach
    
  }
  
  
  if(0) {
    void *p1 = malloc(40);
    void *p2 = malloc(40);
    void *p3 = malloc(40);
    
    printf("p1:%p\n", p1);
    printf("p2:%p\n", p2);
    printf("p3:%p\n", p3);
    
    free(p1);
    free(p2);
    free(p3);
  }
  
  if(0) { 
    int *p1 = malloc(8152);
    //~ int *p2 = malloc(0);
    struct mallinfo info = mallinfo();
    //~ printf("p1 = %p\n", (void*) p1);
    //~ printf("p2 = %p\n", (void*) p2);
    printf("info.arena    = %d\n", info.arena);
    printf("info.ordblks  = %d\n", info.ordblks);
    printf("info.hblks    = %d\n", info.hblks);
    printf("info.hblkhd   = %d\n", info.hblkhd);
    printf("info.uordblks = %d\n", info.uordblks);
    printf("info.fordblks = %d\n", info.fordblks);
    //~ free(p2);
    free(p1);
  }
  
  if(0) {
    int **mat2d, **tri2d;
    
    assert(tsc_alloc2d((void***) &mat2d,3,3,4) == NULL);
    
    for(int i = 0 ; i < 3 ; i++)
      for(int j = 0 ; j < 3 ; j++)
        mat2d[i][j] = i*100+j;
    
    for(int i = 0 ; i < 3 ; i++)
      for(int j = 0 ; j < 3 ; j++)
        printf("mat2d[%d][%d] = %d\n", i, j, mat2d[i][j]);
    
    for(int i = 0 ; i < 3 ; i++)
      for(int j = 0 ; j < 3 ; j++)
        mat2d[i][j] = mat2d[i][j] * mat2d[i][j];  // multiplied with itself
    
    free(mat2d);
    
    assert(tsc_alloc2d_irregular((void***) &tri2d, 3, (size_t [3]){1,2,3}, 4) == NULL);
    
    for(int i = 0 ; i < 3 ; i++)
      for(int j = 0 ; j < (i+1) ; j++)
        tri2d[i][j] = i*100+j;
    
    for(int i = 0 ; i < 3 ; i++)
      for(int j = 0 ; j < (i+1) ; j++)
        printf("mat2d[%d][%d] = %d\n", i, j, tri2d[i][j]);
    
    
    free(tri2d);
    
    char ***mat3d;
    
    // 3x3 16char string
    assert(tsc_alloc3d((void****) &mat3d, 3, 3, 16, 1) == NULL);
    for(int i = 0 ; i < 3 ; i++)
      for(int j = 0 ; j < 3 ; j++)
        sprintf(mat3d[i][j], "hello %d", i*100+j);
    
    for(int i = 0 ; i < 3 ; i++)
      for(int j = 0 ; j < 3 ; j++)
        printf("mat3d[%d][%d] = %s\n", i, j, mat3d[i][j]);
    
    free(mat3d);
  }
  
  {
    char *user, *pass;
    const char *estr;
    printf("user: "); assert(tsc_freadline(&user, stdin) == NULL);  tsc_strtrim_inplace(user);
    printf("pass: "); assert(tsc_getpass(&pass, stdin) == NULL);    tsc_strtrim_inplace(pass);
    printf("\n");
    printf("user: %s pass: %s\n", user, pass);
    
    
    free(user);
    free(pass);
  }
  
  return 0;
}
