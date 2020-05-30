// Author: Mohit Khanna
// Student ID: z5266543
// Platform: Mac

#include "MyGraph.h" // includes MyGraph.h from the current working directory

// heap node
typedef struct HeapNode
{
    double key;               //key of this item
    struct VertexNode *vnode; // task name
    struct HeapNode *parent;  //pointer to parent
    struct HeapNode *left;    //pointer to left child
    struct HeapNode *right;   //pointer to right child
} HeapNode;

//data type for a priority queue (heap)
typedef struct Heap
{                       //this is heap header
    int size;           // count of items in the heap
    HeapNode *LastNode; // last node pointer
    HeapNode *root;     // pointer to the root of heap
} Heap;

// create a new heap node to store an item
HeapNode *newHeapNode(double k, VertexNode *node, HeapNode *L, HeapNode *R, HeapNode *P)
{
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->vnode = node;
    new->left = L;   // left child
    new->right = R;  // righ child
    new->parent = P; // parent
    return new;
}

// create a new empty heap-based priority queue
Heap *newHeap()
{
    Heap *T;
    T = malloc(sizeof(Heap));
    assert(T != NULL);
    T->size = 0;
    T->LastNode = NULL;
    T->root = NULL;
    return T;
}

// helper function that bubbles up the value inserted
// if the value is less than the parent it will swap the positions of the node with its parent
// recursively bubbles through parent stream
HeapNode *BubbleUp(Heap *T, HeapNode *node)
{
    if (!node->parent) //base case trying to swap at root
    {
        T->root = node;
        node->vnode->h = node;
        return node;
    }
    else
    {
        if (node->parent->key > node->key)
        {
            if (node->parent->left == node) // case of child is on left
            {
                HeapNode *temp = newHeapNode(node->key, node->vnode, node->parent, node->parent->right, NULL);
                temp->left->left = node->left;
                temp->left->right = node->right;
                if (temp->left->left)
                    temp->left->left->parent = temp->left;
                if (temp->left->right)
                    temp->left->right->parent = temp->left;
                if (temp->right)
                    temp->right->parent = temp;
                if (node->parent->parent)
                {
                    temp->parent = node->parent->parent;
                    if (node->parent->parent->left == node->parent)
                        node->parent->parent->left = temp;
                    else
                        node->parent->parent->right = temp;
                }
                temp->left->parent = temp;
                node = temp;
                node->vnode->h = node;
                BubbleUp(T, node);
                return node->left;
            }
            else if (node->parent->right == node) // case of child is on right
            {
                HeapNode *temp = newHeapNode(node->key, node->vnode, node->parent->left, node->parent, NULL);
                temp->right->right = node->right;
                temp->right->left = node->right;
                if (temp->right->right)
                    temp->right->right->parent = temp->right;
                if (temp->right->left)
                    temp->right->left->parent = temp->left;
                if (temp->left)
                    temp->left->parent = temp;
                if (node->parent->parent)
                {
                    temp->parent = node->parent->parent;
                    if (node->parent->parent->left == node->parent)
                        node->parent->parent->left = temp;
                    else
                        node->parent->parent->right = temp;
                }
                temp->right->parent = temp;
                node = temp;
                node->vnode->h = node;
                BubbleUp(T, node);
                return node->right;
            }
        }
        else // doesn't need to be swapped
        {
            node->vnode->h = node;
            return node;
        }
    }
    return NULL;
}

// helper function that bubbles down the value inserted
// if the value is more than the parent it will swap the positions of the node with its parent
// recursively bubbles through parent stream
HeapNode *BubbleDown(Heap *T, HeapNode *node)
{
    if (!node->left && !node->right) //base case trying to swap at root
    {
        node->vnode->h = node;
        return node;
    }
    else
    {
        if (node->left->key > node->key)
        {
            HeapNode *temp = newHeapNode(node->left->key, node->left->vnode, node, node->right, NULL);
            node->right = node->left->right;
            node->left = node->left->left;
            if (node->parent)
            {
                if (node->parent->left == node)
                    node->parent->left = temp;
                else
                    node->parent->right = temp;
            }
            else
                T->root = temp;
            node->parent = temp;
            temp->vnode->h = temp;
            BubbleDown(T, temp);
            return node;
        }
        else if (node->right->key > node->key)
        {
            HeapNode *temp = newHeapNode(node->right->key, node->right->vnode, node->left, node, NULL);
            node->left = node->right->left;
            node->right = node->right->right;
            if (node->parent)
            {
                if (node->parent->left == node)
                    node->parent->left = temp;
                else
                    node->parent->right = temp;
            }
            else
                T->root = temp;
            node->parent = temp;
            temp->vnode->h = temp;
            BubbleDown(T, temp);
            return node;
        }
    }
    return NULL;
}

// Function to find the next last node (after last node)
// Used to replace LastNode
// Time complexity worst case is log(n)
HeapNode *FindNextLastNode(Heap *T)
{
    HeapNode *node;
    if (T->size <= 1)
        return NULL;
    if (T->size == 2)
        return T->root;
    else
    {
        if (T->LastNode == T->LastNode->parent->right)
            return T->LastNode->parent->left;
        else
        { //left node
            // moves up tree until finds a right node, then goes up one
            // and down the left subtree, then all the way down right node until leaf
            node = T->LastNode->parent;
            while (node->parent != NULL)
            {
                if (node == node->parent->right)
                {
                    node = node->parent->left; // traverse around the point
                    break;
                }
                node = node->parent;
            }

            while (node->right != NULL)
                node = node->right;
            return node;
        }
    }
}

