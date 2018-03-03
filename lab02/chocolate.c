#include <stdio.h>  
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))  
  
int main(){   
  int N,K;   
  scanf("%d%d",&N,&K);  
  int array[N],Gain[2][N],maxprev,i,j;  
  for(i=0;i<N;i++) { scanf("%d",&array[i]); Gain[0][i] = 0; }  
  Gain[1][0]=0;  
  for(i=1;i<K+1;i++){    
        maxprev =  Gain[0][0] - array[0];;      
        for(j=1; j < N; j++){  
            maxprev = MAX(maxprev,Gain[0][j-1] - array[j-1]);  
            Gain[1][j] = MAX(Gain[1][j-1],array[j] + maxprev);  
        }  
        for(j=1;j<N;j++) Gain[0][j]=Gain[1][j];  
  }  
  printf("%d\n",Gain[1][N-1]);  
  return 0;  
}