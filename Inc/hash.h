
#pragma once

#include "pubdef.h"

#include "linkedlist.h"


/*==========*\
| Hash Table |
\*==========*/

/*  +==========+
    |  size    |
    | hashfunc |       +===============+        +==========+        +==========+
    |  lists  -+-------> DsHashSepNode +-------->   next  -+-------->         -+------> NULL
    +==========+       | DsHashSepNode |        | elem_len |        |   ....   |
   DsHashSepTable      |      ...      |        |   elem  -+-----+  |          |
                       | DsHashSepNode |        +==========+     |  +==========+
                       +===============+    struct DsHashSepNode |
                        DsHashSepNode[]                          |   +----------------+
                                                                 +-> | "Hello, world" |
                                                                     +----------------+        */

typedef struct DsHashSepTableNode {
    struct DsHashSepTableNode* next;
    void* elem;
    size_t elem_len;
} * DsHashSepTableNode;

typedef size_t(*DsHashFuncType)(void* /*key*/, size_t /*key_len*/, size_t /*size*/);

typedef struct DsHashSepTable {
    size_t size;
    DsHashSepTableNode* lists;
    DsHashFuncType hashfunc;
} * DsHashSepTable;


DsHashSepTable DsHashSepTable_New(size_t size, DsHashFuncType hashfunc);

void DsHashSepTable_Delete(DsHashSepTable table);

bool DsHashSepTable_IsKeyIn(DsHashSepTable table, void* key, size_t key_len);

#define DsHashSepTable_EnsureKeyIn DsHashSepTable_Insert

void DsHashSepTable_Insert(DsHashSepTable table, void* key, size_t key_len);

void DsHashSepTable_Remove(DsHashSepTable table, void* key, size_t key_len);

/* Hash Functions */

size_t DsHashFunc_djb2(const char* key, size_t length, size_t size);


/*========*\
| Hash Map |
\*========*/


typedef struct DsHashSepMapNode {
    struct DsHashSepMapNode* next;
    void* key;
    size_t key_len;
    void* value;
    size_t value_len;
} * DsHashSepMapNode;


typedef struct DsHashSepMap {
    size_t size;
    DsHashSepMapNode* lists;
    DsHashFuncType hashfunc;
} * DsHashSepMap;



DsHashSepMap DsHashSepMap_New(size_t size, DsHashFuncType hashfunc);

void DsHashSepMap_Delete(DsHashSepMap table);

void DsHashSepMap_Insert(DsHashSepMap table, void* key, size_t key_len, void* value, size_t value_len);
#define DsHashSepMap_FastInsert(table, key, value)\
    DsHashSepMap_Insert((table), &(key), sizeof(key), &(value), sizeof(value))

DsHashSepMapNode DsHashSepMap_FindNode(DsHashSepMap table, void* key, size_t key_len);

void* DsHashSepMap_FindValue(DsHashSepMap table, void* key, size_t key_len, size_t* value_len);
#define DsHashSepMap_FastFindValue(table, key)\
    DsHashSepMap_FindValue((table), &(key), sizeof(key), NULL)

void DsHashSepMap_Update(DsHashSepMap table, void* key, size_t key_len, void* value, size_t value_len);
#define DsHashSepMap_FastUpdate(table, key, value)\
    DsHashSepMap_Update((table), &(key), sizeof(key), &(value), sizeof(value))

