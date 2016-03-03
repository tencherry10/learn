#include <stdio.h>
#include "klist.h"
#include "kvec.h"

//~ #define __int_free(x)
//~ KLIST_INIT(l32, int, __int_free)

//~ #define __intp_free(x) free(x)
//~ KLIST_INIT(p32, int *, __int_free)


int main()
{
	//~ klist_t(l32) *kl;
	//~ kliter_t(l32) *p;
	//~ kl = kl_init(l32);
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl->mp->cnt, kl->mp->n, kl->mp->max);
	//~ *kl_pushp(l32, kl) = 1;
	//~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl->mp->cnt, kl->mp->n, kl->mp->max);
  //~ *kl_pushp(l32, kl) = 10;
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl->mp->cnt, kl->mp->n, kl->mp->max);
	//~ kl_shift(l32, kl, 0);
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl->mp->cnt, kl->mp->n, kl->mp->max);
  //~ *kl_pushp(l32, kl) = 33;
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl->mp->cnt, kl->mp->n, kl->mp->max);  
	//~ for (p = kl_begin(kl); p != kl_end(kl); p = kl_next(p))
		//~ printf("%d\n", kl_val(p));
	//~ kl_destroy(l32, kl);
  
  //~ int * a;
	//~ klist_t(p32) *kl2;
	//~ kliter_t(p32) *p2;
	//~ kl2 = kl_init(p32);
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl2->mp->cnt, kl2->mp->n, kl2->mp->max);
  //~ a = malloc(sizeof(int)); *a = 1;
	//~ *kl_pushp(p32, kl2) = a;
	//~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl2->mp->cnt, kl2->mp->n, kl2->mp->max);
  //~ a = malloc(sizeof(int)); *a = 10;
  //~ *kl_pushp(p32, kl2) = a;
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl2->mp->cnt, kl2->mp->n, kl2->mp->max);
	//~ kl_shift(p32, kl2, a);
  //~ free(a);
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl2->mp->cnt, kl2->mp->n, kl2->mp->max);
  //~ a = malloc(sizeof(int)); *a = 33;
  //~ *kl_pushp(p32, kl2) = a;
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl2->mp->cnt, kl2->mp->n, kl2->mp->max);
	//~ for (p2 = kl_begin(kl2); p2 != kl_end(kl2); p2 = kl_next(p2))
		//~ printf("%d\n", *kl_val(p2));

	//~ kl_shift(p32, kl2, a);
  //~ free(a);
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl2->mp->cnt, kl2->mp->n, kl2->mp->max);
	//~ kl_shift(p32, kl2, a);
  //~ free(a);
  //~ printf("kl->mp->cnt: %d kl->mp->n: %d kl->mp->max: %d\n", kl2->mp->cnt, kl2->mp->n, kl2->mp->max);
  
	//~ kl_destroy(p32, kl2);
  
  size_t x = 33;
  kv_roundup32(x);
  printf("x = %d\n", x);
  
	return 0;
}
