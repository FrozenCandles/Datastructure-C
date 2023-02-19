#pragma once

#include "pubdef.h"

typedef struct DsForwardListNode {
	struct DsForwardListNode* next;
	void* elem;
}* DsForwardListNode;

typedef struct DsForwardListHeader {
	DsForwardListNode next;
	size_t elem_size;
	size_t length;
}* DsForwardListHeader, * DsForwardList;


/* Construct and Deconstruct */

#define DsForwardList_New(type, size) DsForwardList_New_Inner(size, sizeof(type))
DsForwardList DsForwardList_New_Inner(size_t elem_size);

void DsForwardList_Delete(DsForwardList list);

#define DsForwardList_FromCArray(values, length)\
	DsForwardList_FromCArray_Inner(values, length, sizeof(*(values)))
DsForwardList DsForwardList_FromCArray_Inner(const void* values, size_t length, size_t elem_size);


/* Get list information */

bool DsForwardList_IsEmpty(DsForwardList list);

bool DsForwardList_IsLast(DsForwardListNode node);

#define DsForwardList_PrintStart		printf("ForwardList { ")
#define DsForwardList_PrintSep		printf(" > ")
#define DsForwardList_PrintEnd		printf(" > || } length = %zu\n", list->length)
#define DsForwardList_PrintEllipsis printf(" ... ")

void DsForwardList_Print(DsForwardList list, void (*print_elem)(void* elem));


/* Get node information */

DsForwardListNode DsForwardList_FindNode(DsForwardList list, void* value);

DsForwardListNode DsForwardList_FindPrevNode(DsForwardList list, void* value);


/* Node Adjust */

void DsForwardList_RemoveValue(DsForwardList list, void* value);

void DsForwardList_RemoveAt(DsForwardList list, size_t index);

DsForwardList DsForwardList_PushFront(DsForwardList list, void* value);

DsForwardList DsForwardList_PopFront(DsForwardList list);

DsForwardList DsForwardList_InsertAfter(DsForwardList list, DsForwardListNode prev, void* value);

void DsForwardList_SwapAt(DsForwardList list, size_t index);


/* List Adjust */

void DsForwardList_Clear(DsForwardList list);

DsForwardList DsForwardList_Reverse(DsForwardList list);

DsForwardList DsForwardList_OrderedIntersection(DsForwardList a, DsForwardList b);

DsForwardList DsForwardList_OrderedUnion(DsForwardList a, DsForwardList b);