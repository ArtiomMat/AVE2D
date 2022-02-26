// Artiom's C (not really) Compiler

#ifdef _WIN32
	#include <windows.h>

	#define PAGE_EXEC 0x10
	#define PAGE_READWRITE 0x04
#else
	#include <stdio.h>
	#include <sys/mman.h>

	#define PAGE_EXEC PROT_EXEC
	#define PAGE_READWRITE PROT_READ | PROT_WRITE
#endif

enum
{
	NULLTYPE = 0,
	INT8,
	INT16,
	INT32,
	INT64,
	UINT8,
	UINT16,
	UINT32,
	UINT64,
	FLOAT32,
	PTR32,
	PTR64,
};

typedef struct
{
	unsigned short msize;
	char *argstypes;
	unsigned short argscount;
	int (*call)();
} accf;

typedef struct
{
	char *name;

} avar;

const char *argregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d", "rcx", "rdx", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rax", "rbx", "rbp", "rsp", "rip"};

// Allocate pages for PTR the size of SIZE in bytes.
// Return the address of the allocated memory.
static void *map(void *ptr, unsigned short size, int flags)
{
	#ifdef _WIN32
		DWORD old;
		VirtualProtect(ptr, size, flags, &old);
		return ptr;
	#else
		return mmap(ptr, size, flags, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	#endif
}

static void protect(void *ptr, unsigned short size, int flags)
{
	#ifdef _WIN32
		DWORD old;
		VirtualProtect(ptr, size, flags, &old);
	#else
		mprotect(ptr, size, flags);
	#endif
}

// Free the pages allocated for PTR.
static void unmap(void *ptr, unsigned short size)
{
	#ifdef _WIN32
		VirtualFree(ptr, 0, MEM_RELEASE);
	#else
		munmap(ptr, size);
	#endif
}

// Add a C function to the global scope.
int acc_addglbl(int )

accf acc_compile(char *str, char *argsizes)
{
	int i, j, k;
	accf ret = {0};

	// Copy argsizes to ret.argstypes
	// Dirty hack because of the NULLTYPE=0
	ret.argscount = strlen(argsizes);
	ret.argstypes = malloc(ret.argscount);
	for (i = 0; i < ret.argscount; i++)
		ret.argstypes[i] = argsizes[i];
	
}
void acc_free(accf x);
