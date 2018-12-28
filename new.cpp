#include "new.h"
#include "malloc.h"

void *__gxx_personality_v0;
void *_Unwind_Resume;

extern "C" void __attribute__((weak)) __cxa_throw_bad_array_new_length() {
}

void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}

void operator delete(void *p, long unsigned int size)
{
  free(p);
}

void operator delete[](void *p, long unsigned int size)
{
  free(p);
}
