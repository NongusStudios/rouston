#include "inc/utils.h"

#include <assert.h>
#include <string.h>

VecData* getVecData(void* vec){
    assert(vec);
    return (VecData*)vec - 1;
}
void* getVecStart(VecData* vdata){
    assert(vdata);
    return (void*)(vdata + 1);
}

size_t vecTypelen(void* vec){
    return getVecData(vec)->nbyte;
}

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

FILE* fileOpen(const char* path, const char* flags, size_t* length) {
    FILE* fp;
    fopen_s(&fp, path, flags);
    fseek(fp, 0, SEEK_END);
    *length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return fp;
}

void* vecInit(void* data, size_t len, size_t type_len){
    size_t byte_len = sizeof(VecData) + len * type_len;
    VecData* vec = memAlloc(byte_len);
    memset(vec, 0, byte_len);

    vec->len = len;
    vec->cap = len;
    vec->nbyte = type_len;

    void* start = getVecStart(vec);
    memcpy(start, data, len*type_len * (data != NULL));

    return start;
}
void* vecWithCapacity(size_t cap, size_t type_len){
    size_t byte_len = sizeof(VecData) + cap * type_len;
    VecData* vec = memAlloc(byte_len);
    memset(vec, 0, byte_len);

    vec->len = 0;
    vec->cap = cap;
    vec->nbyte = type_len;

    void* start = getVecStart(vec);

    return start;
}
void vecFree(void* vec){
    if(!vec) return;
    memFree(getVecData(vec));
}

void vecReserve(void** ppvec, size_t cap){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;

    VecData* data = getVecData(vec);
    size_t old_cap = data->cap;

    data = memRealloc(data, sizeof(VecData) + cap * data->nbyte);
    data->cap = cap;
    if(data->len > cap) data->len = cap;

    void* start = getVecStart(data);

    if(cap > old_cap) memset(vecIndex(start, old_cap), 0, (cap - old_cap) * data->nbyte);

    *ppvec = start;
}
void vecResize(void** ppvec, size_t len){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;

    VecData* data = getVecData(vec);
    if(len > data->cap) {
        vecReserve(&vec, ((data->cap+1) * 2 > len) ? (data->cap+1) * 2 : len);
        data = getVecData(vec);
    }

    data->len = len;

    *ppvec = vec;
}

void vecPush(void** ppvec, VecPos pos, const void* item){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;

    switch(pos){
        case VecPos_BACK: {
            vecResize(&vec, vecLen(vec) + 1);
            memmove(vecLast(vec), item, vecTypelen(vec) * (item != NULL));
            break;
        }
        case VecPos_FRONT: {
            vecResize(&vec, vecLen(vec) + 1);
            memmove(vecIndex(vec, 1), vec, vecLen(vec) * vecTypelen(vec));
            memmove(vec, item, vecTypelen(vec) * (item != NULL));
            break;
        }
    }

    *ppvec = vec;
}
void vecPop(void** ppvec, VecPos pos, void* dst){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;

    switch(pos){
        case VecPos_BACK: {
            memmove(dst, vecIndex(vec, vecLen(vec) - 1), vecTypelen(vec) * (dst != NULL));
            vecResize(&vec, vecLen(vec) - 1);
            break;
        }
        case VecPos_FRONT: {
            void* clone = vecClone(vec);
            vecResize(&vec, vecLen(vec) - 1);
            memcpy(vec, vecIndex(clone, 1), vecLen(vec) * vecTypelen(vec));
            memcpy(dst, clone, vecTypelen(vec) * (dst != NULL));
            vecFree(clone);
            break;
        }
    }

    *ppvec = vec;
}
void vecInsert(void** ppvec, size_t index, const void* item){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;
    assert(index < vecLen(vec));

    vecResize(&vec, vecLen(vec) + 1);
    memmove(vecIndex(vec, index + 1), vecIndex(vec, index), (vecLen(vec) - index) * vecTypelen(vec));
    memmove(vecIndex(vec, index), item, vecTypelen(vec) * (item != NULL));

    *ppvec = vec;
}
void vecErase(void** ppvec, size_t index, void* dst){
    assert(ppvec);
    assert(*ppvec);

    void* vec = *ppvec;
    assert(index < vecLen(vec));
    void* clone = vecClone(vec);

    vecResize(&vec, vecLen(vec) - 1);
    memcpy(vecIndex(vec, index), vecIndex(clone, index + 1), (vecLen(vec) - index) * vecTypelen(vec));
    memcpy(dst, vecIndex(clone, index), vecTypelen(vec) * (dst != NULL));
    vecFree(clone);

    *ppvec = vec;
}
void vecClear(void** vec){
    vecResize(vec, 0);
}

void* vecClone(void* vec){
    return vecInit(vec, vecLen(vec), vecTypelen(vec));
}

void* vecEnd(void* vec){
    assert(vec);
    return vecIndex(vec, vecLen(vec));
}
void* vecLast(void* vec){
    assert(vec);
    assert(vecLen(vec) > 0);
    return vecIndex(vec, vecLen(vec) - 1);
}
void* vecIndex(void* vec, size_t index){
    assert(vec);
    return (void*)((char*)vec + index * vecTypelen(vec));
}

size_t vecLen(void* vec){
    assert(vec);
    return getVecData(vec)->len;
}
size_t vecCapacity(void* vec){
    assert(vec);
    return getVecData(vec)->cap;
}