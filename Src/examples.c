
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "examples.h"


#pragma warning(disable:4996)


void write_as_int(FILE* f, void* value) {
    fprintf(f, "%d", *(int*)value);
}

void print_as_int(void* value) {
    printf("%d", *(int*)value);
}

#define use_as_int(value) (*(int*)value)

#define use_as(type, value) (*(type*)value)

char* Int_ToString(const int* const value) {
    char* str = malloc(11);
    check_null(str);
    itoa(*value, str, 10);
    return str;
}




void DsExample_ArrayOperations(void) {

    /* Basic array operations */

    DsArray arr01 = DsArray_New(int, 20);

    DisplayObjectEx("New array:");
    DsArray_Print(arr01, print_as_int);

    DisplayObjectEx("Append some values:");
    for (int e = 0; e < 10; e++)
        DsArray_PushBack(arr01, e);

    DsArray_Print(arr01, print_as_int);

    DisplayObjectEx("Append an array:");
    DsArray_ConcatBack(arr01, (int[]) { 1, 2, 3, 4, 5 }, 5);
    DsArray_Print(arr01, print_as_int);

    DisplayObjectEx("Insert some values:");
    int ind01[] = { 5, 10, 15 };
    int val01[] = { 20, 21, 22 };
    for (size_t i = 0; i < 3; i++)
        DsArray_InsertAt(arr01, val01[i], ind01[i]);
    DsArray_Print(arr01, print_as_int);

    DisplayObjectEx("Remove some values:");
    int ind02[] = { 3, 5, 7, 11 };
    for (size_t i = 0; i < 4; i++)
        DsArray_RemoveAt(arr01, ind02[i]);
    for (size_t i = 0; i < 3; i++)
        DsArray_RemoveValue(arr01, ind02[i]);
    DsArray_Print(arr01, print_as_int);

    DsArray_Delete(arr01);

    /* Array can be atomically reallocated */

    DsArray arr02 = DsArray_New(int, 5);
    DsArray_EnsureSize(arr02, 20);
    DisplayObjectEx("Empty array:");
    DsArray_Print(arr02, print_as_int);

    DisplayObjectEx("Append values more than its capacity:");
    for (int e = 0; e < 30; e++)
        DsArray_PushBack(arr02, e);
    DsArray_Print(arr02, print_as_int);

    DisplayObjectEx("Append values more than its capacity:");
    for (int e = 30; e < 100; e++)
        DsArray_PushBack(arr02, e);
    DsArray_Print(arr02, print_as_int);

    DsArray_Delete(arr02);

}


