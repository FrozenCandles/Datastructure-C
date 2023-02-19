#pragma once

#include "pubdef.h"

#include "linkedlist.h"

/*=====*\
| Stack |
\*=====*/

typedef struct DsStack {
    size_t size;
    size_t elem_size;
    size_t top;
    void* body;
} * DsStack;

#define DSSTACK_NULLTOP (0U)

#define DSSTACK_MINSIZE (2U)



#define DsStack_New(type, size) \
        DsStack_New_Inner(size, sizeof(type))
DsStack DsStack_New_Inner(size_t capacity, size_t elem_size);

void DsStack_Delete(DsStack s);

bool DsStack_IsFull(DsStack s);

void DsStack_Push(DsStack s, void* value);

bool DsStack_IsEmpty(DsStack s);

void DsStack_Pop(DsStack s);

void* DsStack_Top(DsStack s);

void* DsStack_PopTop(DsStack s);

void DsStack_Clear(DsStack s);


/*============*\
| Linked Stack |
\*============*/

typedef DsForwardList DsLinkedStack;
typedef DsForwardListNode DsLinkedStackNode;


#define DsLinkedStack_New DsForwardList_New

#define DsLinkedStack_Delete DsForwardList_Delete

#define DsLinkedStack_IsEmpty DsForwardList_IsEmpty

#define DsLinkedStack_Push DsForwardList_PushFront

#define DsLinkedStack_Pop DsForwardList_PopFront

void* DsLinkedStack_Top(DsLinkedStack s);

#define DsLinkedStack_Clear DsForwardList_Clear
