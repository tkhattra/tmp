/*
    gcc -o sizeof.64 sizeof.c
    gcc -m32 -o sizeof.32 sizeof.c
*/ 

#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define SZ(x) printf("%2zd " #x "%s\n", sizeof(x), (0 > (x) -1) ? "" : " (unsigned)")

typedef void function(void);

int main()
{
   SZ(void*);
   SZ(function*);
   SZ(char);
   SZ(short);
   SZ(int);
   SZ(long);
   SZ(long long);
   SZ(float);
   SZ(double);
   SZ(long double);
   SZ(time_t);
   SZ(suseconds_t);
   SZ(pid_t);
   SZ(wchar_t);
   SZ(size_t);
   SZ(ptrdiff_t);
   SZ(ssize_t);
   SZ(intmax_t);
   SZ(uintmax_t);
   SZ(uintptr_t);

   return 0;
}
