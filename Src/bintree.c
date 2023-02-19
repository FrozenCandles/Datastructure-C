
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bintree.h"

#pragma warning(disable:4996)


#define max(a, b) ((a) > (b) ? (a) : (b))


static inline void DsBinSearchTree_Assign(DsBinSearchTree tree,
                                          DsBinSearchTreeNode node,
                                          void* value) {
    memcpy(node->elem, value, tree->elem_size);
}


static inline int DsBinSearchTree_Compare(DsBinSearchTree tree,
                                         DsBinSearchTreeNode node,
                                         void* value) {
    return memcmp(node->elem, value, tree->elem_size);
}


DsBinSearchTree DsBinSearchTree_New_Inner(size_t elem_size) {
    DsBinSearchTree tree = malloc(sizeof(struct DsBinSearchTreeHeader));
    check_null(tree);
    tree->body = NULL;
    tree->elem_size = elem_size;
    return tree;
}

DsBinSearchTree DsBinSearchTree_FromCArray_Inner(const void* values, 
                                                 size_t length, 
                                                 size_t elem_size) {
    DsBinSearchTree tree = DsBinSearchTree_New_Inner(elem_size);
    for (size_t i = 0; i < length; i++) {
        DsBinSearchTree_Insert(tree, (uint8_t*)values + i * elem_size);
    }
    return tree;
}


DsBinSearchTree DsBinSearchTree_Clear(DsBinSearchTree tree) {
    tree->body = DsBinSearchTreeNode_Clear(tree->body);
}

DsBinSearchTreeNode DsBinSearchTreeNode_Clear(DsBinSearchTreeNode tree) {
    if (tree != NULL) {
        DsBinSearchTreeNode_Clear(tree->left);
        DsBinSearchTreeNode_Clear(tree->right);
        free(tree->elem);
        free(tree);
    }
    return NULL;
}


void DsBinSearchTree_Delete(DsBinSearchTree tree) {
    DsBinSearchTree_Clear(tree);
    free(tree);
}


/**
 * Find a node from tree
 * @param tree
 * @param value
 * @return NULL if not found
 */
DsBinSearchTreeNode DsBinSearchTree_FindNode(DsBinSearchTree tree, void* value) {
    return DsBinSearchTreeNode_FindNode(tree, tree->body, value);
}

DsBinSearchTreeNode DsBinSearchTreeNode_FindNode(DsBinSearchTree tree, 
                                                 DsBinSearchTreeNode node, 
                                                 void* value) {
    if (node == NULL)
        return NULL;
    if (DsBinSearchTree_Compare(tree, node, value) > 0)
        return DsBinSearchTreeNode_FindNode(tree, node->left, value);
    else if (DsBinSearchTree_Compare(tree, node, value) < 0)
        return DsBinSearchTreeNode_FindNode(tree, node->right, value);
    else
        return node;
}


DsBinSearchTreeNode DsBinSearchTree_FindMinNode(DsBinSearchTree tree) {
    return DsBinSearchTreeNode_FindMinNode(tree, tree->body);
}

DsBinSearchTreeNode DsBinSearchTreeNode_FindMinNode(DsBinSearchTree tree,
                                                    DsBinSearchTreeNode node) {
    if (node == NULL)
        return NULL;
    else if (node->left == NULL)
        return node;
    else
        return DsBinSearchTreeNode_FindMinNode(tree, node->left);
}


DsBinSearchTreeNode DsBinSearchTree_FindMaxNode(DsBinSearchTree tree) {
    return DsBinSearchTreeNode_FindMaxNode(tree, tree->body);
}

DsBinSearchTreeNode DsBinSearchTreeNode_FindMaxNode(DsBinSearchTree tree,
                                                    DsBinSearchTreeNode node) {
    if (node != NULL)
        while (node->right != NULL)
            node = node->right;
    return node;
}


