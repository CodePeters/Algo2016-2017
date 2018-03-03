#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#define MAX(a,b) (((a)>(b))?(a):(b))

struct AdjListNode{
    int dest;
    int weight;
    struct AdjListNode* next;
};
 
struct AdjList{
    struct AdjListNode *head;  
};
 
struct Graph{
    int V;
    struct AdjList* array;
};
 
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    for (int i = 0; i < V; ++i) graph->array[i].head = NULL;
    return graph;
}
 
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

struct MinHeapNode{
    int  v;
    int dist;
};
 
struct MinHeap{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =(struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist ) smallest = left;
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist) smallest = right;
    if (smallest != idx)
    {
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap* minHeap){
    return minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap)) return NULL;
    struct MinHeapNode* root = minHeap->array[0];
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size) return 1;
   return 0;
}

void dijkstra(struct Graph* graph, int src,int T,int time[],int B,int acc[],int K,int C[])
{
    int V = graph->V;// Get the number of vertices in graph
    long long int dist[V];      // dist values used to pick minimum weight edge in cut
    struct MinHeap* minHeap = createMinHeap(V);
    for (int v = 0; v < V; ++v)
    {
        if(v!=src){dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;}
    }
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = T;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;
    int idx=0;
    while (!isEmpty(minHeap))
    {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number

        struct AdjListNode* pCrawl = graph->array[u].head;

         while (idx < K-2 && time[C[idx]] < dist[u]) idx++;
            idx--;
            int first = C [idx], second = C [idx + 1];

           // if(u==1) printf("%d %d\n",first,second );

        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX )                              
            {
               // int d=pCrawl->weight + dist[u];

                int wait;
                if ((first == v && second == u) || (second == v && first == u)) {
                    wait = MAX(0, time[C[idx+1]] - dist[u]);
                    
                } else wait = 0;


                if(pCrawl->weight + dist[u]+wait < dist[v]){
                  dist[v] = pCrawl->weight + dist[u]+wait;

                 decreaseKey(minHeap, v, dist[v]);
                }
            }
            pCrawl = pCrawl->next;
        }
        if(u==B) break;
    }
     for(int i=0;i<V;i++){dist[i]-=T;} 
  printf("%lld\n",dist[B]);
}

int main()
{
    int n, m;
    int A,B,T,K;
    scanf("%d%d",&n,&m);
    scanf("%d%d%d%d",&A,&B,&T,&K);
    A--;
    B--;
    int acc[n];
    int time[n];
    memset(acc,-1,sizeof(acc));
    struct Graph* graph = createGraph(n); 
    int *C = (int*)malloc(K*sizeof(int));
    for(int i=0;i<K;i++){scanf("%d",&C[i]);C[i]-=1;acc[C[i]]=i;}
      //  printf("%d\n",C[2] );
    memset(time,-1,sizeof(time));
    time[C[0]]=0;

    for (int i = 0; i < m; i++){

        int a, b, c; 
        scanf("%d%d%d",&a,&b,&c);
        if(acc[a-1]!=-1 && acc[b-1] != -1 && (acc[a-1]==acc[b-1]+1 || acc[a-1]==acc[b-1]-1) ){
            if(acc[a-1] > acc[b-1]) time[a-1]=c;
            else time[b-1]=c;
        }
        addEdge(graph, a-1, b-1, c);
    }

    for(int i=1;i<K;i++){time[C[i]]+=time[C[i-1]];}
    dijkstra(graph,A,T,time,B,acc,K,C);
    return 0;
}