// Author: Mohit Khanna
// Student ID: z5266543
// Platform: Mac

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "MyHeap.h" // includes MyHeap.h from the current working directory
#define MAX 999     // constant MAX

Graph g, r;

// creates an egde between two given vertices in constant time
// ======> Hence Time Complexity :O(1) <======
int InsertEdge(Graph g, Edge *e)
{
    VertexNode *v1;
    VertexNode *v2;
    AdjacentNode *adj1;

    v1 = CheckVertexInGraph(g, e->p1);
    v2 = CheckVertexInGraph(g, e->p2);
    if (v1 && v2) // case of both vertex in Graph
    {
        adj1 = CheckVertexInAdj(v1, e->p2);
        if (adj1) // edge is in graph
            return 0;
        else // edge is not in the graph
        {
            adj1 = newAdjacentNode(v2);
            if (v1->adj && v1->lastadj)
                v1->lastadj->next = adj1;
            else
                v1->adj = adj1;

            v1->lastadj = adj1;
            g->nE += 1;
        }
    }
    else if (v1) // case of vertex 1 in the Graph but not vertex 2
    {
        v2 = newVertexNode(e->p2);
        g->lastvert->next = v2;
        g->lastvert = v2;
        adj1 = newAdjacentNode(v2);
        if (v1->adj && v1->lastadj)
            v1->lastadj->next = adj1;
        else
            v1->adj = adj1;

        v1->lastadj = adj1;
        g->nV += 1;
        g->nE += 1;
    }
    else if (v2) // case of vertex 2 in the Graph but not vertex 1
    {
        v1 = newVertexNode(e->p1);
        g->lastvert->next = v1;
        g->lastvert = v1;
        adj1 = newAdjacentNode(v2);
        if (v1->adj && v1->lastadj)
            v1->lastadj->next = adj1;
        else
            v1->adj = adj1;

        v1->lastadj = adj1;
        g->nV += 1;
        g->nE += 1;
    }
    else // both vertex not in the Graph
    {
        v1 = newVertexNode(e->p1);
        v2 = newVertexNode(e->p2);
        adj1 = newAdjacentNode(v2);
        v1->next = v2;
        v1->adj = adj1;
        v1->lastadj = adj1;
        g->nV += 2;
        g->nE += 1;
        if (g->vertices == NULL)
            g->vertices = v1;
        else
            g->lastvert->next = v1;

        g->lastvert = v2;
    }
    return 1; // successfully created the edge
}

// creates the graph by reading vertices and edges from given files
// it also creates tranpose graphs if the flag is 1
// Lets assume we have N lines in the text file
// O(N) + O(N)
// ======> Hence Time Complexity :O(N) <======
Graph CreateGraph(Graph g, const char *busStops, const char *Distance, int transpose)
{
    int count = 0;
    Edge *e_ptr;
    Vertex *v1, *v2;
    char buses[MAX][20];
    char line[MAX] = "";
    int a, b, c;
    FILE *fp1 = fopen(busStops, "r");
    FILE *fp2 = fopen(Distance, "r");
    if (fp1 == NULL || fp2 == NULL)
    {
        printf("text files does not exist\n");
        exit(1); // exit the program
    }
    while (fgets(line, sizeof line, fp1) != NULL) // O(N)
    {
        char *name = strtok(line, ":\n");
        name = strtok(NULL, ":\n");
        strcpy(buses[count], name);
        count++;
    }
    while (!feof(fp2)) //O(N)
    {
        fscanf(fp2, "%d-%d:%d\n", &a, &b, &c);
        v1 = (Vertex *)malloc(sizeof(Vertex)); // first vertex
        assert(v1 != NULL);
        v1->busName = buses[a];
        v1->busCode = a;

        v2 = (Vertex *)malloc(sizeof(Vertex)); // second vertex
        assert(v2 != NULL);
        v2->busName = buses[b];
        v2->busCode = b;

        e_ptr = (Edge *)malloc(sizeof(Edge)); // edge
        assert(e_ptr != NULL);
        if (transpose) // inverts the edges to create the transpose graph
        {
            e_ptr->p1 = v2;
            e_ptr->p2 = v1;
        }
        else
        {
            e_ptr->p1 = v1;
            e_ptr->p2 = v2;
        }
        if (!InsertEdge(g, e_ptr)) // log(M+N)
            printf("edge not created\n");
    }

    fclose(fp2);
    fclose(fp1);
    return g;
}

// Lets assume we have N vertices
// traversing each vertex one by one
// ======> Hence Time Complexity :O(N) <======
void ResetGraph(Graph g)
{
    VertexNode *temp = g->vertices;
    while (temp != NULL) // O(N)
    {
        temp->h = NULL;
        temp->visited = 0;
        temp->min_distance = 0;
        temp->prior = NULL;
        temp = temp->next;
    }
}