void DsExample_ForwardListOperations(void) {

    /* Basic forward list operations */

    DisplayObjectEx("Create a forward linked list from C array:");
    int val01[] = { 1, 2, 4, 6, 8, 12, 16, 20 };
    DsForwardList list01 = DsForwardList_FromCArray(val01, CArray_GetLength(val01));
    DsForwardList_Print(list01, print_as_int);

    DisplayObjectEx("Insert some values:");
    int inspos01 = 2, insval01 = 3;
    int inspos02 = 6, insval02 = 7;
    DsForwardList_InsertAfter(list01, DsForwardList_FindNode(list01, &inspos01), &insval01);
    DsForwardList_InsertAfter(list01, DsForwardList_FindNode(list01, &inspos02), &insval02);
    DsForwardList_Print(list01, print_as_int);

    DisplayObjectEx("Remove inserted values:");
    DsForwardList_RemoveValue(list01, &insval01);
    DsForwardList_RemoveValue(list01, &insval02);
    DsForwardList_Print(list01, print_as_int);

    DisplayObjectEx("Insert values in the front of list:");
    for (size_t i = 0; i < 6; i++)
        DsForwardList_PushFront(list01, &i);
    DsForwardList_Print(list01, print_as_int);

    DisplayObjectEx("Remove values in the front of list:");
    for (size_t i = 0; i < 4; i++)
        DsForwardList_PopFront(list01);
    DsForwardList_Print(list01, print_as_int);

    DisplayObjectEx("Remove values at specified positions:");
    for (size_t i = 3; i < 7; i++)
        DsForwardList_RemoveAt(list01, i);
    DsForwardList_Print(list01, print_as_int);

    DisplayObjectEx("Swap elements:");
    DsForwardList_SwapAt(list01, 2);
    DsForwardList_SwapAt(list01, 1);
    DsForwardList_Print(list01, print_as_int);

    DsForwardList_Delete(list01);


    int val_A[] = { 0, 1, 3, 4, 6, 7, 9 };
    int val_B[] = { 1, 2, 3, 6, 7, 8, 9 };
    DsForwardList list_A = DsForwardList_FromCArray(val_A, CArray_GetLength(val_A));
    DsForwardList list_B = DsForwardList_FromCArray(val_B, CArray_GetLength(val_B));

    puts("List A and B:");
    DisplayObjectEx("List A:");
    DsForwardList_Print(list_A, print_as_int);
    DisplayObjectEx("List B:");
    DsForwardList_Print(list_B, print_as_int);

    DisplayObjectEx("Their intersection   :");
    DsForwardList list_I = DsForwardList_OrderedIntersection(list_A, list_B);
    DsForwardList_Print(list_I, print_as_int);

    DisplayObjectEx("Their union          :");
    DsForwardList list_U = DsForwardList_OrderedUnion(list_A, list_B);
    DsForwardList_Print(list_U, print_as_int);

    DisplayObjectEx("Their reversed union :");
    DsForwardList list_UR = DsForwardList_Reverse(list_U);
    DsForwardList_Print(list_UR, print_as_int);

    DsForwardList_Delete(list_A);
    DsForwardList_Delete(list_B);
    DsForwardList_Delete(list_I);
    DsForwardList_Delete(list_U);
    DsForwardList_Delete(list_UR);
}




static char DsExample_GetBeforeSymbol(char c) {
    switch (c) {
    case ']':
        return '[';
    case '}':
        return '{';
    case ')':
        return '(';
    case '>':
        return '<';
    default:
        return '\0';
    }
}


#define DsCheckSymbolBalance_UseLinkedStack 0

#if DsCheckSymbolBalance_UseLinkedStack
    #define DsCheckSymbolBalance_New     DsLinkedStack_New
    #define DsCheckSymbolBalance_Push    DsLinkedStack_Push
    #define DsCheckSymbolBalance_Pop     DsLinkedStack_Pop
    #define DsCheckSymbolBalance_Top     DsLinkedStack_Top
    #define DsCheckSymbolBalance_IsEmpty DsLinkedStack_IsEmpty
    #define DsCheckSymbolBalance_Delete  DsLinkedStack_Delete
#else
    #define DsCheckSymbolBalance_New     DsStack_New
    #define DsCheckSymbolBalance_Push    DsStack_Push
    #define DsCheckSymbolBalance_Pop     DsStack_Pop
    #define DsCheckSymbolBalance_Top     DsStack_Top
    #define DsCheckSymbolBalance_IsEmpty DsStack_IsEmpty
    #define DsCheckSymbolBalance_Delete  DsStack_Delete
#endif // DsCheckSymbolBalance_UseLinkedStack



void DsExample_CheckSymbolBalance(char* code_segment) {
    DsStack buf_balance = DsCheckSymbolBalance_New(char, 64);
    for (size_t i = 0; i < strlen(code_segment); i++) {
        switch (code_segment[i]) {
        case '[':
        case '(':
        case '{':
        case '<':
            DsCheckSymbolBalance_Push(buf_balance, &code_segment[i]);
            break;
        case ']':
        case ')':
        case '}':
        case '>':
            if (*(char*)DsCheckSymbolBalance_Top(buf_balance) == DsExample_GetBeforeSymbol(code_segment[i]))
                DsCheckSymbolBalance_Pop(buf_balance);
            else {
                printf("Syntax error near %zu\n", i);
                goto cleanup;
            }
            break;
        }
    }
    if (!DsCheckSymbolBalance_IsEmpty(buf_balance)) {
        puts("Syntax error, not closed");
        goto cleanup;
    }
    puts("Checked");
cleanup:
    DsCheckSymbolBalance_Delete(buf_balance);
    return;
}


