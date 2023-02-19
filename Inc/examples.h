#pragma once

#include "array.h"
#include "linkedlist.h"
#include "stack.h"
#include "queue.h"
#include "bintree.h"
#include "heap.h"
#include "hash.h"


extern void print_as_int(void* value);

#define DisplayObjectEx(msg) printf(msg"\n    ")



void DsExample_ArrayOperations(void);

void DsExample_ForwardListOperations(void);

void DsExample_CheckSymbolBalance(char* code_segment);

void DsExample_QueueOperations(void);

void DsExample_BinSearchTreeOperations(void);

void DsExample_BinSearchTreeDisadvantage(void);

void DsExample_AvlTreeOperations(void);

void DsExample_AvlTreeAdvantage(void);

void DsExample_BinHeapOperations(void);

void DsExample_LeftHeapMerge(void);

void DsExample_HashTableOperations(void);

void DsExample_HashMapOperations(void);
