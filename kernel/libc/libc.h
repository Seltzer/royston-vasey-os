#pragma once

#include <stddef.h>
#include "typedefs.h"


void memcpy(char* dst, const char* src, size_t size);

char* itoa(int val, int base, char* buffer);