void* DsBinSearchTree_FindMaxValue(DsBinSearchTree tree) {
    return DsBinSearchTreeNode_FindMaxNode(tree, tree->body)->elem;
}

void* DsBinSearchTree_FindMinValue(DsBinSearchTree tree) {
    return DsBinSearchTreeNode_FindMinNode(tree, tree->body)->elem;
}


void DsBinSearchTree_Insert(DsBinSearchTree tree, void* value) {
    if (tree->body == NULL)
        tree->body = DsBinSearchTreeNode_Insert(tree, tree->body, value);
    else
        DsBinSearchTreeNode_Insert(tree, tree->body, value);
}

DsBinSearchTreeNode DsBinSearchTreeNode_Insert(DsBinSearchTree tree, 
                                               DsBinSearchTreeNode node, 
                                               void* value) {
    if (node == NULL) {  // empty tree, should create first
        node = malloc(sizeof(struct DsBinSearchTreeNode));
        check_null(node);
        node->elem = malloc(sizeof(tree->elem_size));
        check_null(node->elem);
        DsBinSearchTree_Assign(tree, node, value);
        node->left = node->right = NULL;
    }
    else if (DsBinSearchTree_Compare(tree, node, value) > 0)
        node->left = DsBinSearchTreeNode_Insert(tree, node->left, value);
    else if (DsBinSearchTree_Compare(tree, node, value) < 0)
        node->right = DsBinSearchTreeNode_Insert(tree, node->right, value);
    return node;  // value is already in the tree
}


DsBinSearchTree DsBinSearchTree_Remove(DsBinSearchTree tree, void* value) {
    tree->body = DsBinSearchTreeNode_Remove(tree, tree->body, value);
    return tree;
}

DsBinSearchTreeNode DsBinSearchTreeNode_Remove(DsBinSearchTree tree, 
                                               DsBinSearchTreeNode node, 
                                               void* value) {
    if (node == NULL)                                        // element not found
        return NULL; 

    DsBinSearchTreeNode temp;
    if (DsBinSearchTree_Compare(tree, node, value) > 0)      // go left
        node->left = DsBinSearchTreeNode_Remove(tree, node->left, value);
    else if (DsBinSearchTree_Compare(tree, node, value) < 0) // go right
        node->right = DsBinSearchTreeNode_Remove(tree, node->right, value);
    else if (node->left && node->right) {                    // element found with 2 children
        /* replace by min value in right subtree */
        temp = DsBinSearchTreeNode_FindMinNode(tree, node->right);
        DsBinSearchTree_Assign(tree, node, temp->elem);
        node->right = DsBinSearchTreeNode_Remove(tree, node->right, node->elem);
    }
    else {  // element found with 0 or 1 children
        temp = node;
        if (node->left == NULL)
            node = node->right;
        else if (node->right == NULL)
            node = node->left;
        free(temp->elem);
        free(temp);
    }
    return node;
}


static void DsBinSearchTreeNode_PrintNode(DsBinSearchTree tree, 
                                          DsBinSearchTreeNode node, 
                                          size_t depth, 
                                          void (*print_elem)(void* elem)) {
    if (node != NULL) {
        for (size_t i = 0; i < depth; i++)
            putchar('-');
        putchar(' ');
        print_elem(node->elem);
        putchar('\n');
    }
}

void DsBinSearchTreeNode_Print(DsBinSearchTree tree, 
                               DsBinSearchTreeNode node, 
                               size_t depth,
                               void (*print_elem)(void*)) {
    if (node) {
        DsBinSearchTreeNode_Print(tree, node->left, depth + 1, print_elem);
        DsBinSearchTreeNode_PrintNode(tree, node, depth, print_elem);
        DsBinSearchTreeNode_Print(tree, node->right, depth + 1, print_elem);
    }
}

void DsBinSearchTree_Print(DsBinSearchTree tree, void (*print_elem)(void*)) {
    puts("Binary Search Tree {");
    if (tree->body) {
        DsBinSearchTreeNode_Print(tree, tree->body, 0, print_elem);
    }
    puts("}");
}