// Lets assume we have N vertices and M edges
// traversing each vertex and edge
// ======> Hence Time Complexity :O(M+N) <======
void BFS(VertexNode *node, int print)
{
    SimpleQueueNode *qnode = newQueueNode(node);
    SimpleQueueNode *temp;
    SimpleQueueNode *lastnode = qnode;
    SimpleQueue *queue = newQueue(qnode);
    AdjacentNode *adjtrav;

    node->visited = 1;
    while (qnode != NULL)
    {
        adjtrav = qnode->vnode->adj;
        while (adjtrav != NULL)
        {
            if (adjtrav->vnode->visited == 0)
            {
                if (print)
                    printf("%d:%s\n", adjtrav->vnode->v->busCode, adjtrav->vnode->v->busName);
                lastnode->next = newQueueNode(adjtrav->vnode);
                lastnode = lastnode->next;
                adjtrav->vnode->visited = 1;
            }
            adjtrav = adjtrav->next;
        }
        qnode = qnode->next;
    }
    qnode = queue->head;
}

// finds the distance between two vertex by reading the given Distance.txt on fly
// if found returns the distance or returns -1
// Lets assume there are N lines in the given text file
// ======> Hence Time Complexity :O(N) <======
int distance(int busCode1, int busCode2)
{
    int a, b, c;
    FILE *fp = fopen("Distance.txt", "r");
    while (!feof(fp)) // read the file untill the end of file encountered
    {
        fscanf(fp, "%d-%d:%d\n", &a, &b, &c);
        if ((busCode1 == a && busCode2 == b) || (busCode1 == b && busCode2 == a))
        {
            fclose(fp);
            return c;
        }
    }
    fclose(fp);
    return -1;
}

// Lets assume we have N vertices and M edges
// traversing each vertex and edge through breadth-first search
// O(N) + O(N) + O(N+M)
// ======> Hence Time Complexity :O(M+N) <======
int StronglyConnectivity(const char *busStops, const char *BusNames, const char *BusRoutes, const char *Distance)
{
    VertexNode *temp = g->vertices;
    if (!temp)
    {
        printf("No graph exists\n");
        exit(1);
    }
    BFS(g->vertices, 0);
    while (temp != NULL) // O(N)
    {
        if (!temp->visited)
            return 0;
        temp = temp->next;
    }
    VertexNode *temp_rev = r->vertices;
    if (!temp_rev)
    {
        printf("No transpose graph exists\n");
        exit(1);
    }
    BFS(r->vertices, 0);
    while (temp_rev != NULL) // O(N)
    {
        if (!temp_rev->visited)
            return 0;
        temp_rev = temp_rev->next;
    }
    return 1;
}

// Lets assume we have N vertices and M edges
// using kosaraju's algorithm to find maximal strongely connected components
// ======> Hence Time Complexity :O(M+N) <======
void maximalStronglyComponents(const char *busStops, const char *BusNames, const char *BusRoutes, const char *Distance)
{
    int count = 0;
    VertexNode *temp = g->vertices;
    VertexNode *temp_rev = r->vertices;
    if (!temp)
    {
        printf("No graph exists\n");
        exit(1);
    }
    while (temp != NULL)
    {
        if (!temp->visited)
        {
            printf("Strongly connected component %d:\n", ++count);
            printf("%d:%s\n", temp->v->busCode, temp->v->busName);
            BFS(temp, 1); // passing the node from the queue into the transpose graph , as per kosaraju's algo
        }
        temp = temp->next;
    }
}

// Lets assume we have N vertices and M edges
// traversing each vertex and edge through breadth-first search
// ======> Hence Time Complexity :O(M+N) <======
void reachableStops(const char *sourceStop, const char *busStops, const char *BusNames, const char *BusRoutes, const char *Distance)
{
    VertexNode *desired = NULL;
    VertexNode *temp = g->vertices;
    AdjacentNode *adjtemp;
    if (!temp)
    {
        printf("No graph exists\n");
        exit(1);
    }
    while (temp != NULL)
    {
        if (!strcmp(temp->v->busName, sourceStop))
            desired = temp;
        temp = temp->next;
    }
    if (!desired)
    {
        printf("Vertex not in graph\n");
        exit(1);
    }
    BFS(desired, 1); // O(M+N)
}

