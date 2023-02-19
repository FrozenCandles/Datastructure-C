//
// Created by Hello on 2022-11-22.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "array.h"




DsArray DsArray_New_Inner(size_t size, size_t elem_size) {

    DsArray arr = (DsArray)malloc(sizeof(struct DsArray));
    if (arr == NULL) exit(1);
    arr->body = malloc(size * elem_size);
    arr->length = 0;
    arr->size = size;
    arr->elem_size = elem_size;
    return arr;
}


void DsArray_Delete(DsArray arr) {
    free(arr->body);
    free(arr);
}


static inline void* DsArray_GetItem(const struct DsArray* arr, size_t index) {
    return (void*)(((uint8_t*)arr->body) + arr->elem_size * index);
}



#define DsArray_PrintStart printf("Array { ")

#define DsArray_PrintSep printf(", ")

#define DsArray_PrintEnd printf(" } length = %zu, size = %zu\n", arr->length, arr->size)

#define DsArray_PrintEllipsis printf(" ... ")

void DsArray_Print(DsArray arr, void (*print_elem)(void* elem)) {
    DsArray_PrintStart;
    if (arr->length < 20) {
        for range(i, arr->length) {
            if (i)
                DsArray_PrintSep;
            print_elem(DsArray_GetItem(arr, i));
        }
        DsArray_PrintEnd;
    }
    else {
        for range(i, 10) {
            if (i)
                DsArray_PrintSep;
            print_elem(DsArray_GetItem(arr, i));
        }
        DsArray_PrintSep;
        DsArray_PrintEllipsis;
        for (size_t i = arr->length - 10; i < arr->length; i++) {
            DsArray_PrintSep;
            print_elem(DsArray_GetItem(arr, i));
        }
        DsArray_PrintEnd;
    }

}



void DsArray_EnsureSize(DsArray arr, const size_t new_size) {

    if (new_size <= arr->size)
        return;

    /* size too large */
    size_t actual_new_size = (!arr->size)
        ? new_size
        : arr->size * DsArray_EnsureSize_NewSize;

    if (new_size > actual_new_size)
        actual_new_size = new_size;
    void* new_body = (void*)malloc(arr->elem_size * actual_new_size);
    if (new_body == 0) exit(1);
    if (arr->size)
        memcpy(new_body, arr->body, arr->elem_size * arr->size);
    if (arr->body)
        free(arr->body);
    arr->body = new_body;
    arr->size = actual_new_size;
}


void* DsArray_GetLastItem(const struct DsArray* arr) {
    if (!arr->length)
        return NULL;
    return DsArray_GetItem(arr, arr->length - 1);
}


/**
 *
 * @param arr
 * @param elem
 * @param elem_size
 * @return index of new element
 */
size_t DsArray_PushBack_Inner(struct DsArray* arr, const void* elem) {

    DsArray_EnsureSize(arr, ++arr->length);
    memcpy(DsArray_GetLastItem(arr), elem, arr->elem_size);

    return arr->length - 1;
}


size_t DsArray_ConcatBack(DsArray arr, const void* src, const size_t length) {

    if (!arr)
        return 0;
    if (!src || !length)
        return arr->length;

    DsArray_EnsureSize(arr, arr->length + length);
    memcpy(DsArray_GetItem(arr, arr->length), src,
        arr->elem_size * length);
    arr->length += length;

    return arr->length;
}


size_t DsArray_ConcatArrayBack(DsArray dst, DsArray src) {
    return DsArray_ConcatBack(dst, src->body, src->length);
}


void DsArray_InsertAt_Inner(DsArray arr, const void* value, const size_t index) {

    if (index > arr->length)
        return;
    else if (index == arr->length) {
        DsArray_PushBack(arr, value);
        return;
    }
    else {
        DsArray_EnsureSize(arr, ++arr->length);

        void* new_item = DsArray_GetItem(arr, index);

        memmove(DsArray_GetItem(arr, index + 1), new_item,
            (arr->length - index) * arr->elem_size);
        memcpy(new_item, value, arr->elem_size);
    }

}


void DsArray_RemoveAt(DsArray arr, size_t index) {

    if (index >= arr->length)
        return;
    
    if (--arr->length == 0) {
        arr->length = 0;
        return;
    }

    //void* removed = malloc(arr->elem_size);
    //if (removed)
    //    memcpy(removed, DsArray_GetItem(arr, index), arr->elem_size);

    memmove(DsArray_GetItem(arr, index), 
            DsArray_GetItem(arr, index + 1),
            arr->elem_size * (arr->length- index));

}


size_t DsArray_FindValue(DsArray arr, const void* value, const size_t start_index) {

    if (start_index > arr->length)
        return DSARRAY_INVALID_INDEX;

    for (size_t i = start_index; i < arr->length; i++) {
        if (memcmp(DsArray_GetItem(arr, i), value, arr->elem_size) == 0)
            return i;
    }

    return DSARRAY_INVALID_INDEX;
}


bool DsArray_RemoveValue_Inner(DsArray arr, const void* value) {

    size_t index = DsArray_FindValue(arr, value, 0);
    if (index != DSARRAY_INVALID_INDEX) {
        DsArray_RemoveAt(arr, index);
        return true;
    }
    return false;
}


DsArray DsArray_FromCArray_Inner(void* src, size_t src_size, size_t size, size_t elem_size) {
    DsArray arr = DsArray_New_Inner(size, elem_size);
    memcpy(src, arr->body, src_size * elem_size);
    arr->size = size;
    arr->length = src_size;
    arr->elem_size = elem_size;
    return arr;
}

