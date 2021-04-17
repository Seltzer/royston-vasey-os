#include <stdbool.h>
#include "libc.h"


void memcpy(char* dest, const char* src, size_t size)
{
	for (int i = 0; i < size; i++) {
		*(dest + i) = *(src + i);
	}
}


/*
 * Bad itoa implementation, badly in need of malloc
 *
 * @param buffer - 32 elements in length, exactly
 */
char* itoa(int val, int base, char* buffer) 
{
	buffer[31] = 0;
	
	int i = 30;
	
	for (; val && i; --i, val /= base)
		buffer[i] = "0123456789abcdef"[val % base];

	return &buffer[i + 1];
}