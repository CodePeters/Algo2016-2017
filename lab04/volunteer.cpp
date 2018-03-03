#include <stdio.h>
#include <vector>
#include <queue>
#define BSIZE 1<<15
#define N  1100000
using namespace std;
vector<pair<int, int> > S;
vector<int> g[N], gr[N] ,G[N], order;
queue<int> qu;
int cost[N],TYPE[N],used[N],scc_type[N];
long long COST[N],dp[N];
char buffer[BSIZE];
long bpos = 0L, bsize = 0L;
int n, m, C, a, b;

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

void DFS(int x){
    TYPE[x] = 1;
    int cos = scc_type[x];
    for (int j = 0; j < G[x].size(); j++){
         if (TYPE[ G[x][j] ] == 0)  DFS( G[x][j] );
         cos = max( cost[ G[x][j] ], cos ); 
    }
    cost[x] = cos;
} 
 
int main(){

    n = readLong(),m = readLong();
    for (int i = 1; i <= m+n; i++){
        if(i<=n){TYPE[i] = readLong(); cost[i] = readLong(); continue;}
        a = readLong(),b = readLong();
        g[a].push_back(b);
        gr[b].push_back(a);
    }
    for (int i = 1; i <= n; i++){
        if (used[i]) continue;
        //used[i] = 1;
        S.push_back(make_pair(i, 0));
        while (S.size()){
            int qv = S.back().first;
            int qptr = S.back().second;
            S.pop_back();
            used[qv] = 1;
            if (qptr == g[qv].size()){ order.push_back(qv); continue;}
            S.push_back(make_pair(qv, qptr + 1));
            int to = g[qv][qptr];
            if (!used[to]) S.push_back(make_pair(to, 0));
        }
    }
 
    reverse(order.begin(), order.end());
    memset(used,0,sizeof(used));
    for (int i = 0; i < order.size(); i++)
    {
        int id = order[i];
        if (used[id]) continue;
        ++C;
        qu.push(id);
        while (qu.size())
        {
            int v = qu.front();
            used[v] = C;
            COST[C] += cost[v];
            scc_type[C]+=TYPE[v];
            qu.pop();
            for (int j = 0; j < gr[v].size(); j++){
                int to = gr[v][j];
                if (!used[to]) { used[to] = C; qu.push(to);}
            }
        }
    }
 
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < g[i].size(); j++){
            int to = g[i][j];
            if (used[i] == used[to]) continue;
            G[used[i]].push_back(used[to]);
        }

    int v1 = used[1];
    memset(TYPE,0,sizeof(TYPE));
    DFS(v1);
    for (int i = C; i; --i) if (scc_type[i]>0)dp[i] = COST[i]; else dp[i]=0 ;
    for (int i = C; i; --i)     
        for (int j = 0; j < G[i].size(); j++){
            int to = G[i][j];
            if(scc_type[i] == 0 ) if(cost[i] == 0 ) continue;
            dp[i] = max(dp[i], dp[to] + COST[i]);
        }

    if (cost[v1]==0 && scc_type[v1]==0) dp[v1] = -1;
    printf("%lld\n",dp[v1]);
    return 0;
}