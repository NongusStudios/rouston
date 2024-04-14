#pragma once

#include <stdlib.h>
#include <stdio.h>

void* memAlloc(size_t n);
void* memRealloc(void* ptr, size_t n);
void  memFree(void* ptr);

FILE* fileOpen(const char* path, const char* flags, size_t* length);

#define VEC(type) type* /*! Used as a type qualifier for vectors. */
#define VEC_INIT(type, ...) (type*)vecInit((type[]){__VA_ARGS__}, sizeof((type[]){__VA_ARGS__})/sizeof(type), sizeof(type)) /*! e.g clt_vec(int) v = ctl_vec_init(int, 1, 2, 3, 4, 5); */
#define VEC_INIT_EMPTY(type) (type*)vecInit(NULL, 0, sizeof(type)) /*! Initialises an empty vector of type. */
#define VEC_INIT_WITH_CAPACITY(type, n) (type*)vecWithCapacity((n), sizeof(type)) /*! Initialises an empty vector of type with capacity of n. */

#define VEC_FOREACH(type, v, iter) for(type* v = iter; v != vecEnd(iter); v++) /*! Allows for scuffed foreach emulation in C. */

#define VEC_REF(vec) ((void**)&vec) /*! Helper macro so you can do this 'ctl_vec_push(ctl_vptr(vec), ...)' instead of 'vecPush(&(void*)vec, ...)' */

/*! Used to point to either end of a vector. */
typedef enum VecPos {
    VecPos_FRONT, /*! Points to the first element of a vector. */
    VecPos_BACK,  /*! Points to the last element of a vector.  */
} VecPos;

/*! Length data of a vector. */
typedef struct VecData {
    size_t len; /*! Length of the vectors elements. */
    size_t cap; /*! Length of the vectors data. */
    size_t nbyte; /*! Size of an element in bytes. */
} VecData;

/*!
 * Creates a new vector.
 * @param data An array of elements to populate the vector with. Each element must be of type_len (bytes) and contain len elements.
 *        Note: data can be left NULL and the data will be zeroed.
 * @param len How many elements to allocate.
 * @param type_len Size of each element in bytes.
 * @return A pointer to the start of the vector's data.
*/
void* vecInit(void* data, size_t len, size_t type_len);

/*!
 * Creates a new vector with a chosen capacity, but a length of 0.
 * @param cap The capacity of the vector in elements.
 * @param type_len Size of each element in bytes.
 * @return A pointer to the start of the vector's data.
*/
void* vecWithCapacity(size_t cap, size_t type_len);

/*!
 * Frees the resources allocated by vecInit(...) and vecWithCapacity(...).
 * Not calling this before a vector goes out of scope will result in a memory leak.
 * @param vec The vector.
*/
void  vecFree(void* vec);

/*!
 * Sets the capacity of vec to cap.
 * @param ppvec Pointer to a vector.
 * @param cap Capacity to set vec to
*/
void vecReserve(void** ppvec, size_t cap);

/*!
 * Resizes vec to len
 * @param[in, out] vec Pointer to a vector.
 * @param[in] len Length to resize to.
*/
void vecResize(void** vec, size_t len);

/*!
 * Pushes an element to either end of vec.
 * @param[in, out] vec Pointer to a vector.
 * @param[in] pos Enum that sets which end of vec to push (CTL_VECPOS_BACK/CTL_VECPOS_FRONT).
 * @param[in] item Element data to push to vec.
*/
void vecPush(void** vec, VecPos pos, const void* item);

/*!
 * Removes an element from either end of vec.
 * @param[in, out] vec Pointer to a vector.
 * @param[in] pos Enum that sets which end of vec to push (CTL_VECPOS_BACK/CTL_VECPOS_FRONT).
 * @param[out] dst Used to return the value of the removed element.
*/
void vecPop(void** vec, VecPos pos, void* dst);

/*!
 * Inserts an element at index.
 * a = [0, 1, 2, 3, 4, 5]
 * a.insert(2: index, 11: item)
 * a: [0, 1, 11, 2, 3, 4, 5]
 * @param[in, out] vec Pointer to a vector.
 * @param[in] index The index that item will be inserted at.
 * @param[in] item The data that will be inserted at index.
*/
void vecInsert(void** vec, size_t index, const void* item);

/*!
 * Erases an element at index.
 * @param[in, out] vec Pointer to a vector.
 * @param[in] index The index to erase.
 * @param[out] dest Used to return the data at index (size=type_len).
*/
void vecErase(void** vec, size_t index, void* dst);

/*!
 * Clears the vector but keeps capacity.
 * @param[in, out] vec Pointer to a vector.
*/
void vecClear(void** vec);

/*!
 * Creates a clone of vec.
 * @param vec The vector to clone.
 * @return The cloned vector.
*/
void* vecClone(void* vec);

/*!
 * Gets the end of vec.
 * @return Pointer to the address after the last element.
*/
void* vecEnd(void* vec);

/*!
 * Gets the last element of vec.
 * @return Pointer to last element of vec.
*/
void* vecLast(void* vec);

/*!
 * Indexes vec by index.
 * @param vec Vector to index.
 * @param index What to index vec by.
 * @return Pointer to the index.
*/
void* vecIndex(void* vec, size_t index);

/*!
 * Gets the length of vec.
 * @param vec Vector that size will be fetched from.
 * @return The length of vec.
*/
size_t vecLen(void* vec);

/*!
 * Gets the capacity of vec.
 * @param vec Vector that capacity will be fetched from.
 * @return The capacity of vec.
*/
size_t vecCapacity(void* vec);