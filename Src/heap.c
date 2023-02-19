

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "heap.h"

#pragma warning(disable:4996)


/*==========*\
| Binary Heap|
\*==========*/

static inline void* DsBinHeap_GetComputedElem(DsBinHeap h,
                                              size_t pos) {
    return (uint8_t*)(h->body) + pos * h->elem_size;
}

static inline void DsBinHeap_Assign(DsBinHeap h,
                                    size_t pos,
                                    void* value) {
    memcpy(DsBinHeap_GetComputedElem(h, pos), value, h->elem_size);
}

static inline int DsBinHeap_Compare(DsBinHeap h,
                                    size_t pos,
                                    void* value) {
    return memcmp(DsBinHeap_GetComputedElem(h, pos), value, h->elem_size);
}


DsBinHeap DsBinHeap_New_Inner(size_t capacity, size_t elem_size) {
    DsBinHeap heap = malloc(sizeof(struct DsBinHeap));
    check_null(heap);
    heap->body = malloc(elem_size * (capacity + 1));
    check_null(heap->body);
    heap->size = capacity;
    heap->elem_size = elem_size;
    heap->length = 0;
    
    memset(heap->body, DSHEAP_MIN_ELEM, elem_size);
    return heap;
}

bool DsBinHeap_IsEmpty(DsBinHeap heap) {
    return heap->length == 0;
}

bool DsBinHeap_IsFull(DsBinHeap heap) {
    return heap->length == heap->size;
}


void DsBinHeap_Insert(DsBinHeap heap, void* elem) {
    if (DsBinHeap_IsFull(heap))
        return;  // out of space
    size_t i;
    for (i = ++heap->length; DsBinHeap_Compare(heap, i / 2, elem) > 0; i /= 2)
        DsBinHeap_Assign(heap, i, DsBinHeap_GetComputedElem(heap, i / 2));
    DsBinHeap_Assign(heap, i, elem);
}




void DsBinHeap_RemoveMin(DsBinHeap heap) {
    if (DsBinHeap_IsEmpty(heap))
        return;   // empty heap
    void* last = DsBinHeap_GetComputedElem(heap, heap->length--);
    size_t i = 1;
    while (i * 2 <= heap->size) {
        size_t child = i * 2;      // find smaller child
        if (child != heap->length &&
            DsBinHeap_Compare(heap, child + 1, DsBinHeap_GetComputedElem(heap, child)) < 0)
            child++;
        if (DsBinHeap_Compare(heap, child, last) < 0)
            DsBinHeap_Assign(heap, i, DsBinHeap_GetComputedElem(heap, child));
        else
            break;
        i = child;
    }
    DsBinHeap_Assign(heap, i, last);
}


void* DsBinHeap_FindMinValue(DsBinHeap heap) {
    if (DsBinHeap_IsEmpty(heap))
        return NULL;
    return DsBinHeap_GetComputedElem(heap, 1);
}


void DsBinHeap_Delete(DsBinHeap heap) {
    free(heap->body);
    free(heap);
}


#define DsBinHeap_PrintStart printf("BinaryHeap { ")

#define DsBinHeap_PrintSep printf(", ")

#define DsBinHeap_PrintEnd printf(" } length = %zu, size = %zu\n", heap->length, heap->size)

#define DsBinHeap_PrintEllipsis printf(" ... ")

void DsBinHeap_PrintAsArray(DsBinHeap heap, void (*print_elem)(void* elem)) {
    DsBinHeap_PrintStart;
    if (heap->length < 20) {
        for (size_t i = 1; i < heap->length; i++) {
            if (i)
                DsBinHeap_PrintSep;
            print_elem(DsBinHeap_GetComputedElem(heap, i));
        }
        DsBinHeap_PrintEnd;
    }
    else {
        for (size_t i = 1; i < heap->length; i++) {
            if (i)
                DsBinHeap_PrintSep;
            print_elem(DsBinHeap_GetComputedElem(heap, i));
        }
        DsBinHeap_PrintSep;
        DsBinHeap_PrintEllipsis;
        for (size_t i = heap->length - 10; i < heap->length; i++) {
            DsBinHeap_PrintSep;
            print_elem(DsBinHeap_GetComputedElem(heap, i));
        }
        DsBinHeap_PrintEnd;
    }

}


