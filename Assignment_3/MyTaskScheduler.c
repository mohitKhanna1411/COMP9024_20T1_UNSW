// Author: Mohit Khanna
// Student ID: z5266543
// Platform: Mac

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define MAX 9999 // constant MAX

// This template is only a guide
// You need to include additional fields, data structures and functions

// data type for heap nodes
typedef struct HeapNode
{
    // each node stores the priority (key), name, execution time,
    //  release time and deadline of one task
    int key;      //key of this task
    int TaskName; //task name
    int Etime;    //execution time of this task
    int Rtime;    // release time of this task
    int Dline;    // deadline of this task
    int degree;   //degree of the key
    struct HeapNode *child;
    struct HeapNode *parent;
    struct HeapNode *sibling;
} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap
{             //this is heap header
    int size; // count of items in the heap
    HeapNode *root;
} BinomialHeap;

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->TaskName = n;
    new->Etime = c;
    new->Rtime = r;
    new->Dline = d;
    new->degree = 0;
    new->child = NULL;
    new->parent = NULL;
    new->sibling = NULL;
    return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
    BinomialHeap *T;
    T = malloc(sizeof(BinomialHeap));
    assert(T != NULL);
    T->root = NULL;
    return T;
}

// merges two binomial heaps with same degree
// Lets assume we have N tasks to schedule
// binomial heap can have at most 1 + logN binomial trees
// ======> Hence Time Complexity :O(logN) <======
HeapNode *heapMerge(BinomialHeap *H1, BinomialHeap *H2)
{
    HeapNode *root1 = H1->root;
    HeapNode *root2 = H2->root;
    if (root1 == NULL) // if H1 heap is empty
        return root2;  // return H2
    if (root2 == NULL) // check is H2 heap is empty
        return root1;  // return H1
    HeapNode *node1;
    HeapNode *node2;

    if (root1->degree <= root2->degree) // check if degree of H1 is smaller or not
    {
        node1 = root1;          // make root1 as node1
        root1 = root1->sibling; // traverse through the siblings of H1
    }
    else
    {
        node1 = root2;          // make root2 as node1
        root2 = root2->sibling; // traverse through the siblings of H1
    }
    node2 = node1;                         // copy of node1
    while (root1 != NULL && root2 != NULL) // perform while operation untill both the heaps arent exhausted
    {
        if (root1->degree <= root2->degree) // check the degrees
        {
            node2->sibling = root1; //  make sibling of H1 as the root
            root1 = root1->sibling;
        }
        else
        {
            node2->sibling = root2; // make sibling of H2 as the root
            root2 = root2->sibling;
        }

        node2 = node2->sibling; // traverse through the siblings
    }
    node2->sibling = root1 != NULL ? root1 : root2; // making root of heaps are the siblings of other
    return node1;                                   // return the root node of merged heaps
}

// union two binomial heaps with same degree
// Lets assume we have N tasks to schedule
// time complexity of heapMerge is O(logN), while loop will execute O(logN) times
// O(logN) + O(logN)
// ======> Hence Time Complexity :O(logN) <======
HeapNode *heapUnion(BinomialHeap *H1, BinomialHeap *H2)
{
    HeapNode *curr = heapMerge(H1, H2); // call by value
    if (curr == NULL)
        return NULL;
    H1->root = NULL;
    H2->root = NULL;
    HeapNode *prev = NULL;
    HeapNode *temp = curr;
    HeapNode *next = curr->sibling;
    while (next != NULL) // execute unitll next in reached null
    {
        if ((next->sibling != NULL && next->sibling->degree == temp->degree) || temp->degree != next->degree) // check if the degrees are same
        {
            prev = temp; // move forward with temp node
            temp = next;
        }
        else // if the degrees are same ==>union them
        {
            if (next->key > temp->key) //cehcking on key level
            {
                temp->sibling = next->sibling;
                next->parent = temp;
                next->sibling = temp->child;
                temp->child = next; // make next as child of temp
                temp->degree++;     // incerase the degree of temp tree
            }
            else
            {
                if (prev == NULL)
                    curr = next;
                else
                    prev->sibling = next;

                temp->parent = next;
                temp->sibling = next->child;
                next->child = temp; // make temp as child of next
                next->degree++;     // incerase the degree of next tree
                temp = next;
            }
        }
        next = temp->sibling; // iterate over all the siblings
    }
    return curr; // return root node of the merged heap
}

