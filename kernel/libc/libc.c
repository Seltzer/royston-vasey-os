#include "libc.h"


void memcpy(char* dest, const char* src, size_t size)
{
	for (int i = 0; i < size; i++) {
		*(dest + i) = *(src + i);
	}
}