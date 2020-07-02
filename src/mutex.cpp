#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "mutex.h"

namespace sysipc {

struct _mutex_t {
#ifdef _WIN32
    CRITICAL_SECTION cs;
#else
    pthread_mutex_t m;
#endif
};

mutex_t mutex_create(void) {
    _mutex_t *mtx = new _mutex_t;
#ifdef _WIN32
    InitializeCriticalSection(&mtx->cs);
#else
    pthread_mutex_init(&mtx->m, NULL);
#endif
    return (mutex_t)mtx;
}

void mutex_lock(mutex_t mtx) {
#ifdef _WIN32
    EnterCriticalSection(&((_mutex_t *)mtx)->cs);
#else
    pthread_mutex_lock(&((_mutex_t *)mtx)->m);
#endif
}

void mutex_unlock(mutex_t mtx) {
#ifdef _WIN32
    LeaveCriticalSection(&((_mutex_t *)mtx)->cs);
#else
    pthread_mutex_unlock(&((_mutex_t *)mtx)->m);
#endif
}

void mutex_destroy(mutex_t mtx) {
#ifdef _WIN32
    DeleteCriticalSection(&((_mutex_t *)mtx)->cs);
#else
    pthread_mutex_destroy(&((_mutex_t *)mtx)->m);
#endif
    delete mtx;
}

};
