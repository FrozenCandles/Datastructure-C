

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist.h"



static inline void DsForwardListNode_Assign(DsForwardList list, 
                                            DsForwardListNode node, 
                                            void* value) {
    memcpy(node->elem, value, list->elem_size);
}


static inline int DsForwardListNode_Compare(DsForwardList list,
                                            DsForwardListNode node,
                                            void* value) {
    return memcmp(node->elem, value, list->elem_size);
}


/**
 * Construct a forward list
 * @param elem_size
 */
DsForwardList DsForwardList_New_Inner(size_t elem_size) {
    DsForwardListHeader list = (DsForwardListHeader)malloc(sizeof(struct DsForwardListHeader));
    check_null(list);
    list->elem_size = elem_size;
    list->length = 0;
    list->next = NULL;
    return list;
}

/**
 * Return true if the linked list is empty
 * @param header
 * @return
 */
bool DsForwardList_IsEmpty(DsForwardList list) {
    return list->next == NULL;
}

/**
 * Return true if current node is the last position
 * @param node
 * @return
 */
bool DsForwardList_IsLast(DsForwardListNode node) {
    return node->next == NULL;
}

/**
 * Return a node in certain position, NULL if not found
 * @param value
 * @param node
 * @return
 */
DsForwardListNode DsForwardList_FindNode(DsForwardList list, void* value) {
    DsForwardListNode next = list->next;
    while (next != NULL && !DsForwardListNode_Compare(list, next, value))
        next = next->next;
    return next;
}

/**
 * If previous node is not found, last returned
 * @param value
 * @param list
 * @return
 */
DsForwardListNode DsForwardList_FindPrevNode(DsForwardList list, void* value) {
    DsForwardListNode prev = list->next;
    while (prev->next != NULL && !DsForwardListNode_Compare(list, prev->next, value))
        prev = prev->next;
    return prev;
}

/**
 * Delete first occurence of element in linked list
 * @param value
 * @param list
 */
void DsForwardList_RemoveValue(DsForwardList list, void* value) {
    DsForwardListNode prev = DsForwardList_FindPrevNode(list, value);
    if (!DsForwardList_IsLast(prev)) {
        DsForwardListNode temp = prev->next;
        prev->next = temp->next;  // bypass deleted cell
        free(temp->elem);
        free(temp);
        list->length--;
    }
}


/**
 * @brief delete element at (index)
 *
 * @param list
 * @param index
 * @param ret
 * @return
 */
void DsForwardList_RemoveAt(DsForwardList list, size_t index) {
    if (index == 0) {
        DsForwardList_PopFront(list);
        return;
    }
    DsForwardListNode node = list->next;
    size_t i = 0;
    while (node->next && i < index - 1) {  // find element at (index)
        node = node->next;
        i++;
    }
    if (!(node->next) || i > index - 1) {
        /* incorrect deleting position */
        return;
    }
    DsForwardListNode temp = node->next;
    node->next = temp->next;  // bypass deleted cell
    free(temp->elem);
    free(temp);
    list->length--;
}

/**
 * Insert after previous node
 * @param list
 * @param value
 * @param prev
 */
DsForwardList DsForwardList_InsertAfter(DsForwardList list,
                                     DsForwardListNode prev, 
                                     void* value) {
    DsForwardListNode temp = (DsForwardListNode)malloc(sizeof(struct DsForwardListNode));
    check_null(temp);
    temp->elem = malloc(list->elem_size);
    check_null(temp->elem);

    DsForwardListNode_Assign(list, temp, value);
    temp->next = prev->next;
    prev->next = temp;
    list->length++;
    return list;
}



DsForwardList DsForwardList_PushFront(DsForwardList list, void* value) {

    DsForwardListNode temp = (DsForwardListNode)malloc(sizeof(struct DsForwardListNode));
    check_null(temp);
    temp->elem = malloc(list->elem_size);
    check_null(temp->elem);

    DsForwardListNode_Assign(list, temp, value);
    temp->next = list->next;
    list->next = temp;
    list->length++;

    return list;
}


DsForwardList DsForwardList_PopFront(DsForwardList list) {
    if (!DsForwardList_IsEmpty(list)) {
        DsForwardListNode front = list->next;
        list->next = list->next->next;
        free(front->elem);
        free(front);
        list->length--;
    }
    return list;
}

/**
 * Exchange element between index and its next by adjusting pointers.
 * Position starts with 0
 * @param list
 * @param pos
 */