static int pow2i(int a) {
    int r = 1;
    while (a--) {
        r *= 2;
    }
    return r;
}

/**
 * refrence: https://stackoverflow.com/questions/65140349/printing-binary-heap-in-tree-format-c-language
 * @param arr
 * @param length
 * @param elem_size
 * @param elem_tostring
 */
void CArray_PrintAsTree(const void* arr, size_t length, 
                        size_t elem_size, char* (*elem_tostring)(void* value)) {
    int longest_digits = 0;
    int tree_depth = 0;
    int pos = 0;
    int depth = 0;
    for (size_t i = 0; i < length; ++i) {
        if (*(int*)((uint8_t*)arr + i * elem_size) != -1) {
            char* elem_str = elem_tostring((uint8_t*)arr + i * elem_size);
            const int len = snprintf(NULL, 0, "%s", elem_str);
            free(elem_str);
            if (longest_digits < len)
                longest_digits = len;
        }
        if (pos == 0) {
            tree_depth++;
            pos = pow2i(depth++);
        }
        pos--;
    }
    pos = 0;
    depth = 0;
    const int additional_offset = 3;
    int max_width = pow2i(tree_depth) * (longest_digits + additional_offset);
    for (size_t i = 0; i < length; ++i) {
        const bool first = pos == 0;
        if (first) {
            pos = pow2i(depth);
            depth++;
        }
        const int count_elems = pow2i(depth);
        const int chunk = max_width / count_elems;
        const int width = chunk + (first ? -chunk / 2 : 0);
        const int pre_spaces = width - longest_digits;

        printf("%*s", pre_spaces, "");
        if ((uint8_t*)arr + i * elem_size == NULL)
            printf("%*s", longest_digits, "-");
        else {
            char* elem_str = elem_tostring((uint8_t*)arr + i * elem_size);
            printf("%*s", longest_digits, elem_str);
            free(elem_str);
        }
        if (pos == 1)
            printf("\n");
        pos--;
    }
}


void DsBinHeap_PrintAsTree(DsBinHeap heap, char* (*elem_tostring)(void*)) {
    CArray_PrintAsTree((uint8_t*)heap->body + heap->elem_size, 
                       heap->length, 
                       heap->elem_size, 
                       elem_tostring);
    putchar('\n');
}



/*============*\
| Leftist Heap |
\*============*/


//#if sizeof intptr_t == 4
//    #define POINT_INT_TYPESIGNAL "%lu"
//#elif sizeof intptr_t == 8
//    #define POINT_INT_TYPESIGNAL "%llu"
//#endif


static inline int DsLeftHeap_Compare(DsLeftHeap heap,
                                     DsLeftHeapNode node,
                                     void* value) {
    return memcmp(node->elem, value, heap->elem_size);
}


static inline void DsLeftHeap_Assign(DsLeftHeap h,
                                     DsLeftHeapNode node,
                                     void* value) {
    memcpy(node->elem, value, h->elem_size);
}


DsLeftHeap DsLeftHeap_New_Inner(size_t elem_size) {
    DsLeftHeap tree = malloc(sizeof(struct DsLeftHeapHeader));
    check_null(tree);
    tree->body = NULL;
    tree->elem_size = elem_size;
    return tree;
}


void DsLeftHeap_Clear(DsLeftHeap heap) {
    heap->body = DsLeftHeapNode_Clear(heap->body);
}


DsLeftHeapNode DsLeftHeapNode_Clear(DsLeftHeapNode heap) {
    if (heap != NULL) {
        DsLeftHeapNode_Clear(heap->left);
        DsLeftHeapNode_Clear(heap->right);
        free(heap->elem);
        free(heap);
    }
    return NULL;
}


void DsLeftHeap_Delete(DsLeftHeap heap) {
    DsLeftHeap_Clear(heap);
    free(heap);
}


void DsLeftHeapNode_SwapChildren(DsLeftHeapNode heapnode) {
    DsLeftHeapNode tmp = heapnode->left;
    heapnode->left = heapnode->right;
    heapnode->right = tmp;
}


