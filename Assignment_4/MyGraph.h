// Author: Mohit Khanna
// Student ID: z5266543
// Platform: Mac

typedef struct Vertex
{
    int busCode;
    char *busName;
} Vertex;

// each edge is a pair of vertices (end-points)
typedef struct Edge
{
    Vertex *p1; // first end point
    Vertex *p2; // second end point
} Edge;

typedef struct AdjacentNode
{
    struct VertexNode *vnode;
    struct AdjacentNode *next;
    int visited;
} AdjacentNode;

// A vertex node stores a vertex and other information
typedef struct VertexNode
{
    struct Vertex *v;
    struct AdjacentNode *adj;
    struct AdjacentNode *lastadj;
    struct VertexNode *next;
    struct HeapNode *h;
    int visited;
    double min_distance;
    struct VertexNode *prior;
} VertexNode;

typedef struct GraphRep *Graph;

// graph header
typedef struct GraphRep
{
    VertexNode *vertices; // pointer to head of vertices
    VertexNode *lastvert; // pointer to last vertex
    int nV;               // #vertices
    int nE;               // #edges
} GraphRep;

typedef struct SimpleQueue
{
    struct SimpleQueueNode *head;
} SimpleQueue;

// arranges the vertices into a simple queue
typedef struct SimpleQueueNode
{
    struct VertexNode *vnode;
    struct SimpleQueueNode *next;
} SimpleQueueNode;

// creates a new vertex
Vertex *newVertex(int busCode, char *busName)
{
    struct Vertex *v = malloc(sizeof(struct Vertex));
    assert(v != NULL);
    v->busCode = busCode;
    v->busName = busName;
    return v;
}

// creates a new adjacent node
AdjacentNode *newAdjacentNode(VertexNode *vert)
{
    struct AdjacentNode *node = malloc(sizeof(struct AdjacentNode));
    assert(node != NULL);
    node->vnode = vert;
    node->next = NULL;
    node->visited = 0;
    return node;
}

// creates a new edge between two vertices
Edge *newEdge(Vertex *v1, Vertex *v2)
{
    struct Edge *e = malloc(sizeof(struct Edge));
    assert(e != NULL);
    e->p1 = v1;
    e->p2 = v2;
    return e;
}

// creates a new vertex node
VertexNode *newVertexNode(Vertex *vert)
{
    struct VertexNode *newNode = malloc(sizeof(struct VertexNode));
    assert(newNode != NULL);
    newNode->v = vert;
    newNode->next = NULL;
    newNode->adj = NULL;
    newNode->lastadj = NULL;
    newNode->visited = 0;
    newNode->h = NULL;
    newNode->min_distance = 0;
    newNode->prior = NULL;
    return newNode;
}

// creates a new empty graph
Graph CreateEmptyGraph()
{
    Graph g = malloc(sizeof(GraphRep));
    assert(g != NULL);
    g->vertices = NULL;
    g->lastvert = NULL;
    g->nV = 0;
    g->nE = 0;
    return g;
}

//creates a new queue node
SimpleQueueNode *newQueueNode(VertexNode *vert)
{
    struct SimpleQueueNode *qnode = malloc(sizeof(struct SimpleQueueNode));
    assert(qnode != NULL);
    qnode->vnode = vert;
    return qnode;
}

// creates a new queue and returns the first node
SimpleQueue *newQueue(SimpleQueueNode *node)
{
    struct SimpleQueue *q = malloc(sizeof(struct SimpleQueue));
    assert(q != NULL);
    q->head = node;
    return q;
}

// Checks whether the vertex is in graph g
// if not returns NULL
VertexNode *CheckVertexInGraph(Graph g, Vertex *vert)
{
    VertexNode *node = g->vertices;
    if (node)
    {
        while (node != NULL)
        {
            if (node->v->busCode == vert->busCode && !strcmp(node->v->busName, vert->busName))
                return node;
            node = node->next;
        }
    }
    return NULL;
}

// Checks whether the vertex is the adjacent list of a vertex node
// if not returns NULL
AdjacentNode *CheckVertexInAdj(VertexNode *original, Vertex *vert)
{
    AdjacentNode *node = original->adj;
    if (node)
    {
        while (node != NULL)
        {
            if (node->vnode->v->busCode == vert->busCode && !strcmp(node->vnode->v->busName, vert->busName))
                return node;
            node = node->next;
        }
    }
    return NULL;
}