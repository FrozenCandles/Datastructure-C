
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"


/*==========*\
| Hash Table |
\*==========*/

DsHashSepTable DsHashSepTable_New(size_t size, DsHashFuncType hashfunc) {

    DsHashSepTable table = malloc(sizeof(struct DsHashSepTable));
    check_null(table);
    table->lists = malloc(sizeof(DsHashSepTableNode) * size);
    check_null(table->lists);
    table->size = size;
    table->hashfunc = hashfunc;
    memset(table->lists, 0, sizeof(DsHashSepTableNode) * size);

    return table;
}


static inline int DsHashSepTable_Compare(DsHashSepTableNode node, void* key, size_t key_len) {
    return memcmp(node->elem, key, key_len);
}

static void DsHashSepTable_Assign(DsHashSepTableNode node, void* key, size_t key_len) {
    memcpy(node->elem, key, key_len);
    node->elem_len = key_len;
}

#define DsHashSepTable_GetHashedList(table, key, key_len)\
    (table)->lists[table->hashfunc((key), (key_len), (table)->size)]



bool DsHashSepTable_IsKeyIn(DsHashSepTable table, void* key, size_t key_len) {
    DsHashSepTableNode target = DsHashSepTable_GetHashedList(table, key, key_len);

    while (target && target->elem) {
        if (DsHashSepTable_Compare(target, key, key_len) == 0)
            return true;
        target = target->next;
    }
    return false;
}

/* reference: https://github.com/TheAlgorithms/C/blob/master/hash
 * @param key
 * @param len
 * @param size size of hash table
 * @return
 */
size_t DsHashFunc_djb2(const char* key, size_t length, size_t size) {
    size_t hash = 5381; /* init value */
    for (size_t i = 0; i < length; i++)
        hash = ((hash << 5) + hash) + key[i];
    return hash % size;
}


void DsHashSepTable_Insert(DsHashSepTable table, void* key, size_t key_len) {
    DsHashSepTableNode target = DsHashSepTable_GetHashedList(table, key, key_len);

    if (target == NULL) {   // not used
        target = malloc(sizeof(struct DsHashSepTableNode));
        check_null(target);
        target->elem = malloc(key_len);
        check_null(target->elem);
        DsHashSepTable_Assign(target, key, key_len);
        target->next = NULL;
        DsHashSepTable_GetHashedList(table, key, key_len) = target;
        return;
    }

    while (DsHashSepTable_Compare(target, key, key_len) != MemAllEqual && target->next != NULL) {
        target = target->next;
    }

    if (target->next == NULL) {
        DsHashSepTableNode new_node = malloc(sizeof(struct DsHashSepTableNode));
        check_null(new_node);
        new_node->elem = malloc(key_len);
        check_null(new_node->elem);
        DsHashSepTable_Assign(new_node, key, key_len);
        new_node->next = target->next;
        target->next = new_node;
    }
    /* key has been inserted, do nothing */
}

void DsHashSepTable_Remove(DsHashSepTable table, void* key, size_t key_len) {
    DsHashSepTableNode target = DsHashSepTable_GetHashedList(table, key, key_len);

    if (target == NULL)
        return;
    while (DsHashSepTable_Compare(target, key, key_len) != MemAllEqual && target->next != NULL)
        target = target->next;

    if (target != NULL) {
        free(target->elem);
        free(target);
        DsHashSepTable_GetHashedList(table, key, key_len) = NULL;
    }
}


void DsHashSepTable_DeleteList(DsHashSepTableNode list) {
    DsHashSepTableNode temp;
    while (list != NULL) {
        temp = list->next;
        free(list->elem);
        free(list);
        list = temp;
    } 
}


void DsHashSepTable_Delete(DsHashSepTable table) {
    for (size_t i = 0; i < table->size; i++) {
        DsHashSepTableNode this_list = table->lists[i];
        if (this_list != NULL) {
            DsHashSepTable_DeleteList(this_list);
        }
    }
    free(table->lists);
    free(table);
}


/*========*\
| Hash Map |
\*========*/


DsHashSepMap DsHashSepMap_New(size_t size, DsHashFuncType hashfunc) {

    DsHashSepMap table = malloc(sizeof(struct DsHashSepMap));
    check_null(table);
    table->lists = malloc(sizeof(DsHashSepMapNode) * size);
    check_null(table->lists);
    table->size = size;
    table->hashfunc = hashfunc;
    memset(table->lists, 0, sizeof(DsHashSepMapNode) * size);

    return table;
}



