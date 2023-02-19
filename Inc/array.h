#pragma once

#include "pubdef.h"




typedef struct DsArray {
    void* body;
    size_t length;
    size_t size;
    size_t elem_size;
} * DsArray;


#define DSARRAY_INVALID_INDEX ((size_t)-1)


/* Construct and Deconstruct */

#define DsArray_EnsureSize_NewSize 2

#define DsArray_New(type, size) DsArray_New_Inner(size, sizeof(type))
DsArray DsArray_New_Inner(size_t size, size_t elem_size);

void DsArray_Delete(DsArray arr);


/* Get information */

#define CArray_GetLength(arr) (sizeof(arr) / sizeof(*arr))

void DsArray_Print(DsArray arr, void (*print_elem)(void* elem));

size_t DsArray_FindValue(DsArray arr, const void* value, const size_t start_index);

/* Size Adjust */

void DsArray_EnsureSize(DsArray arr, const size_t new_size);

void DsArray_Resize(DsArray arr, size_t new_size);

/* Element Adjust */

#define DsArray_InsertAt(arr, value, index) DsArray_InsertAt_Inner(arr, &(value), index)
void DsArray_InsertAt_Inner(DsArray arr, const void* value, size_t index);

void DsArray_RemoveAt(DsArray arr, size_t index);

#define DsArray_RemoveValue(arr, value) DsArray_RemoveValue_Inner(arr, &(value))
bool DsArray_RemoveValue_Inner(DsArray arr, const void* value);

#define DsArray_PushBack(arr, elem) DsArray_PushBack_Inner(arr, &(elem));
size_t DsArray_PushBack_Inner(struct DsArray* arr, const void* elem);

size_t DsArray_ConcatBack(DsArray arr, const void* src, size_t length);




#define DsArray_FromCArray(values, length, size)\
	DsArray_FromCArray_Inner(values, length, size, sizeof(*(values)))
DsArray DsArray_FromCArray_Inner(void* src, size_t src_size, size_t size, size_t elem_size);