static DsLeftHeapNode DsLeftHeapNode_Merge_Inner(DsLeftHeap heap_desc, 
                                                 DsLeftHeapNode heap_A, 
                                                 DsLeftHeapNode heap_B) {
    if (heap_A->left == NULL)
        heap_A->left = heap_B;
    else {
        heap_A->right = DsLeftHeapNode_Merge(heap_desc, heap_A->right, heap_B);
        if (heap_A->left->Npl < heap_A->right->Npl)
            DsLeftHeapNode_SwapChildren(heap_A);
        heap_A->Npl = heap_A->right->Npl + 1;
    }
    return heap_A;
}


DsLeftHeapNode DsLeftHeapNode_Merge(DsLeftHeap heap_desc, 
                                    DsLeftHeapNode heap_A, 
                                    DsLeftHeapNode heap_B) {
    if (heap_A == NULL) return heap_B;
    if (heap_B == NULL) return heap_A;
    if (DsLeftHeap_Compare(heap_desc, heap_A, heap_B->elem) < 0)
        return DsLeftHeapNode_Merge_Inner(heap_desc, heap_A, heap_B);
    else
        return DsLeftHeapNode_Merge_Inner(heap_desc, heap_B, heap_A);
}


DsLeftHeap DsLeftHeap_Merge(DsLeftHeap heap_A, DsLeftHeap heap_B) {
    DsLeftHeap heap_M = DsLeftHeap_New_Inner(heap_A->elem_size);
    heap_M->body = DsLeftHeapNode_Merge(heap_A, heap_A->body, heap_B->body);
    heap_A->body = NULL;
    heap_B->body = NULL;
    return heap_M;
}



DsLeftHeap DsLeftHeap_Insert(DsLeftHeap heap, void* elem) {
    DsLeftHeapNode node = malloc(sizeof(struct DsLeftHeapNode));
    check_null(node);
    node->elem = malloc(heap->elem_size);
    check_null(node->elem);
    DsLeftHeap_Assign(heap, node, elem);
    node->left = node->right = NULL;
    heap->body = DsLeftHeapNode_Merge(heap, heap->body, node);
    return heap;
}


bool DsLeftHeap_IsEmpty(DsLeftHeap heap) {
    return heap == NULL;
}


/**
 * @brief
 * To get the minimum, use FindMin()
 * @param heap
 * @return leftheap new tree
 */
void DsLeftHeap_RemoveMin(DsLeftHeap heap) {
    if (DsLeftHeap_IsEmpty(heap))
        return;  // empty heap
    DsLeftHeapNode left = heap->body->left;
    DsLeftHeapNode right = heap->body->right;
    free(heap->body);
    heap->body = DsLeftHeapNode_Merge(heap, left, right);
}


void* DsLeftHeap_FindMin(DsLeftHeap heap) {
    if (!DsLeftHeap_IsEmpty(heap))
        return heap->body->elem;
    return NULL;  // return value to avoid warning
}


//static void LeftHeap_PrintNode(DsLeftHeapNode node, int depth) {
//    if (node/* && node->height > 0*/) {
//        for (int i = 0; i < depth; i++)
//            printf("-");
//        printf(" %d\n", node->elem);
//    }
//}
//
//void LeftHeap_Print(DsLeftHeap tree, int depth) {
//    if (tree) {
//        LeftHeap_Print(tree->left, depth + 1);
//        LeftHeap_PrintNode(tree, depth);
//        LeftHeap_Print(tree->right, depth + 1);
//    }
//}



void DsLeftHeap_ExportGraphviz(DsLeftHeap tree,
                               const char* dotfile_name,
                               const char* pngfile_name,
                               void (*write_elem)(FILE*, void*)) {
    FILE* dotfile = fopen(dotfile_name, "w");
    DsLeftHeapNode x;
    if ((x = tree->body)) {
        fputs("digraph G {\n", dotfile);
        fputs("    graph [nodesep=0.1]\n", dotfile);
        fputs("    node [shape=circle]\n", dotfile);
        fputs("    edge [arrowhead=vee]\n", dotfile);
        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            fprintf(dotfile, "%lu", x);//write_elem(dotfile, x->elem);
            fprintf(dotfile, "[group=");
            fprintf(dotfile, "%lu", x);//write_elem(dotfile, x->elem);
            fputs("]\n", dotfile);
        }
        DsLeftHeapNode_ExportGraphviz(tree, x, dotfile, write_elem);
        fputs("}", dotfile);
        fclose(dotfile);
        char cmd[528];
        sprintf(cmd, "dot -Tpng %s -o %s\n", dotfile_name, pngfile_name);
        system(cmd);
    }
}