void DsExample_QueueOperations(void) {

    DisplayObjectEx("Original queue:");
    int val[] = { 2, 6, 4, 2, 5, 3, 7, 2, 8, 6, 5, 3 };
    DsQueue q = DsQueue_FromCArray(val, CArray_GetLength(val), 18);
    DsQueue_Print(q, print_as_int);

    DisplayObjectEx("Dequeue 4 elements:");
    for (int i = 0; i < 4; i++)
        DsQueue_Dequeue(q);
    DsQueue_Print(q, print_as_int);

    DisplayObjectEx("Enqueue 4 elements:");
    for (int i = 0; i < 4; i++) {
        int enqval = i * i + 1;
        DsQueue_Enqueue(q, &enqval);
    }
    DsQueue_Print(q, print_as_int);

    DisplayObjectEx("Dequeue 3 elements:");
    for (int i = 0; i < 3; i++)
        DsQueue_Dequeue(q);
    DsQueue_Print(q, print_as_int);

    DisplayObjectEx("Enqueue 3 elements:");
    for (int i = 0; i < 3; i++) {
        int enqval = (i + 2 ) * 3;
        DsQueue_Enqueue(q, &enqval);
    }
    DsQueue_Print(q, print_as_int);

    DsQueue_Delete(q);
}


void DsExample_BinSearchTreeOperations(void) {
    
    puts("Create a binary search tree:");
    int val[] = { 13, 2, 6, 4, 5, 3, 7, 8, 0, 27, 19, 26, 25, 22 };
    DsBinSearchTree tree = DsBinSearchTree_FromCArray(val, CArray_GetLength(val));
    DsBinSearchTree_Print(tree, print_as_int);

    printf("The maximum value of tree is: ");
    print_as_int(DsBinSearchTree_FindMaxValue(tree));
    putchar('\n');

    puts("Remove some elements:");
    int remval[] = { 12, 2, 6, 8, 19 };
    for (size_t i = 0; i < CArray_GetLength(remval); i++)
        DsBinSearchTree_Remove(tree, &remval[i]);
    DsBinSearchTree_Print(tree, print_as_int);

    puts("Insert some elements:");
    int insval[] = { 17, 1, 9, 12 };
    for (size_t i = 0; i < CArray_GetLength(insval); i++)
        DsBinSearchTree_Insert(tree, &insval[i]);
    DsBinSearchTree_Print(tree, print_as_int);

    DsBinSearchTree_Delete(tree);
}


void DsExample_BinSearchTreeDisadvantage(void) {

    srand(2);
    puts("Here is a binary search tree:");
    DsBinSearchTree tree = DsBinSearchTree_New(int);
    for (size_t i = 0; i < 20; i++) {
        int val = rand() % 50;
        DsBinSearchTree_Insert(tree, &val);
    }
    DsBinSearchTree_Print(tree, print_as_int);
#if USE_GRAPHVIZ
    DsBinSearchTree_ExportGraphviz(tree, "bintree1.dot", "bintree1.png", write_as_int);
#endif
    puts("After 500 times randomly insert/remove:");
    for (size_t i = 0; i < 500; i++) {
        int insval = rand() % 50;
        DsBinSearchTree_Insert(tree, &insval);
        int delval = rand() % 50;
        DsBinSearchTree_Remove(tree, &delval);
    }
    DsBinSearchTree_Print(tree, print_as_int);
#if USE_GRAPHVIZ
    DsBinSearchTree_ExportGraphviz(tree, "bintree2.dot", "bintree2.png", write_as_int);
#endif
    puts("After 5000 times randomly insert/remove:");
    for (size_t i = 0; i < 5000; i++) {
        int insval = rand() % 50;
        DsBinSearchTree_Insert(tree, &insval);
        int delval = rand() % 50;
        DsBinSearchTree_Remove(tree, &delval);
    }
    DsBinSearchTree_Print(tree, print_as_int);
#if USE_GRAPHVIZ
    DsBinSearchTree_ExportGraphviz(tree, "bintree3.dot", "bintree3.png", write_as_int);
#endif
    puts("The tree leans sharply to the left.");

    DsBinSearchTree_Delete(tree);
}