/**
 * reference: https://blog.nanpuyue.com/2019/054.html 的一个照抄实现
 * @param tree Binary search tree object
 * @param dotfile_name Saved dot file name
 * @param pngfile_name Saved png file name
 * @param write_elem
 * @return
 */
void DsBinSearchTree_ExportGraphviz(DsBinSearchTree tree,
                                    const char* dotfile_name, 
                                    const char* pngfile_name, 
                                    void (*write_elem)(FILE* f, void* elem)) {
    FILE* dotfile = fopen(dotfile_name, "w");
    DsBinSearchTreeNode x;
    if (x = tree->body) {
        fputs("digraph G {", dotfile);
        fputs("    graph [nodesep=0.1]", dotfile);
        fputs("    node [shape=circle]", dotfile);
        fputs("    edge [arrowhead=vee]", dotfile);
        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            write_elem(dotfile, x->elem);
            fprintf(dotfile, "[group =");
            write_elem(dotfile, x->elem);
            fputs("]", dotfile);
        }
        DsBinSearchTreeNode_ExportGraphviz(tree, x, dotfile, write_elem);
        fputs("}", dotfile);
        fclose(dotfile);
        /* it doesn't seem to work */
        char cmd[528];
        sprintf(cmd, "dot -Tpng %s -o %s\n", dotfile_name, pngfile_name);
        system(cmd);
#if 0
        FILE* dotfile = fopen(dotfile_name, "w");
        fputs("digraph G{", dotfile);
        DsBinSearchTreeNode_ExportGraphviz(tree, tree->body, dotfile, write_elem);
        fputs("}", dotfile);
        char cmd[528];
        sprintf(cmd, "dot -Tpng %s -o %s\n", dotfile_name, pngfile_name);
        system(cmd);
#endif
    }
}



void DsBinSearchTreeNode_ExportGraphviz(DsBinSearchTree tree,
                                        DsBinSearchTreeNode node, 
                                        FILE* dotfile,
                                        void (*write_elem)(FILE* f, void* elem)) {
    void* target = NULL;
    size_t distance = 0;

    DsBinSearchTreeNode x;
    if ((x = node->left) != NULL) {
        DsBinSearchTreeNode left_max = x;
        size_t left_distance = 1;
        DsBinSearchTreeNode _x;
        while ((_x = left_max->right)) {
            left_max = _x;
            left_distance += 1;
        }
        target = left_max->elem;
        distance = left_distance;

        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            write_elem(dotfile, x->elem);
            fprintf(dotfile, " [group=");
            write_elem(dotfile, x->elem);
            fputs("]", dotfile);
        }
        fprintf(dotfile, "    ");
        write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> ");
        write_elem(dotfile, x->elem);
        fputc('\n', dotfile);
        DsBinSearchTreeNode_ExportGraphviz(tree, x, dotfile, write_elem);
    }
    if (node->left || node->right) {
        fprintf(dotfile, "    _");
        write_elem(dotfile, node->elem);
        fputs(" [group=", dotfile);
        write_elem(dotfile, node->elem);
        fputs(", label=\"\", width=0, style=invis]\n    ", dotfile);
        write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> _");
        write_elem(dotfile, node->elem);
        fprintf(dotfile, " [style=invis]\n");
    }

    if ((x = node->right) != NULL) {
        DsBinSearchTreeNode right_min = x;
        size_t right_distance = 1;
        DsBinSearchTreeNode _x;
        while ((_x = right_min->left)) {
            right_min = _x;
            right_distance += 1;
        }
        if (right_distance <= distance) {
            target = right_min->elem;
            distance = right_distance;
        }

        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            write_elem(dotfile, x->elem);
            fprintf(dotfile, " [group=");
            write_elem(dotfile, x->elem);
            fputs("]\n", dotfile);
        }
        fprintf(dotfile, "    ");
        write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> ");
        write_elem(dotfile, x->elem);
        fputc('\n', dotfile);
        DsBinSearchTreeNode_ExportGraphviz(tree, x, dotfile, write_elem);
    }
    if (distance > 1) {
        if (target) {
            fprintf(dotfile, "    {rank=same; _");
            write_elem(dotfile, node->elem);
            fprintf(dotfile, "; ");
            write_elem(dotfile, target);
            fputs("}\n", dotfile);
        }
    }
}

