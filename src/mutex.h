#ifndef __SYSIPC_MUTEX_H
#define __SYSIPC_MUTEX_H

namespace sysipc {

typedef struct _mutex_t *mutex_t;

mutex_t mutex_create(void);
void mutex_lock(mutex_t mtx);
void mutex_unlock(mutex_t mtx);
void mutex_destroy(mutex_t mtx);

};

#endif