// put the time complexity analysis for Insert() here
// Time complexity is O(log n) for insertion
// This is due to bubble up
HeapNode *Insert(Heap *T, double k, VertexNode *VertNode)
{
    HeapNode *node = newHeapNode(k, VertNode, NULL, NULL, NULL), *temp;
    if (T->size == 0) // Base cases for small tree
    {
        HeapNode *node = newHeapNode(k, VertNode, NULL, NULL, NULL);
        T->root = node;
    }
    else if (T->size == 1) // case of only 1 node
    {
        node->parent = T->root;
        if (T->size == 1)
            T->root->left = node;
        else
            T->root->right = node;
    }
    else
    {
        if (!T->LastNode->parent->right)
        {
            T->LastNode->parent->right = node;
            node->parent = T->LastNode->parent;
        }
        else //move up until find root or leaf node
        { 
            while (1) //temporarily moves the last node to move through tree)
            { 
                if (T->LastNode == T->root)
                {
                    while (T->LastNode->left)
                        T->LastNode = T->LastNode->left;
                    T->LastNode->left = node;
                    node->parent = T->LastNode;
                    break;
                }
                else if (T->LastNode->parent->left == T->LastNode) //Left leaf
                { 
                    T->LastNode = T->LastNode->parent->right;
                    while (T->LastNode->left)
                        T->LastNode = T->LastNode->left;
                    T->LastNode->left = node;
                    node->parent = T->LastNode;
                    break;
                }
                T->LastNode = T->LastNode->parent;
            }
        }
    }
    T->LastNode = BubbleUp(T, node);
    T->size++;
    return node;
}

// returns 1 if right child is smaller/equal or 0 if left is smaller
int FindMinChild(HeapNode *node)
{
    if (node->right != NULL)
    {
        if (node->left->key >= node->right->key)
            return 1;
        else
            return 0;
    }
    return 0;
}

// returns the number of children of the current node
int NumChildNodes(HeapNode *node)
{
    if (!node->left && !node->right)
        return 0;
    else if (!node->right)
        return 1;
    return 2;
}

// recursively bubbles down tree until it hits the end
// then moves last node to the current position and updates last node
// exception cases exists for example if the last node is the current node
HeapNode *BubbleOut(Heap *T, HeapNode *node)
{
    int ChildNodes = NumChildNodes(node); // number of child nodes
    if (ChildNodes == 0)
    {
        if (node->parent == NULL)
        { // root case

            T->size--;
            T->root = NULL;
            return node;
        }
        else
        {
            HeapNode *NextLastNode = FindNextLastNode(T);
            T->size--;
            if (T->LastNode == node)
            {
                T->LastNode = NextLastNode;
                return NULL;
            }
            else
            {
                HeapNode *NewNode = newHeapNode(T->LastNode->key, T->LastNode->vnode, NULL, NULL, NULL);
                if (T->LastNode->parent->left == T->LastNode)
                    T->LastNode->parent->left = NULL;
                else
                    T->LastNode->parent->right = NULL;
                if (NextLastNode == node)
                    T->LastNode = NewNode;
                else
                    T->LastNode = NextLastNode;

                return NewNode;
            }
        }
    }
    else if (ChildNodes == 1)
    {
        HeapNode *NewNode = newHeapNode(node->left->key, node->left->vnode, NULL, NULL, node->parent);
        T->LastNode = FindNextLastNode(T);
        T->size--;
        if (!NewNode->parent)
            T->root = NewNode;
        return NewNode;
    }
    else if (ChildNodes == 2)
    {
        int IndicatorMin = FindMinChild(node);
        if (IndicatorMin == 0)
        {
            HeapNode *NewNode;
            if (T->LastNode != node->right)
                NewNode = newHeapNode(node->left->key, node->left->vnode, BubbleOut(T, node->left), node->right, node->parent);
            else
                NewNode = newHeapNode(node->left->key, node->left->vnode, BubbleOut(T, node->left), NULL, node->parent);
            if (NewNode->right)
                NewNode->right->parent = NewNode;
            if (NewNode->left)
                NewNode->left->parent = NewNode;
            if (!NewNode->parent)
                T->root = NewNode;
            return NewNode;
        }
        else if (IndicatorMin == 1)
        {
            HeapNode *temp, *NewNode = newHeapNode(node->right->key, node->right->vnode, node->left, BubbleOut(T, node->right), node->parent);
            if (NewNode->left)
                NewNode->left->parent = NewNode;
            if (NewNode->right)
                NewNode->right->parent = NewNode;
            if (!NewNode->parent)
                T->root = NewNode;
            NewNode->vnode->h = NewNode;
            return NewNode;
        }
    }
    return NULL;
}

// removes the minimum node and rearranges the heap
// Time complexity is O(logN)
HeapNode *RemoveMin(Heap *T)
{
    HeapNode *node = T->root;
    BubbleOut(T, node);
    return node;
}