#if 0
static void DsBinSearchTreeNode_ExportNodeGraphviz(DsBinSearchTree tree,
                                                   DsBinSearchTreeNode node, 
                                                   FILE* dotfile,
                                                   void (*write_elem)(FILE* f, void* elem)) {
    if (node->left) {
        write_elem(dotfile, node->elem);
        fprintf(dotfile, "->");
        write_elem(dotfile, node->left->elem);
        fputs(";", dotfile);
    }
    if (node->right) {
        write_elem(dotfile, node->elem);
        fprintf(dotfile, "->");
        write_elem(dotfile, node->right->elem);
        fputs(";", dotfile);
    }
}


void DsBinSearchTreeNode_ExportGraphviz(DsBinSearchTree tree,
                                        DsBinSearchTreeNode node, 
                                        FILE* dotfile,
                                        void (*write_elem)(FILE* f, void* elem)) {
    if (node) {
        DsBinSearchTreeNode_ExportGraphviz(tree, node->left, dotfile, write_elem);
        DsBinSearchTreeNode_ExportNodeGraphviz(tree, node, dotfile, write_elem);
        DsBinSearchTreeNode_ExportGraphviz(tree, node->right, dotfile, write_elem);
    }
}
#endif


/*==============================*\
| Adelson-Velsky and Landis Tree |
\*==============================*/


static inline DsAvlTreeNode DsAvlTree_CompareNode(DsAvlTree tree,
                                                  DsAvlTreeNode node1,
                                                  DsAvlTreeNode node2) {
    return memcmp(node1->elem, node2->elem, tree->elem_size) ? node1
                                                             : node2;
}

static inline void DsAvlTree_Assign(DsAvlTree tree,
                                    DsAvlTreeNode node,
                                    void* value) {
    memcpy(node->elem, value, tree->elem_size);
}

static inline int DsAvlTree_Compare(DsAvlTree tree,
                                    DsAvlTreeNode node,
                                    void* value) {
    return memcmp(node->elem, value, tree->elem_size);
}


static int DsAvlTree_Height(DsAvlTreeNode node) {
    if (node == NULL)
        return -1;  // empty tree
    return node->height;
}


static DsAvlTreeNode DsAvlTree_LeftSingleRotate(DsAvlTreeNode k2) {
    DsAvlTreeNode k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(DsAvlTree_Height(k2->left), DsAvlTree_Height(k2->right)) + 1;
    k1->height = max(DsAvlTree_Height(k1->left), k2->height) + 1;
    return k1;  // new root
}

static DsAvlTreeNode DsAvlTree_RightSingleRotate(DsAvlTreeNode k1) {
    DsAvlTreeNode k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(DsAvlTree_Height(k1->left), DsAvlTree_Height(k1->right)) + 1;
    k2->height = max(DsAvlTree_Height(k2->right), k1->height) + 1;
    return k2;  // new root
}


static DsAvlTreeNode DsAvlTree_LeftDoubleRotate(DsAvlTreeNode k3) {
    k3->left = DsAvlTree_RightSingleRotate(k3->left);
    return DsAvlTree_LeftSingleRotate(k3);
}


static DsAvlTreeNode DsAvlTree_RightDoubleRotate(DsAvlTreeNode k1) {
    k1->right = DsAvlTree_LeftSingleRotate(k1->right);
    return DsAvlTree_RightSingleRotate(k1);
}



