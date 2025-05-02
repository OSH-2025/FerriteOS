#define a_barrier a_barrier
static inline void a_barrier(void)
{
    __asm__ __volatile__("dsync" : : : "memory");
}

#define a_ll a_ll
static inline int a_ll(volatile int *p)
{
	int v = 0;
	return v;
}

#define a_sc a_sc
static inline int a_sc(volatile int *p, int v)
{
	int r = 0;
	return !r;
}

#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	int old = 0;
	return old;
}
