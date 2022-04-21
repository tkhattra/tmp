// compile-time assert
#define ct_assert(e) ((void)sizeof(char[1 - 2*!(e)]))
