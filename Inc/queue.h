#pragma once


#include "pubdef.h"


typedef struct DsQueue {
    size_t length;
    size_t size;
    size_t elem_size;
    size_t front;
    size_t rear;
    void* body;
} * DsQueue;


#define DsQueue_New(type, size) \
        DsQueue_New_Inner(size, sizeof(type))
DsQueue DsQueue_New_Inner(size_t capacity, size_t elem_size);

#define DsQueue_FromCArray(values, length, capacity)\
	    DsQueue_FromCArray_Inner(values, length, sizeof(*(values)), capacity)
DsQueue DsQueue_FromCArray_Inner(const void* values, size_t length, size_t elem_size, size_t capacity);

void DsQueue_Delete(DsQueue q);

bool DsQueue_IsEmpty(DsQueue q);

void DsQueue_Clear(DsQueue q);

bool DsQueue_IsFull(DsQueue q);

void DsQueue_Enqueue(DsQueue q, void* value);

void* DsQueue_Dequeue(DsQueue q);


#define DsQueue_PrintStart    printf("Queue | ")
#define DsQueue_PrintSep      printf(" > ")
#define DsQueue_PrintEnd      printf(" | } length = %zu, size = %zu\n", q->length, q->size)
#define DsQueue_PrintEllipsis printf(" ... ")
#define DsQueue_PrintEmptyPos printf("* ")

void DsQueue_Print(DsQueue q, void (*print_elem)(void* elem));