DsAvlTree DsAvlTree_New_Inner(size_t elem_size) {
    DsAvlTree tree = malloc(sizeof(struct DsAvlTreeHeader));
    check_null(tree);
    tree->body = NULL;
    tree->elem_size = elem_size;
    return tree;
}


DsAvlTree DsAvlTree_FromCArray_Inner(const void* values,
                                     size_t length,
                                     size_t elem_size) {
    DsAvlTree tree = DsAvlTree_New_Inner(elem_size);
    for (size_t i = 0; i < length; i++) {
        DsAvlTree_Insert(tree, (uint8_t*)values + i * elem_size);
    }
    return tree;
}


DsAvlTree DsAvlTree_Clear(DsAvlTree tree) {
    DsAvlTreeNode_Clear(tree->body);
    tree->body = NULL;
}


DsAvlTreeNode DsAvlTreeNode_Clear(DsAvlTreeNode tree) {
    if (tree != NULL) {
        DsAvlTreeNode_Clear(tree->left);
        DsAvlTreeNode_Clear(tree->right);
        free(tree->elem);
        free(tree);
    }
    return NULL;
}


void DsAvlTree_Delete(DsAvlTree tree) {
    DsAvlTreeNode_Clear(tree->body);
    free(tree);
}


DsAvlTreeNode DsAvlTree_FindMaxNode(DsAvlTree tree) {
    if (tree->body != NULL)
        return DsAvlTreeNode_FindMaxNode(tree->body);
    else
        return NULL;
}

DsAvlTreeNode DsAvlTreeNode_FindMaxNode(DsAvlTreeNode node) {
    if (node != NULL)
        while (node->right != NULL)
            node = node->right;
    return node;
}

void* DsAvlTree_FindMaxValue(DsAvlTree tree) {
    if (tree->body != NULL)
        return DsAvlTreeNode_FindMaxNode(tree->body)->elem;
    else
        return NULL;
}


DsAvlTreeNode DsAvlTree_FindMinNode(DsAvlTree tree) {
    if (tree->body != NULL)
        return DsAvlTreeNode_FindMinNode(tree->body);
    else
        return NULL;
}

DsAvlTreeNode DsAvlTreeNode_FindMinNode(DsAvlTreeNode node) {
    if (node != NULL)
        while (node->left != NULL)
            node = node->left;
    return node;
}

void* DsAvlTree_FindMinValue(DsAvlTree tree) {
    if (tree->body != NULL)
        return DsAvlTreeNode_FindMinNode(tree->body)->elem;
    else
        return NULL;
}



void DsAvlTree_Insert(DsAvlTree tree, void* value) {
    tree->body = DsAvlTreeNode_Insert(tree, tree->body, value);
}

DsAvlTreeNode DsAvlTreeNode_Insert(DsAvlTree tree, 
                                   DsAvlTreeNode node, 
                                   void* value) {
    if (node == NULL) {    // create and return a one-node tree
        node = malloc(sizeof(struct DsAvlTreeNode));
        check_null(node);
        node->elem = malloc(tree->elem_size);
        check_null(node->elem);
        DsAvlTree_Assign(tree, node, value);
        node->height = 0;
        node->left = node->right = NULL;
    }
    else if (DsAvlTree_Compare(tree, node, value) > 0) {
        node->left = DsAvlTreeNode_Insert(tree, node->left, value);
        if (DsAvlTree_Height(node->left) - DsAvlTree_Height(node->right) == 2) {
            if (DsAvlTree_Compare(tree, node->left, value) > 0)
                node = DsAvlTree_LeftSingleRotate(node);
            else
                node = DsAvlTree_LeftDoubleRotate(node);
        }
    }
    else if (DsAvlTree_Compare(tree, node, value) < 0) {
        node->right = DsAvlTreeNode_Insert(tree, node->right, value);
        if (DsAvlTree_Height(node->right) - DsAvlTree_Height(node->left) == 2) {
            if (DsAvlTree_Compare(tree, node->right, value) < 0)
                node = DsAvlTree_RightSingleRotate(node);
            else
                node = DsAvlTree_RightDoubleRotate(node);
        }
    }
    /* elem is in the tree already */
    node->height = max(DsAvlTree_Height(node->left), DsAvlTree_Height(node->right)) + 1;
    return node;
}



