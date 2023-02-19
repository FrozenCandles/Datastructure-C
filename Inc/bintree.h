#pragma once


#include "pubdef.h"


/*==================*\
| Binary Search Tree |
\*==================*/

typedef struct DsBinSearchTreeNode {
    void* elem;
    struct DsBinSearchTreeNode* left;
    struct DsBinSearchTreeNode* right;
} * DsBinSearchTreeNode;


typedef struct DsBinSearchTreeHeader {
    struct DsBinSearchTreeNode* body;
    size_t elem_size;
} * DsBinSearchTree;


/* Construct */

#define DsBinSearchTree_New(type) \
        DsBinSearchTree_New_Inner(sizeof(type))
DsBinSearchTree DsBinSearchTree_New_Inner(size_t elem_size);

#define DsBinSearchTree_FromCArray(values, length)\
	    DsBinSearchTree_FromCArray_Inner(values, length, sizeof(*(values)))
DsBinSearchTree DsBinSearchTree_FromCArray_Inner(const void* values, size_t length, size_t elem_size);

DsBinSearchTree DsBinSearchTree_Clear(DsBinSearchTree tree);

void DsBinSearchTree_Delete(DsBinSearchTree tree);

/* Find */

DsBinSearchTreeNode DsBinSearchTree_FindNode(DsBinSearchTree tree, void* value);

DsBinSearchTreeNode DsBinSearchTree_FindMinNode(DsBinSearchTree tree);

DsBinSearchTreeNode DsBinSearchTree_FindMaxNode(DsBinSearchTree tree);

void* DsBinSearchTree_FindMaxValue(DsBinSearchTree tree);

void* DsBinSearchTree_FindMinValue(DsBinSearchTree tree);

void DsBinSearchTree_Print(DsBinSearchTree tree, void (*print_elem)(void* elem));

void DsBinSearchTree_ExportGraphviz(DsBinSearchTree tree, const char* dotfile_name, 
                                    const char* pngfile_name, void (*write_elem)(FILE* f, void* elem));

/* Adjust */

void DsBinSearchTree_Insert(DsBinSearchTree tree, void* value);

DsBinSearchTree DsBinSearchTree_Remove(DsBinSearchTree tree, void* value);


/* Belows are internal implementation */

DsBinSearchTreeNode DsBinSearchTreeNode_Clear(DsBinSearchTreeNode tree);

DsBinSearchTreeNode DsBinSearchTreeNode_FindNode(DsBinSearchTree tree, DsBinSearchTreeNode node, void* value);

DsBinSearchTreeNode DsBinSearchTreeNode_FindMinNode(DsBinSearchTree tree, DsBinSearchTreeNode node);

DsBinSearchTreeNode DsBinSearchTreeNode_FindMaxNode(DsBinSearchTree tree, DsBinSearchTreeNode node);

DsBinSearchTreeNode DsBinSearchTreeNode_Insert(DsBinSearchTree tree, DsBinSearchTreeNode node, void* value);

DsBinSearchTreeNode DsBinSearchTreeNode_Remove(DsBinSearchTree tree, DsBinSearchTreeNode node, void* value);

void DsBinSearchTreeNode_Print(DsBinSearchTree tree, DsBinSearchTreeNode node, size_t depth, void (*print_elem)(void* elem));

void DsBinSearchTreeNode_ExportGraphviz(DsBinSearchTree tree, DsBinSearchTreeNode node,
                                        FILE* dotfile, void (*write_elem)(FILE* f, void* elem));

/*==============================*\
| Adelson-Velsky and Landis Tree |
\*==============================*/


typedef struct DsAvlTreeNode {
    void* elem;
    struct DsAvlTreeNode* left;
    struct DsAvlTreeNode* right;
    int height;
} * DsAvlTreeNode;


typedef struct DsAvlTreeHeader {
    struct DsAvlTreeNode* body;
    size_t elem_size;
} * DsAvlTree;

/* Functions */

#define DsAvlTree_New(type) \
        DsAvlTree_New_Inner(sizeof(type))
DsAvlTree DsAvlTree_New_Inner(size_t elem_size);

#define DsAvlTree_FromCArray(values, length)\
	    DsAvlTree_FromCArray_Inner(values, length, sizeof(*(values)))
DsAvlTree DsAvlTree_FromCArray_Inner(const void* values, size_t length, size_t elem_size);

DsAvlTree DsAvlTree_Clear(DsAvlTree tree);

void DsAvlTree_Delete(DsAvlTree tree);

DsAvlTreeNode DsAvlTree_FindMaxNode(DsAvlTree tree);

DsAvlTreeNode DsAvlTree_FindMinNode(DsAvlTree tree);

void* DsAvlTree_FindMaxValue(DsAvlTree tree);

void* DsAvlTree_FindMinValue(DsAvlTree tree);

void DsAvlTree_Insert(DsAvlTree tree, void* value);

void DsAvlTree_Remove(DsAvlTree tree, void* value);

void DsAvlTree_Print(DsAvlTree tree, void (*print_elem)(void* elem));

void DsAvlTree_ExportGraphviz(DsAvlTree tree, const char* dotfile_name,
                              const char* pngfile_name, void (*write_elem)(FILE* f, void* elem));

/* Belows are internal implementation */

DsAvlTreeNode DsAvlTreeNode_FindMaxNode(DsAvlTreeNode node);

DsAvlTreeNode DsAvlTreeNode_FindMinNode(DsAvlTreeNode node);

DsAvlTreeNode DsAvlTreeNode_Insert(DsAvlTree tree, DsAvlTreeNode node, void* value);

DsAvlTreeNode DsAvlTreeNode_Remove(DsAvlTree tree, DsAvlTreeNode node, void* value);

DsAvlTreeNode DsAvlTreeNode_Clear(DsAvlTreeNode tree);

void DsAvlTreeNode_Print(DsAvlTree tree, DsAvlTreeNode node, size_t depth, void (*print_elem)(void* elem));

void DsAvlTreeNode_ExportGraphviz(DsAvlTree tree, DsAvlTreeNode node,
                                  FILE* dotfile, void (*write_elem)(FILE* f, void* elem));

