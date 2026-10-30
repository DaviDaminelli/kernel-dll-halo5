#include <windows.h>
#include <malloc.h>

// Generous alignment, since Xbox allocations can request aligned memory.
#define XMEM_ALIGNMENT 256

PVOID WINAPI XMemAlloc(SIZE_T size, ULONGLONG attributes)
{
    (void)attributes;
    if (size == 0) size = 1;
    void *p = _aligned_malloc(size, XMEM_ALIGNMENT);
    if (p) ZeroMemory(p, size);
    return p;
}

VOID WINAPI XMemFree(PVOID address, ULONGLONG attributes)
{
    (void)attributes;
    if (address) _aligned_free(address);
}