void DsForwardList_SwapAt(DsForwardList list, size_t index) {
    if (index == 0) {
        DsForwardListHeader prev = list;
        DsForwardListNode this = prev->next;
        DsForwardListNode next = this->next;
        DsForwardListNode nnxt = next->next;

        prev->next = next;
        this->next = nnxt;
        next->next = this;
    }
    else {
        DsForwardListNode node = list->next;
        for (size_t i = 0; i < index - 1; i++) {
            if (node->next)
                node = node->next;
            else
                return;
        }
        DsForwardListNode prev = node;
        DsForwardListNode this = prev->next;
        DsForwardListNode next = this->next;
        DsForwardListNode nnxt = next->next;

        prev->next = next;
        this->next = nnxt;
        next->next = this;
    }
}



void DsForwardList_Print(DsForwardList list, void (*print_elem)(void* elem)) {

    DsForwardList_PrintStart;
    if (list->length < 20) {
        for (DsForwardListNode node = list->next; node; node = node->next) {
            if (node != list->next)
                DsForwardList_PrintSep;
            print_elem(node->elem);
        }
        DsForwardList_PrintEnd;
    }
    else {
        DsForwardListNode node = list->next;
        for (size_t i = 0; i < 20; i++) {
            if (node) {
                if (node != list->next)
                    DsForwardList_PrintSep;
                print_elem(node->elem);
                node = node->next;
            }
        }
        DsForwardList_PrintSep;
        DsForwardList_PrintEllipsis;
        DsForwardList_PrintEnd;
    }
}


/**
 * Clear linked list and each element
 * @param list
 */
void DsForwardList_Clear(DsForwardList list) {
    DsForwardListNode pres, temp;
    pres = list->next;
    list->next = NULL;
    while (pres != NULL) {
        temp = pres->next;
        free(pres->elem);
        free(pres);
        pres = temp;
    }
}


void DsForwardList_Delete(DsForwardList list) {
    if (!DsForwardList_IsEmpty(list))
        DsForwardList_Clear(list);
    free(list);
}



DsForwardList DsForwardList_FromCArray_Inner(const void* values, size_t length, size_t elem_size) {
    DsForwardList header = DsForwardList_New_Inner(elem_size);  // create header node
    
    for (int i = length - 1; i >= 0; i--) {    // create some nodes
        //printf("%d; ", *(int*)((uint8_t*)values + i * elem_size));
        DsForwardList_PushFront(header, (uint8_t*)values + i * elem_size);
    }
    return header;
}




DsForwardList DsForwardList_Reverse(DsForwardList list) {
    DsForwardList header = DsForwardList_New_Inner(list->elem_size);

    if (DsForwardList_IsEmpty(list))
        return header;

    DsForwardListNode node = list->next;
    while (node) {
        DsForwardList_PushFront(header, node->elem);
        node = node->next;
    }
    return header;
}




/**
 * Set intersection between two ordered list
 * @param a need to be orderly
 * @param b need to be orderly
 * @return
 */
DsForwardList DsForwardList_OrderedIntersection(DsForwardList a, DsForwardList b) {
    DsForwardList result = DsForwardList_New_Inner(a->elem_size);
    check_null(result);

    DsForwardListNode a_node = a->next;
    DsForwardListNode b_node = b->next;
    while (a_node != NULL && b_node != NULL) {
        if (DsForwardListNode_Compare(a, a_node, b_node->elem) < 0)
            a_node = a_node->next;
        else if (DsForwardListNode_Compare(a, a_node, b_node->elem) > 0)
            b_node = b_node->next;
        else {
            DsForwardList_PushFront(result, a_node->elem);
            a_node = a_node->next;
            b_node = b_node->next;
        }
    }
    return result;
}

DsForwardList DsForwardList_OrderedUnion(DsForwardList a, DsForwardList b) {
    DsForwardList result = DsForwardList_New_Inner(a->elem_size);
    check_null(result);

    DsForwardListNode a_node = a->next;
    DsForwardListNode b_node = b->next;

    while (a_node != NULL && b_node != NULL) {
        if (DsForwardListNode_Compare(a, a_node, b_node->elem) < 0) {
            DsForwardList_PushFront(result, a_node->elem);
            a_node = a_node->next;
        }
        else if (DsForwardListNode_Compare(a, a_node, b_node->elem) > 0) {
            DsForwardList_PushFront(result, a_node->elem);
            b_node = b_node->next;
        }
        else {
            DsForwardList_PushFront(result, a_node->elem);
            a_node = a_node->next;
            b_node = b_node->next;
        }
    }
    return result;
}

