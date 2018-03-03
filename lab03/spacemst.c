#include <stdio.h>
#include <stdlib.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define BSIZE 1<<15

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;

long readLong() 
{
	long d = 0L, x = 0L;
	char c;

	while (1)  {
		if (bpos >= bsize) {
			bpos = 0;
			if (feof(stdin)) return x;
			bsize = fread(buffer, 1, BSIZE, stdin);
		}
		c = buffer[bpos++];
		if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
		else if (d == 1) return (int)x;
	}
	return -1;
}

struct edge{
	int u,v,cost;
};

int Compfunc(const void* a, const void* b)
{
    return (((struct edge*)a)->cost - ((struct edge*)b)->cost);
}

struct subset
{
    int parent;
    int rank;
};
 
int find(struct subset subsets[], int i)
{
 
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
 
    return subsets[i].parent;
}
 
void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
 
    if (subsets[xroot].rank < subsets[yroot].rank)subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank) subsets[yroot].parent = xroot;
    else{
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int MST(int V,int M,struct edge* Edge)
{
    int e = 0;  
    int i = 0;
    int total_cost=0;  
 
    qsort(Edge, M, sizeof(Edge[0]), Compfunc);

    struct subset *subsets=(struct subset*)malloc(V * sizeof(struct subset));
 
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
 
    while (e < V - 1){

        struct edge next_edge = Edge[i++];
 
        int x = find(subsets, next_edge.u);
        int y = find(subsets, next_edge.v);
 
        if (x != y)
        {
            e++;
            total_cost+=next_edge.cost;
            Union(subsets, x, y);
        }
    }
    free(subsets);
    return total_cost;
}

int main(){

int M,K,N;
scanf("%d%d%d",&N,&K,&M);
struct edge * Edges= (struct edge*)malloc((M+K)*sizeof(struct edge));

for(int i=M;i<M+K;i++){
	Edges[i].u=N;
	Edges[i].v=readLong()-1;
	Edges[i].cost=readLong();
}

for(int i=0;i<M;i++){
	Edges[i].u=readLong()-1;
	Edges[i].v=readLong()-1;
	Edges[i].cost=readLong();
}

int total_cost=MST(N,M,Edges);
total_cost=MIN(total_cost,MST(N+1,M+K,Edges));
printf("%d\n",total_cost);
free(Edges);
return 0;
}