#include "inc/state.h"

#include <memory.h>

AppState __gAppState;

void initAppState(){
    memset(&__gAppState, 0, sizeof(AppState));

}

AppState* getAppState() { return &__gAppState; }

void* memAlloc(size_t n){
    void* p = calloc(n, 1);
    if(!p && n > 0) {
        fprintf(stderr, "FATAL_ERROR: memAlloc failed to allocate more memory.");
        abort();
    }
    return p;
}
void* memRealloc(void* ptr, size_t n){
    void* p = realloc(ptr, n);
    if(!p && n > 0){
        fprintf(stderr, "FATAL_ERROR: memRealloc failed to reallocate more memory.");
        abort();
    }
    return p;
}
void  memFree(void* ptr){
    free(ptr);
}

bool handleError(const Error* err){
    switch(err->type){
        case ErrorType_SUCCESS: break;
        case ErrorType_WARNING: printf("WARNING: %s\n", err->what); break;
        case ErrorType_ERROR:
            fprintf(stderr, "ERROR: %s\n", err->what);
            return false;
    }
    return true;
}