void DsLeftHeapNode_ExportGraphviz(DsLeftHeap tree,
                                   DsLeftHeapNode node,
                                   FILE* dotfile,
                                   void (*write_elem)(FILE*, void*)) {
    DsLeftHeapNode target = NULL;//void* target = NULL;
    size_t distance = 0;

    DsLeftHeapNode x;
    if ((x = node->left) != NULL) {
        DsLeftHeapNode left_max = x;
        size_t left_distance = 1;
        DsLeftHeapNode _x;
        while ((_x = left_max->right)) {
            left_max = _x;
            left_distance += 1;
        }
        target = left_max;
        distance = left_distance;

        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            fprintf(dotfile, "%lu", x);//write_elem(dotfile, x->elem);
            fprintf(dotfile, " [group=");
            fprintf(dotfile, "%lu", x);//write_elem(dotfile, x->elem);
            fputs("]\n", dotfile);
        }
        fprintf(dotfile, "    %lu [label=\"", node);
        write_elem(dotfile, node->elem);
        fprintf(dotfile, "\"]\n");
        fprintf(dotfile, "    %lu [label=\"", x);
        write_elem(dotfile, x->elem);
        fprintf(dotfile, "\"]\n");

        fprintf(dotfile, "    ");
        fprintf(dotfile, "%lu", node);//write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> ");
        fprintf(dotfile, "%lu", x);//write_elem(dotfile, x->elem);
        fputc('\n', dotfile);
        DsLeftHeapNode_ExportGraphviz(tree, x, dotfile, write_elem);
    }
    if (node->left || node->right) {
        fprintf(dotfile, "    _");
        fprintf(dotfile, "%lu", node);//write_elem(dotfile, node->elem);
        fputs(" [group=", dotfile);
        fprintf(dotfile, "%lu", node);//write_elem(dotfile, node->elem);
        fputs(", label=\"\", width=0, style=invis]\n    ", dotfile);
        fprintf(dotfile, "%lu", node);//write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> _");
        fprintf(dotfile, "%lu", node);//write_elem(dotfile, node->elem);
        fprintf(dotfile, " [style=invis]\n");
    }

    if ((x = node->right) != NULL) {
        DsLeftHeapNode right_min = x;
        size_t right_distance = 1;
        DsLeftHeapNode _x;
        while ((_x = right_min->left)) {
            right_min = _x;
            right_distance += 1;
        }
        if (right_distance <= distance) {
            target = right_min;
            distance = right_distance;
        }

        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            fprintf(dotfile, "%lu", x);//write_elem(dotfile, x->elem);
            fprintf(dotfile, " [group=");
            fprintf(dotfile, "%lu", x);//write_elem(dotfile, x->elem);
            fputs("]\n", dotfile);
        }
        fprintf(dotfile, "    %lu [label=\"", node);
        write_elem(dotfile, node->elem);
        fprintf(dotfile, "\"]\n");
        fprintf(dotfile, "    %lu [label=\"", x);
        write_elem(dotfile, x->elem);
        fprintf(dotfile, "\"]\n");

        fprintf(dotfile, "    ");
        fprintf(dotfile, "%lu", node);//write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> ");
        fprintf(dotfile, "%lu", x);//write_elem(dotfile, x->elem);
        fputc('\n', dotfile);
        DsLeftHeapNode_ExportGraphviz(tree, x, dotfile, write_elem);
    }
    if (distance > 1) {
        if (target) {
            fprintf(dotfile, "    {rank=same; _");
            fprintf(dotfile, "%lu", node);//write_elem(dotfile, node->elem);
            fprintf(dotfile, "; ");
            fprintf(dotfile, "%lu", target);//write_elem(dotfile, target);
            fputs("}\n", dotfile);
        }
    }
}