void DsExample_AvlTreeOperations(void) {

    puts("Create a AVL tree:");
    DsAvlTree tree = DsAvlTree_New(int);
    int vals[] = { 13, 2, 6, 4, 5, 3, 7, 8, 0, 27, 19, 26, 25, 22 };
    for (size_t i = 0; i < CArray_GetLength(vals); i++) {
        DsAvlTree_Insert(tree, &vals[i]);
    }
    DsAvlTree_Print(tree, print_as_int);

    printf("The maximum value of tree is: ");
    print_as_int(DsAvlTree_FindMaxValue(tree));
    putchar('\n');

    puts("Remove some elements:");
    int remval[] = { 12, 2, 6, 8, 19 };
    for (size_t i = 0; i < CArray_GetLength(remval); i++)
        DsAvlTree_Remove(tree, &remval[i]);
    DsAvlTree_Print(tree, print_as_int);

    puts("Insert some elements:");
    int insval[] = { 17, 1, 9, 12 };
    for (size_t i = 0; i < CArray_GetLength(insval); i++) {
        DsAvlTree_Insert(tree, &insval[i]);
    }
    DsAvlTree_Print(tree, print_as_int);

    DsAvlTree_Delete(tree);
}


void DsExample_AvlTreeAdvantage(void) {

    srand(2);
    puts("Here is a AVL tree:");
    DsAvlTree tree = DsAvlTree_New(int);
    for (size_t i = 0; i < 20; i++) {
        int val = rand() % 50;
        DsAvlTree_Insert(tree, &val);
    }
    DsAvlTree_Print(tree, print_as_int);
#if USE_GRAPHVIZ
    DsAvlTree_ExportGraphviz(tree, "avltree1.dot", "avltree1.png", write_as_int);
#endif
    puts("After 500 times randomly insert/remove:");
    for (size_t i = 0; i < 500; i++) {
        int insval = rand() % 50;
        DsAvlTree_Insert(tree, &insval);
        int delval = rand() % 50;
        DsAvlTree_Remove(tree, &delval);
    }
    DsAvlTree_Print(tree, print_as_int);
#if USE_GRAPHVIZ
    DsAvlTree_ExportGraphviz(tree, "avltree2.dot", "avltree2.png", write_as_int);
#endif
    puts("After 5000 times randomly insert/remove:");
    for (size_t i = 0; i < 5000; i++) {
        int insval = rand() % 50;
        DsAvlTree_Insert(tree, &insval);
        int delval = rand() % 50;
        DsAvlTree_Remove(tree, &delval);
    }
    DsAvlTree_Print(tree, print_as_int);
#if USE_GRAPHVIZ
    DsAvlTree_ExportGraphviz(tree, "avltree3.dot", "avltree3.png", write_as_int);
#endif
    puts("The tree becomes even more balanced.");

    DsAvlTree_Delete(tree);
}


void DsExample_BinHeapOperations(void) {

    srand(4);

    DsBinHeap heap = DsBinHeap_New(int, 128);
    puts("Here is a binary heap:");
    for (int i = 1; i < 20; ++i) {
        int insval = rand() % 100;
        DsBinHeap_Insert(heap, &insval);
    }
    DsBinHeap_PrintAsTree(heap, (char *(*)(void *)) Int_ToString);

    int insval = 6;
    printf("Insert value: %d\n", insval);
    DsBinHeap_Insert(heap, &insval);

    DsBinHeap_PrintAsTree(heap, (char *(*)(void *)) Int_ToString);

    printf("Remove minimum: %d\n", *(int*)DsBinHeap_FindMinValue(heap));
    DsBinHeap_RemoveMin(heap);
    DsBinHeap_PrintAsTree(heap, (char *(*)(void *)) Int_ToString);

    printf("Remove minimum: %d\n", *(int*)DsBinHeap_FindMinValue(heap));
    DsBinHeap_RemoveMin(heap);
    DsBinHeap_PrintAsTree(heap, (char *(*)(void *)) Int_ToString);

    printf("Remove minimum: %d\n", *(int*)DsBinHeap_FindMinValue(heap));
    DsBinHeap_RemoveMin(heap);
    DsBinHeap_PrintAsTree(heap, (char *(*)(void *)) Int_ToString);
    
    DsBinHeap_Delete(heap);
}