// inert node into a binomial heap
// Lets assume we have N tasks to schedule
// time complexity of heapUnion is logN
// ======> Hence Time Complexity :O(logN) <======
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline
    // You don't need to check if this task already exists in T
    HeapNode *node = newHeapNode(k, n, c, r, d);
    BinomialHeap *H2 = newHeap();
    H2->root = node;
    T->root = heapUnion(T, H2); // call by value
    free(H2);                   // free H2 after union with existing binomail heap
}

// returns minimum node and modifies the heap T
// Lets assume we have N tasks to schedule
// binomial heap can have at most 1 + logN binomial trees
// ======> Hence Time Complexity :O(logN) <======
HeapNode *RemoveMin(BinomialHeap *T)
{
    if (T->root == NULL)
        return NULL;
    HeapNode *temp = NULL;
    HeapNode *curr = T->root;
    HeapNode *node = curr;
    HeapNode *next = curr->sibling;
    while (next != NULL) // execute untill next is null
    {
        if (next->key < curr->key)
        {
            curr = next;
            temp = node;
        }
        node = next;
        next = next->sibling; // traverse through only the siblings of the root node
    }
    // promoting the children to root level after removal of min/root node
    if (curr == T->root)
        T->root = curr->sibling;
    else
        temp->sibling = curr->sibling;
    HeapNode *child = curr->child;
    HeapNode *root = NULL;
    while (child != NULL) // execute untill child is null
    {
        HeapNode *next = child->sibling;
        child->sibling = root;
        child->parent = NULL;
        root = child;
        child = next;
    }
    // union of binomial heap of same degrees
    BinomialHeap *N = newHeap();
    N->root = root;
    T->root = heapUnion(T, N); // call by value
    free(N);                   // free heap N after union
    return curr;               // return min node from the binomial heap
}

// returns value of minimum key from the heap T without modifing the heap T
// Lets assume we have N tasks to schedule
// binomial heap can have at most 1 + logN binomial trees
// ======> Hence Time Complexity :O(logN) <======
int Min(BinomialHeap *T)
{
    HeapNode *curr = T->root;
    if (curr == NULL) // check if binomial heap is null
        return MAX;   // return MAX i.e 9999
    HeapNode *next = curr->sibling;
    while (next != NULL) // execute untill the next node is null
    {
        if (next->key < curr->key) // check if next is smaller than current
            curr = next;           // make next as current node
        next = next->sibling;      // traverse through the sibling of the root nodes to find minimum because of heap property
    }
    return curr->key; // return the minimum element
}

