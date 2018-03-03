#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAX(a,b) (((a)>(b))?(a):(b))
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

void writeInt (int n)
{
    int N = n, rev;
    int  count = 0;
    rev = N;
    if (N == 0) { putchar_unlocked('0'); putchar_unlocked('\n'); return ;}
    while ((rev % 10) == 0) { count++; rev /= 10;}
    rev = 0;
    while (N != 0) { rev = (rev<<3) + (rev<<1) + N % 10; N /= 10;}
    while (rev != 0) { putchar_unlocked(rev % 10 + '0'); rev /= 10;}
    while (count--) putchar_unlocked('0');
    putchar_unlocked('\n');
}

struct edge{
	int u,v,cost;
};

struct node{
	int child;
	int cost;
	struct node *next;
};

typedef struct node* nodeptr;
nodeptr *Neighboors;
int anc[30005][16], maxcost[30005][16];
struct subset *subsets;
struct edge *Edges; 
int *current;
int *L;
int *fa;

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
 
    if (subsets[i].parent != i) subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}
 
void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
 
    if (subsets[xroot].rank < subsets[yroot].rank)subsets[xroot].parent = yroot;
    else if(subsets[xroot].rank > subsets[yroot].rank) subsets[yroot].parent = xroot;
    else{
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

void insert(int x,int y,int cost)
{
      	if(Neighboors[x]==NULL){
        		Neighboors[x]=(nodeptr)malloc(sizeof(struct node));
        		Neighboors[x]->child=y;
        		Neighboors[x]->cost=cost;
        		Neighboors[x]->next=NULL;
        }
        else{
            nodeptr new=(nodeptr)malloc(sizeof(struct node));
            new->child=y;
            new->cost=cost;
            new->next=Neighboors[x];
            Neighboors[x]=new;
        }
}

void MST(int V,int M)
{    
    int e = 0,i = 0;
 
    qsort(Edges, M, sizeof(Edges[0]), Compfunc);
    for (int v = 0; v < V; ++v){

        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
 
    while (e < V - 1){
        struct edge next_edge = Edges[i++];
        int x = find(subsets, next_edge.u);
        int y = find(subsets, next_edge.v);
 
        if (x != y){
        	  insert(next_edge.u,next_edge.v,next_edge.cost);
        	  insert(next_edge.v,next_edge.u,next_edge.cost);
            e++;
            Union(subsets, x, y);
        }
     }
     
}

void DFS(int u, int f)
{
    fa[u] = f;
    nodeptr new=Neighboors[u];
    while(new!=NULL){
        if((new->child)!=f) {
          current[new->child]=new->cost; 
          L[new->child] = L[u] + 1;
          DFS(new->child,u);
        }
        new=new->next;     
    }
}

void progress(int V)
{
    for(int i=0; i<V; i++)
    {
        anc[i][0] = fa[i], maxcost[i][0] = current[i];
        for(int j=1; (1 << j) < V; j++) anc[i][j] = -1;
    }

    for(int j=1; (1 << j) < V; j++) 
      for(int i=0; i<V; i++)
       if(anc[i][j-1] != -1)
       {
          int a = anc[i][j-1];
          anc[i][j] = anc[a][j-1];
          maxcost[i][j] = MAX(maxcost[i][j-1], maxcost[a][j-1]);
       }
}

int query(int p, int q)
{
    int lo,tmp;

    if (L[p] < L[q]) {tmp = p; p = q; q = tmp;}

    for(lo = 1; (1 << lo) <= L[p]; lo++);
    lo--;

    int ans = -1e9;
    for(int i=lo; i>=0; i--)
        if(L[p] - (1<<i) >= L[q]) ans = MAX(ans, maxcost[p][i]), p = anc[p][i];

    if(p == q) return ans;

    for(int i=lo; i>=0; i--)
       if(anc[p][i] != -1 && anc[p][i] != anc[q][i])
       {
           ans = MAX(ans, maxcost[p][i]);
           p = anc[p][i];
           ans = MAX(ans, maxcost[q][i]);
           q = anc[q][i];
       }

    ans = MAX(ans, current[p]);
    ans = MAX(ans, current[q]);
    return ans;
}

int main(){
 	
  int M,N,i;
  scanf("%d%d",&N,&M);
  Edges = (struct edge*)malloc((M)*sizeof(struct edge));
  subsets=(struct subset*)malloc(N * sizeof(struct subset));
  Neighboors = (nodeptr *)malloc(N*sizeof(nodeptr));
  current = (int*)malloc(N * sizeof(int));
  L = (int*)malloc(N * sizeof(int));
  fa =(int*)malloc(N * sizeof(int));

  for(int i=0;i<M;i++){
    	Edges[i].u=readLong()-1;
    	Edges[i].v=readLong()-1;
	    Edges[i].cost=readLong();	
  }

  for(int i=0;i<N;i++) Neighboors[i]=NULL;
  MST(N,M);

  free(Edges);
  free(subsets);

  L[0] = current[0] = 0;
  
  DFS(0,-1);
  progress(N);

  int p, q,Q=readLong();
  while(Q--){
        p=readLong()-1;
        q=readLong()-1;
        writeInt(query(p, q));
  }
  free(current);
  free(fa);
  free(L);
  for(i=0; i<N; i++){
    nodeptr head=Neighboors[i];
    nodeptr curr;
    while ((curr = head) != NULL) { 
      head = head->next;          
      free (curr);                
    }
  }
  free(Neighboors);
  
  return 0;
}