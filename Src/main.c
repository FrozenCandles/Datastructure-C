#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <crtdbg.h>

#include "examples.h"



int main(void) {

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);

    //DsExample_ArrayOperations();
    
    //DsExample_ForwardListOperations();

    //DsExample_CheckSymbolBalance("int [func({a: i, b: 2}), vector<T{c: 3}(*args)>]{1, 2, 3}");

    //DsExample_QueueOperations();

    //DsExample_BinSearchTreeOperations();

    //DsExample_BinSearchTreeDisadvantage();

    //DsExample_AvlTreeOperations();

    //DsExample_AvlTreeAdvantage();

    //DsExample_BinHeapOperations();

    //DsExample_LeftHeapMerge();

    //DsExample_HashTableOperations();

    //DsExample_HashMapOperations();

    _ASSERT(_CrtCheckMemory());
    _CrtDumpMemoryLeaks();

    return 0;
}