// Lets assume we have N tasks to schedule
// time complexity of removeMin, insert, min each is O(log N) and there are N tasks iterating through while loop
// ======> Hence Time Complexity :O(N * logN) <======
int TaskScheduler(char *f1, char *f2, int m)
{
    int p = 0;
    int array[MAX];
    char t;
    int integer;
    FILE *file1 = fopen(f1, "r"); // open file in read mode
    FILE *file2 = fopen(f2, "w"); // open file in write mode
    if (file1 == NULL)
    {
        printf("file1 does not exist\n");
        exit(1); // exit the program
    }
    while (!feof(file1))
    {
        if (fscanf(file1, "%d", &integer) == 1) // check if the input is valid integer
            array[p++] = integer;               // create an array of valid integer elements
        else
        {
            t = fgetc(file1);
            if (t != '\0' && t != EOF) // check if character is illegal input
            {
                printf("input error when reading the attribute of the task %d\n", array[p - (p % 4)]); // error printing
                exit(1);                                                                               // exit the program
            }
        }
    }
    fclose(file1);

    for (int n = 0; n < p - 3; n += 4)
    {
        if (array[n] < 0 || array[n + 1] <= 0 || array[n + 2] < 0 || array[n + 3] <= 0) // checking if the input meets all validation conditions
        {
            printf("input error when reading the attribute of the task %d\n", array[n]); // error printing
            exit(1);                                                                     // exit the program
        }
    }

    //This is FIRST priority queue
    BinomialHeap *releaseBH = newHeap();
    for (int k = 0; k < p - 3; k += 4) //Binomial heap releaseBH
    {
        Insert(releaseBH, array[k + 2], array[k], array[k + 1], array[k + 2], array[k + 3]); // with release time as keys
        releaseBH->size++;                                                                   // size increament
    }

    //This is SECOND priority queue
    BinomialHeap *coreBH = newHeap(); //Binomial heap coreBH
    int r = Min(releaseBH);
    for (int j = 1; j <= m; j++)
    {
        Insert(coreBH, r, j, 0, 0, 0); // with minimum release times as keys
        coreBH->size++;
    }

    HeapNode *node;
    //This is THIRD priority queue
    BinomialHeap *deadlinesBH = newHeap(); //Binomial heap deadlinesBH
    while (releaseBH->size > 0)
    {
        int s1 = Min(releaseBH); // get minimum node of releaseBH
        int s2 = Min(coreBH);    // get minimum node of coreBH
        int s = (s1 >= s2) ? s1 : s2;
        while (Min(releaseBH) <= s) // execute untill s is greater than min of releaseBH
        {
            node = RemoveMin(releaseBH); //remove node from releaseBH to insert into deadlinesBH
            releaseBH->size--;
            Insert(deadlinesBH, node->Dline, node->TaskName, node->Etime, node->Rtime, node->Dline); // with deadline as keys
            deadlinesBH->size++;
        }

        while (deadlinesBH->size > 0) // execute untill dealines binomial heap is exhausted
        {
            HeapNode *node_core;
            node_core = RemoveMin(coreBH); //key is scheduling point
            coreBH->size--;
            int core = node_core->TaskName;

            HeapNode *deadline_node;
            deadline_node = RemoveMin(deadlinesBH);
            deadlinesBH->size--;
            int f = (deadline_node->Rtime >= node_core->key) ? deadline_node->Rtime : node_core->key;
            Insert(coreBH, f + deadline_node->Etime, core, 0, 0, 0);            // inserting node into coreBH
            coreBH->size++;                                                     // increamenting the size
            fprintf(file2, "%d Core%d %d\n", deadline_node->TaskName, core, f); // writing into the file2
            if (deadline_node->key < f + deadline_node->Etime)                  //check if the deadlines are missed
            {
                fprintf(file2, "where task %d misses its deadline.\n", deadline_node->TaskName); // write which task's dealines misses
                return 0;                                                                        // return 0 to indicate that no feasible solution exists
            }

            // to include all tasks before scheduling point
            while (Min(releaseBH) <= Min(coreBH))
            {
                node = RemoveMin(releaseBH);
                releaseBH->size--;
                Insert(deadlinesBH, node->Dline, node->TaskName, node->Etime, node->Rtime, node->Dline);
                deadlinesBH->size++;
            }
        }
    }
    fclose(file2);
    return 1;
}

int main() //sample main for testing
{
    int i;
    i = TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
    if (i == 0)
        printf("No feasible schedule!\n");
    /* There is a feasible schedule on 4 cores */
    i = TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
    if (i == 0)
        printf("No feasible schedule!\n");
    /* There is no feasible schedule on 3 cores */
    i = TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
    if (i == 0)
        printf("No feasible schedule!\n");
    /* There is a feasible schedule on 5 cores */
    i = TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
    if (i == 0)
        printf("No feasible schedule!\n");
    /* There is no feasible schedule on 4 cores */
    i = TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
    if (i == 0)
        printf("No feasible schedule!\n");
    /* There is no feasible schedule on 2 cores */
    i = TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
    if (i == 0)
        printf("No feasible schedule!\n");
    /* There is a feasible schedule on 2 cores */
    return 0;
}
