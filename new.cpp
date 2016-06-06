#include "new.h"
#include "malloc.h"

void *__gxx_personality_v0;
void *_Unwind_Resume;

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