static int DsAvlTreeNode_GetBalance(DsAvlTreeNode node) {
    if (node == NULL)
        return -1;
    return DsAvlTree_Height(node->left) - DsAvlTree_Height(node->right);
}


void DsAvlTree_Remove(DsAvlTree tree, void* value) {
    if (tree->body == NULL)
        return;
    else
        tree->body = DsAvlTreeNode_Remove(tree, tree->body, value);
}


/**
 * reference: https://www.geeksforgeeks.org/deletion-in-an-avl-tree/
 * @param tree
 * @param node
 * @param value
 * @return
 */
DsAvlTreeNode DsAvlTreeNode_Remove(DsAvlTree tree, DsAvlTreeNode node, void* value) {

    if (node == NULL)
        return NULL;

    if (DsAvlTree_Compare(tree, node, value) > 0)
        node->left = DsAvlTreeNode_Remove(tree, node->left, value);
    else if (DsAvlTree_Compare(tree, node, value) < 0)
        node->right = DsAvlTreeNode_Remove(tree, node->right, value);
    else {
        if ((node->left == NULL) || (node->right == NULL)) {
            DsAvlTreeNode temp = node->left ? node->left 
                                            : node->right;
            if (temp == NULL) {
                temp = node;
                node = NULL;
            }
            else {
                DsAvlTree_Assign(tree, node, temp->elem);
                node->left = temp->left;
                node->right = temp->right;
            }
            free(temp->elem);
            free(temp);
        }
        else {
            DsAvlTreeNode temp = DsAvlTreeNode_FindMinNode(node->right);
            DsAvlTree_Assign(tree, node, temp->elem);
            node->right = DsAvlTreeNode_Remove(tree, node->right, temp->elem);
        }
    }
    if (node == NULL)
        return node;

    node->height = 1 + max(DsAvlTree_Height(node->left), DsAvlTree_Height(node->right));

    int balance = DsAvlTree_Height(node->left) - DsAvlTree_Height(node->right);

    if (balance > 1 && DsAvlTreeNode_GetBalance(node->left) >= 0)
        return DsAvlTree_LeftSingleRotate(node);
    if (balance > 1 && DsAvlTreeNode_GetBalance(node->left) < 0)
        return DsAvlTree_LeftDoubleRotate(node);
    if (balance < -1 && DsAvlTreeNode_GetBalance(node->right) <= 0)
        return DsAvlTree_RightSingleRotate(node);
    if (balance < -1 && DsAvlTreeNode_GetBalance(node->right) > 0)
        return DsAvlTree_RightDoubleRotate(node);

    return node;
}



static void DsAvlTreeNode_PrintNode(DsAvlTree tree,
                                    DsAvlTreeNode node,
                                    size_t depth,
                                    void (*print_elem)(void* elem)) {
    if (node) {
        for (size_t i = 0; i < depth; i++)
            putchar('-');
        putchar(' ');
        print_elem(node->elem);
        putchar('\n');
    }
}

void DsAvlTreeNode_Print(DsAvlTree tree,
                         DsAvlTreeNode node,
                         size_t depth,
                         void (*print_elem)(void* elem)) {
    if (node) {
        DsAvlTreeNode_Print(tree, node->left, depth + 1, print_elem);
        DsAvlTreeNode_PrintNode(tree, node, depth, print_elem);
        DsAvlTreeNode_Print(tree, node->right, depth + 1, print_elem);
    }
}