void DsExample_LeftHeapMerge(void) {

    srand(6);

    puts("Leftist heap A:      (see generated image)");
    DsLeftHeap heap_A = DsLeftHeap_New(int);
    for (size_t i = 0; i < 20; i++) {
        int val = rand() % 50 + 2;
        DsLeftHeap_Insert(heap_A, &val);
    }

#if USE_GRAPHVIZ
    DsLeftHeap_ExportGraphviz(heap_A, "leftheapA.dot", "leftheapA.png", write_as_int);
#endif

    puts("Leftist heap B:      (see generated image)");
    DsLeftHeap heap_B = DsLeftHeap_New(int);
    for (size_t i = 0; i < 20; i++) {
        int val = rand() % 50 + 5;
        DsLeftHeap_Insert(heap_B, &val);
    }

#if USE_GRAPHVIZ
    DsLeftHeap_ExportGraphviz(heap_B, "leftheapB.dot", "leftheapB.png", write_as_int);
#endif

    puts("Merge heap A and B:  (see generated image)");
    DsLeftHeap heap_M = DsLeftHeap_Merge(heap_A, heap_B);

#if USE_GRAPHVIZ
    DsLeftHeap_ExportGraphviz(heap_M, "leftheapM.dot", "leftheapM.png", write_as_int);
#endif

    DsLeftHeap_Delete(heap_A);
    DsLeftHeap_Delete(heap_B);
    DsLeftHeap_Delete(heap_M);
}


void DsExample_HashTableOperations(void) {

    DsHashSepTable hash_table = DsHashSepTable_New(50003, DsHashFunc_djb2);

    char key1[] = "Hello, world";
    DsHashSepTable_Insert(hash_table, key1, sizeof(key1));
    char key2[] = "Hello, World";
    DsHashSepTable_Insert(hash_table, key2, sizeof(key2));
    char key3[] = "hello, world";
    DsHashSepTable_Insert(hash_table, key3, sizeof(key3));
    char key4[] = "hello, World";
    DsHashSepTable_Insert(hash_table, key4, sizeof(key4));

    DsHashSepTable_Remove(hash_table, key2, sizeof(key2));

    printf("\"%s\" %s the hash table\n", key2,
        DsHashSepTable_IsKeyIn(hash_table, key2, sizeof(key2))? "is in"
                                                              : "is not in");
    printf("\"%s\" %s the hash table\n", key4,
        DsHashSepTable_IsKeyIn(hash_table, key4, sizeof(key4))? "is in"
                                                              : "is not in");
    DsHashSepTable_Delete(hash_table);

}


void DsExample_HashMapOperations(void) {

    DsHashSepMap config = DsHashSepMap_New(50003, (DsHashFuncType) DsHashFunc_djb2);

    bool debug_flag = false;
    DsHashSepMap_FastInsert(config, "debug", debug_flag);
    char file[] = "Src/examples.c";
    DsHashSepMap_FastInsert(config, "file", file);
    int storage[] = { 43, 52, 37, 69, 81, 47 };
    DsHashSepMap_FastInsert(config, "storage", storage);
    
    debug_flag = true;
    DsHashSepMap_FastUpdate(config, "debug", debug_flag);

    if (use_as_int(DsHashSepMap_FastFindValue(config, "debug")))
        /* NOTE: in C1X, macro true / false are defined as int */
        printf("Nothing in <%s>, line %d", (char*)DsHashSepMap_FastFindValue(config, "file"), __LINE__);

    DsHashSepMap_Delete(config);

}




//void test02(void) {
//
//    for range(i, 10)
//        printf("%d ", i);
//    puts("");
//    for range(c, 55, 65) {
//        putchar(c);
//        putchar(' ');
//    }
//    puts("");
//    for range(n, 1, 24, 3)
//        printf("%d ", n);
//
//}