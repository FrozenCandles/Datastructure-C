#pragma once

#include "pubdef.h"

#define DSBINHEAP_BETTER_PRINT

#define DSHEAP_MIN_ELEM 0


/*===========*\
| Binary Heap |
\*===========*/

typedef struct DsBinHeap {
    size_t size;
    size_t elem_size;
    size_t length;
    void* body;
} * DsBinHeap;

typedef DsBinHeap* DsPriorityQueue;

#define DsBinHeap_New(type, size) \
        DsBinHeap_New_Inner(size, sizeof(type))
DsBinHeap DsBinHeap_New_Inner(size_t capacity, size_t elem_size);

void DsBinHeap_Delete(DsBinHeap heap);

bool DsBinHeap_IsEmpty(DsBinHeap heap);

bool DsBinHeap_IsFull(DsBinHeap heap);

void DsBinHeap_Insert(DsBinHeap heap, void* value);

void* DsBinHeap_FindMinValue(DsBinHeap heap);

void DsBinHeap_RemoveMin(DsBinHeap heap);

void DsBinHeap_PrintAsArray(DsBinHeap heap, void (*print_elem)(void* elem));

void DsBinHeap_PrintAsTree(DsBinHeap heap, char* (*elem_tostring)(void* value));

void CArray_PrintAsTree(const void* arr, size_t length, size_t elem_size, char* (*elem_tostring)(void* value));


/*============*\
| Leftist Heap |
\*============*/

typedef struct DsLeftHeapNode {
    void* elem;
    struct DsLeftHeapNode* left;
    struct DsLeftHeapNode* right;
    int Npl;
} * DsLeftHeapNode;

typedef struct DsLeftHeapHeader {
    struct DsLeftHeapNode* body;
    size_t elem_size;
} * DsLeftHeap;

#define DsLeftHeap_New(type) \
        DsLeftHeap_New_Inner(sizeof(type))
DsLeftHeap DsLeftHeap_New_Inner(size_t elem_size);

void DsLeftHeap_Clear(DsLeftHeap heap);

DsLeftHeapNode DsLeftHeapNode_Clear(DsLeftHeapNode heap);

void DsLeftHeap_Delete(DsLeftHeap heap);

DsLeftHeap DsLeftHeap_Merge(DsLeftHeap heap_A, DsLeftHeap heap_B);

void DsLeftHeap_RemoveMin(DsLeftHeap heap);

void* DsLeftHeap_FindMin(DsLeftHeap heap);

DsLeftHeap DsLeftHeap_Insert(DsLeftHeap heap, void* elem);

void DsLeftHeap_ExportGraphviz(DsLeftHeap tree, const char* dotfile_name,
                               const char* pngfile_name, void (*write_elem)(FILE* f, void* elem));

/* Belows are internal implementation */

void DsLeftHeapNode_SwapChildren(DsLeftHeapNode heapnode);

DsLeftHeapNode DsLeftHeapNode_Merge(DsLeftHeap heap_desc, DsLeftHeapNode heap_A, DsLeftHeapNode heap_B);

void DsLeftHeapNode_ExportGraphviz(DsLeftHeap tree, DsLeftHeapNode node,
                                   FILE* dotfile, void (*write_elem)(FILE* f, void* elem));


