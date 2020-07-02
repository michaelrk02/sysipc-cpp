#ifndef __SYSIPC_MUTEX_H
#define __SYSIPC_MUTEX_H

#include <sysipc/base.h>

namespace sysipc {

typedef struct _mutex_t *mutex_t;

SYSIPC_API mutex_t mutex_create(void);
SYSIPC_API void mutex_lock(mutex_t mtx);
SYSIPC_API void mutex_unlock(mutex_t mtx);
SYSIPC_API void mutex_destroy(mutex_t mtx);

};

#endif