void DsAvlTree_Print(DsAvlTree tree, void (*print_elem)(void* elem)) {
    puts("AVL Tree {");
    if (tree->body) {
        DsAvlTreeNode_Print(tree, tree->body, 0, print_elem);
    }
    puts("}");
}


void DsAvlTree_ExportGraphviz(DsAvlTree tree,
                              const char* dotfile_name,
                              const char* pngfile_name,
                              void (*write_elem)(FILE*, void*)) {
    FILE* dotfile = fopen(dotfile_name, "w");
    DsAvlTreeNode x;
    if ((x = tree->body)) {
        fputs("digraph G {", dotfile);
        fputs("    graph [nodesep=0.1]", dotfile);
        fputs("    node [shape=circle]", dotfile);
        fputs("    edge [arrowhead=vee]", dotfile);
        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            write_elem(dotfile, x->elem);
            fprintf(dotfile, "[group =");
            write_elem(dotfile, x->elem);
            fputs("]", dotfile);
        }
        DsAvlTreeNode_ExportGraphviz(tree, x, dotfile, write_elem);
        fputs("}", dotfile);
        fclose(dotfile);
        char cmd[528];
        sprintf(cmd, "dot -Tpng %s -o %s\n", dotfile_name, pngfile_name);
        system(cmd);
    }
}


void DsAvlTreeNode_ExportGraphviz(DsAvlTree tree,
                                  DsAvlTreeNode node,
                                  FILE* dotfile,
                                  void (*write_elem)(FILE* f, void* elem)) {
    void* target = NULL;
    size_t distance = 0;

    DsAvlTreeNode x;
    if ((x = node->left) != NULL) {
        DsAvlTreeNode left_max = x;
        size_t left_distance = 1;
        DsAvlTreeNode _x;
        while ((_x = left_max->right)) {
            left_max = _x;
            left_distance += 1;
        }
        target = left_max->elem;
        distance = left_distance;

        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            write_elem(dotfile, x->elem);
            fprintf(dotfile, " [group=");
            write_elem(dotfile, x->elem);
            fputs("]", dotfile);
        }
        fprintf(dotfile, "    ");
        write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> ");
        write_elem(dotfile, x->elem);
        fputc('\n', dotfile);
        DsAvlTreeNode_ExportGraphviz(tree, x, dotfile, write_elem);
    }
    if (node->left || node->right) {
        fprintf(dotfile, "    _");
        write_elem(dotfile, node->elem);
        fputs(" [group=", dotfile);
        write_elem(dotfile, node->elem);
        fputs(", label=\"\", width=0, style=invis]\n    ", dotfile);
        write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> _");
        write_elem(dotfile, node->elem);
        fprintf(dotfile, " [style=invis]\n");
    }

    if ((x = node->right) != NULL) {
        DsAvlTreeNode right_min = x;
        size_t right_distance = 1;
        DsAvlTreeNode _x;
        while ((_x = right_min->left)) {
            right_min = _x;
            right_distance += 1;
        }
        if (right_distance <= distance) {
            target = right_min->elem;
            distance = right_distance;
        }

        if (x->left || x->right) {
            fprintf(dotfile, "    ");
            write_elem(dotfile, x->elem);
            fprintf(dotfile, " [group=");
            write_elem(dotfile, x->elem);
            fputs("]\n", dotfile);
        }
        fprintf(dotfile, "    ");
        write_elem(dotfile, node->elem);
        fprintf(dotfile, " -> ");
        write_elem(dotfile, x->elem);
        fputc('\n', dotfile);
        DsAvlTreeNode_ExportGraphviz(tree, x, dotfile, write_elem);
    }
    if (distance > 1) {
        if (target) {
            fprintf(dotfile, "    {rank=same; _");
            write_elem(dotfile, node->elem);
            fprintf(dotfile, "; ");
            write_elem(dotfile, target);
            fputs("}\n", dotfile);
        }
    }
}




