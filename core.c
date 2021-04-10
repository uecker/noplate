
#include <stdlib.h>

#include "core.h"


extern void xfree(const void* x)
{
	free((void*)x);
}

