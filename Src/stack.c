
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"



static inline void* DsStack_GetComputedElem(DsStack s,
                                            size_t pos) {
    return (uint8_t*)(s->body) + pos * s->elem_size;
}

static inline void DsStack_Assign(DsStack s,
                                  size_t pos,
                                  void* value) {
    memcpy(DsStack_GetComputedElem(s, pos), value, s->elem_size);
}

static inline int DsStack_Compare(DsStack s,
                                  size_t pos,
                                  void* value) {
    return memcmp(DsStack_GetComputedElem(s, pos), value, s->elem_size);
}




DsStack DsStack_New_Inner(size_t capacity, size_t elem_size) {
    if (capacity < DSSTACK_MINSIZE) {
        fputs("Stack size too small!!", stderr);
        exit(EXIT_FAILURE);
    }
    DsStack s = malloc(sizeof(struct DsStack));
    check_null(s);

    s->body = malloc(elem_size * capacity);
    check_null(s->body);

    s->top = DSSTACK_NULLTOP;
    s->size = capacity;
    s->elem_size = elem_size;
    return s;
}

/**
 *
 * @param s
 */
void DsStack_Clear(DsStack s) {
    s->top = DSSTACK_NULLTOP;
}


void DsStack_Delete(DsStack s) {
    if (s != NULL) {
        free(s->body);
        free(s);
    }
}


bool DsStack_IsFull(DsStack s) {
    return s->top >= s->size - 1;
}

void DsStack_Push(DsStack s, void* value) {
    if (DsStack_IsFull(s))
        return;  // stack full
    DsStack_Assign(s, ++s->top, value);
}


bool DsStack_IsEmpty(DsStack s) {
    return s->top == DSSTACK_NULLTOP;
}

void DsStack_Pop(DsStack s) {
    if (DsStack_IsEmpty(s))
        return;  // stack empty
    s->top--;
}


void* DsStack_Top(DsStack s) {
    if (!DsStack_IsEmpty(s))
        return DsStack_GetComputedElem(s, s->top);
    fputs("Try to get value of an empty stack", stderr);
    return NULL; // return value to avoid warning
}

void* DsStack_PopTop(DsStack s) {
    if (!DsStack_IsEmpty(s))
        return DsStack_GetComputedElem(s, s->top--);
    return NULL;  // return value to avoid warning
}


/*============*\
| Linked Stack |
\*============*/

void* DsLinkedStack_Top(DsLinkedStack s) {
    return s->next->elem;
}