static inline int DsHashSepMap_Compare(DsHashSepMapNode node, void* key, size_t key_len) {
    return memcmp(node->key, key, key_len);
}

static void DsHashSepMap_Assign(DsHashSepMapNode node, void* key, size_t key_len,
                                                       void* value, size_t value_len) {
    memcpy(node->key, key, key_len);
    node->key_len = key_len;
    memcpy(node->value, value, value_len);
    node->value_len = value_len;
}


void DsHashSepMap_Insert(DsHashSepMap table, void* key, size_t key_len, void* value, size_t value_len) {

    DsHashSepMapNode target = DsHashSepTable_GetHashedList(table, key, key_len);

    if (target == NULL) {   // not used
        target = malloc(sizeof(struct DsHashSepMapNode));
        check_null(target);
        target->key = malloc(key_len);
        check_null(target->key);
        target->value = malloc(value_len);
        check_null(target->value);
        DsHashSepMap_Assign(target, key, key_len, value, value_len);
        target->next = NULL;
        DsHashSepTable_GetHashedList(table, key, key_len) = target;
        return;
    }

    while (!DsHashSepMap_Compare(target, key, key_len) && target->next != NULL) {
        target = target->next;
    }

    if (target->next == NULL) {
        DsHashSepMapNode new_node = malloc(sizeof(struct DsHashSepMapNode));
        check_null(new_node);
        new_node->key = malloc(key_len);
        check_null(new_node->key);
        new_node->key = malloc(value_len);
        check_null(new_node->value);
        DsHashSepMap_Assign(new_node, key, key_len, value, value_len);
        new_node->next = target->next;
        target->next = new_node;
    }
    /* key has been inserted, do nothing */
}


void DsHashSepMap_Remove(DsHashSepMap table, void* key, size_t key_len) {

    DsHashSepMapNode target = DsHashSepTable_GetHashedList(table, key, key_len);

    if (target == NULL)
        return;

    while (DsHashSepMap_Compare(target, key, key_len) != MemAllEqual && target->next != NULL)
        target = target->next;

    if (target != NULL) {
        free(target->key);
        free(target->value);
        free(target);
        DsHashSepTable_GetHashedList(table, key, key_len) = NULL;
    }
}


DsHashSepMapNode DsHashSepMap_FindNode(DsHashSepMap table, void* key, size_t key_len) {

    DsHashSepMapNode target = DsHashSepTable_GetHashedList(table, key, key_len);

    while (target != NULL && DsHashSepMap_Compare(target, key, key_len) != MemAllEqual)
        target = target->next;

    if (target == NULL)          // not inserted
        return NULL;
    
    return target;
}


void* DsHashSepMap_FindValue(DsHashSepMap table, void* key, size_t key_len, size_t* value_len) {
    DsHashSepMapNode target = DsHashSepMap_FindNode(table, key, key_len);

    if (target == NULL)          // not inserted
        return NULL;
    else {
        if (value_len != NULL)
            *value_len = target->value_len;
        return target->value;
    }
}


void DsHashSepMap_Update(DsHashSepMap table, void* key, size_t key_len, void* value, size_t value_len) {
    DsHashSepMapNode target = DsHashSepTable_GetHashedList(table, key, key_len);

    if (target == NULL)          // not inserted
        return;
    else {
        target->value_len = value_len;
        free(target->value);
        target->value = malloc(value_len);  // maybe I should use realloc()
        check_null(target->value);
        memcpy(target->value, value, value_len);
    }
}


void DsHashSepMap_DeleteList(DsHashSepMapNode list) {
    DsHashSepMapNode temp;
    while (list != NULL) {
        temp = list->next;
        free(list->key);
        free(list->value);
        free(list);
        list = temp;
    }
}


void DsHashSepMap_Delete(DsHashSepMap table) {
    for (size_t i = 0; i < table->size; i++) {
        DsHashSepMapNode this_list = table->lists[i];
        if (this_list != NULL) {
            DsHashSepMap_DeleteList(this_list);
        }
    }
    free(table->lists);
    free(table);
}