// Lets assume we have N vertices and M edges
// Implementing Dijkstra's shortest path algorithm using min-heap priority queue
// O(logN) + O(1) + O(logN) * O(M+N)
// ======> Hence Time Complexity :O(logN(M+N)) <======
void TravelRoute(const char *sourceStop, const char *destinationStop, const char *busStops, const char *BusNames, const char *BusRoutes, const char *Distance)
{
    VertexNode *temp = g->vertices, *A, *B;
    AdjacentNode *adjtemp;
    Heap *que = newHeap();
    HeapNode *heaptemp;
    double length;
    char PathBusName[MAX][20];
    int PathBusCode[MAX];
    int ctr = 0;
    // finds A and B by traversing each vertex one by one
    while (temp != NULL)
    {
        if (!strcmp(temp->v->busName, sourceStop))
            A = temp;
        if (!strcmp(temp->v->busName, destinationStop))
            B = temp;
        temp = temp->next;
    }
    // Checks for valid input
    if (!A || !B || (!strcmp(A->v->busName, B->v->busName) && A->v->busCode == B->v->busCode))
    {
        printf("No route exists from %s to %s\n", sourceStop, destinationStop);
        exit(1);
    }
    Insert(que, 0, A); // O(logN)
    // loops through each vertex and expands all edges if vertex hasn't been visisted
    // continually recalculates the minimum value
    while (que->root != NULL)
    {
        heaptemp = RemoveMin(que); // O(logN)
        if (heaptemp->vnode->visited == 0)
        {
            heaptemp->vnode->visited = 1;
            adjtemp = heaptemp->vnode->adj;
            while (adjtemp != NULL)
            {
                length = distance(heaptemp->vnode->v->busCode, adjtemp->vnode->v->busCode); // log(1)
                if (adjtemp->vnode != A)
                {
                    if (adjtemp->vnode->min_distance == 0)
                    {
                        Insert(que, length + heaptemp->vnode->min_distance, adjtemp->vnode); // O(logN)
                        adjtemp->vnode->min_distance = length + heaptemp->vnode->min_distance;
                        adjtemp->vnode->prior = heaptemp->vnode;
                    }
                    else if ((heaptemp->vnode->min_distance + length) < adjtemp->vnode->min_distance)
                    {
                        if (!adjtemp->vnode->h && heaptemp->vnode->min_distance + length < adjtemp->vnode->min_distance)
                            Insert(que, length + adjtemp->vnode->min_distance, adjtemp->vnode); // O(logN)
                        else
                        {
                            adjtemp->vnode->h->key = length + adjtemp->vnode->min_distance;
                            BubbleUp(que, adjtemp->vnode->h);
                            BubbleDown(que, adjtemp->vnode->h);
                            adjtemp->vnode->min_distance = length + heaptemp->vnode->min_distance;
                            adjtemp->vnode->prior = heaptemp->vnode;
                        }
                    }
                }
                adjtemp = adjtemp->next;
            }
        }
        heaptemp->vnode->h = NULL;
    }
    temp = B;
    //small loop to back to original path and print in reverse order
    if (B->prior != NULL)
    {
        while (temp != NULL)
        {
            strcpy(PathBusName[ctr], temp->v->busName);
            PathBusCode[ctr] = temp->v->busCode;
            ctr++;
            temp = temp->prior;
        }
        ctr--;
        for (ctr = ctr; ctr >= 0; ctr--)
            printf("%d:%s\n", PathBusCode[ctr], PathBusName[ctr]);
    }
    else
        printf("No route exists from %s to %s\n", sourceStop, destinationStop);
}

// main function
int main()
{
    g = CreateEmptyGraph();                                // O(1)
    r = CreateEmptyGraph();                                //O (1)
    g = CreateGraph(g, "BusStops.txt", "Distance.txt", 0); // creating a graph, O(N)
    r = CreateGraph(r, "BusStops.txt", "Distance.txt", 1); // creating a transpose graph, O(N)
    printf("<=====================StronglyConnectivity=====================>\n");
    if (StronglyConnectivity("BusStops.txt", "BusNames.txt", "BusRoutes.txt", "Distance.txt"))
        printf("Graph is Strongly Connected\n");
    else
        printf("Graph is not Strongly Connected\n");
    ResetGraph(g);
    ResetGraph(r);

    printf("<===================maximalStronglyComponents==================>\n");
    maximalStronglyComponents("BusStops.txt", "BusNames.txt", "BusRoutes.txt", "Distance.txt");
    ResetGraph(g);

    printf("<========================reachableStops========================>\n");
    reachableStops("Bridge Street", "BusStops.txt", "BusNames.txt", "BusRoutes.txt", "Distance.txt");
    ResetGraph(g);

    printf("<=========================TravelRoute==========================>\n");
    TravelRoute("Cumberland", "Phillip Street", "BusStops.txt", "BusNames.txt", "BusRoutes.txt", "Distance.txt");

    return 0;
}