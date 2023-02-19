
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"



static inline void* DsQueue_GetComputedElem(DsQueue q,
                                            size_t pos) {
    return (uint8_t*)(q->body) + pos * q->elem_size;
}

static inline void DsQueue_Assign(DsQueue q,
                                  size_t pos,
                                  void* value) {
    memcpy(DsQueue_GetComputedElem(q, pos), value, q->elem_size);
}

static inline int DsQueue_Compare(DsQueue q,
                                  size_t pos,
                                  void* value) {
    return memcmp(DsQueue_GetComputedElem(q, pos), value, q->elem_size);
}



bool DsQueue_IsEmpty(DsQueue q) {
    return q->length == 0;
}


bool DsQueue_IsFull(DsQueue q) {
    return q->length == q->size;
}

/**
 * Clear a queue
 * @param q
 */
void DsQueue_Clear(DsQueue q) {
    q->length = 0;
    q->front = 1;
    q->rear = 0;
}


DsQueue DsQueue_New_Inner(size_t capacity, size_t elem_size) {
    DsQueue q = malloc(sizeof(struct DsQueue));
    check_null(q);
    DsQueue_Clear(q);
    q->body = malloc(capacity * elem_size);
    check_null(q->body);
    q->size = capacity;
    q->elem_size = elem_size;
    return q;
}


DsQueue DsQueue_FromCArray_Inner(const void* values, 
                                 size_t length, 
                                 size_t elem_size, 
                                 size_t capacity) {
    DsQueue q = DsQueue_New_Inner(capacity, elem_size);
    for (size_t i = 0; i < length; i++)
        DsQueue_Enqueue(q, (uint8_t*)values + i * elem_size);
    return q;
}

/**
 * If front or rear reaches the end, go back to beginning
 * @param pos
 * @param q
 * @return
 */
static int DsQueue_Succ(DsQueue q, size_t pos) {
    if (++pos == q->size)
        pos = 0;
    return pos;
}

void DsQueue_Enqueue(DsQueue q, void* value) {
    if (DsQueue_IsFull(q))
        return;  // queue full
    q->length++;
    q->rear = DsQueue_Succ(q, q->rear);
    DsQueue_Assign(q, q->rear, value);
}


void* DsQueue_Dequeue(DsQueue q) {
    void* elem;
    if (DsQueue_IsEmpty(q)) {
        fputs("Try to get value of an empty queue", stderr);
        return NULL;  // return to avoid warning
    }
    q->length--;
    elem = DsQueue_GetComputedElem(q, q->front);
    q->front = DsQueue_Succ(q, q->front);
    return elem;
}


void DsQueue_Delete(DsQueue q) {
    free(q->body);
    free(q);
}


/*
 * TODO: Add support for printing very long queue
 */
void DsQueue_Print(DsQueue q, void (*print_elem)(void* elem)) {
    DsQueue_PrintStart;
    if (q->front < q->rear) {
        for (size_t i = 0; i < q->size; i++)
            if (i < q->front)
                DsQueue_PrintEmptyPos;
            else if (i == q->front) {
                putchar('<');
                print_elem(DsQueue_GetComputedElem(q, i));
                putchar(' ');
            }
            else if (i < q->rear) {
                print_elem(DsQueue_GetComputedElem(q, i));
                putchar(' ');
            }
            else if (i == q->rear) {
                print_elem(DsQueue_GetComputedElem(q, i));
                putchar('>');
                putchar(' ');
            }
            else
                DsQueue_PrintEmptyPos;
        DsQueue_PrintEnd;
    }
    else {
        for (size_t i = 0; i < q->size; i++)
            if (i < q->rear) {
                print_elem(DsQueue_GetComputedElem(q, i));
                putchar(' ');
            }
            else if (i == q->rear) {
                print_elem(DsQueue_GetComputedElem(q, i));
                putchar('>');
                putchar(' ');
            }
            else if (i < q->front)
                DsQueue_PrintEmptyPos;
            else if (i == q->front) {
                putchar('<');
                print_elem(DsQueue_GetComputedElem(q, i));
                putchar(' ');
            }
            else {
                print_elem(DsQueue_GetComputedElem(q, i));
                putchar(' ');
            }
        DsQueue_PrintEnd;
    }
}