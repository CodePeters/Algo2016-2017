#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15

char buffer[BSIZE];
int N,K;
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
		else if (d == 1) return x;
	}
	return -1;
}

void compute(int** dp,int** Sum,int l,int r,int optl,int optr)
{
	if (l > r) return;  
	int opt, m = (l + r) / 2;
	dp[1][m] = 1000000000;
	for (int i = optl; i <=m && i<=optr; i++) {
		int c=(Sum[m][m] - Sum[m][i - 1] - Sum[i - 1][m] + Sum[i - 1][i - 1]) / 2;
		if (dp[1][m] > dp[0][i-1] + c) {
			dp[1][m] = dp[0][i-1] + c;
			opt = i;
		}
	}
	
	compute(dp,Sum,l, m - 1, optl,opt);
	compute(dp,Sum,m + 1, r, opt,optr);
}

int main(){ 

	N=(int)readLong();
	K=(int)readLong();

	int **dp=(int**) malloc(2*sizeof(int*));
	dp[0]= (int*)malloc((N+1)*sizeof(int));
	dp[1]= (int*)malloc((N+1)*sizeof(int)); 

	int **Sum=(int**) malloc((N+1)*sizeof(int*));
	for(int i=0;i<N+1;i++) Sum[i]=(int*)malloc((N+1)*sizeof(int));

	for(int i=0;i<N+1;i++) {Sum[i][0]=Sum[0][i]=0;}      

	for(int i=1;i<N+1;i++){
       Sum[i][i] = Sum[i][i - 1] + Sum[i - 1][i] - Sum[i - 1][i - 1];  
  	  for(int j=i+1;j<N+1;j++){
         Sum[i][j] = (int)readLong() + Sum[i][j - 1] + Sum[i - 1][j] - Sum[i - 1][j - 1];
         Sum[j][i] = Sum[i][j];
       }     
    }   

    dp[0][0]=0;
	for(int i=1;i<N+1;i++) { dp[0][i]=(Sum[i][i] - Sum[i][0] - Sum[0][i] + Sum[0][0]) / 2;}
	
	int output=dp[0][N];

	for (int i = 2; i <K+1; i++){
	    compute(dp,Sum,i,N,i,N);
		for(int j=0;j<N+1;j++) dp[0][j]=dp[1][j];
		output=dp[1][N];	
	}

	printf("%d\n", output);

	free(dp[0]);
	free(dp[1]);
	free(dp);
	for(int i=0;i<N+1;i++) free(Sum[i]);
	free(Sum);

	return 0; 
}