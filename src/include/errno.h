#ifndef ERRNO_H
#define ERRNO_H

#include "../../include/errno.h"

#ifdef __GNUC__
__attribute__((const))
#endif
hidden int *___errno_location(void) : itype(_Ptr<int>);

#undef errno
#define errno (*___errno_location())

#endif
