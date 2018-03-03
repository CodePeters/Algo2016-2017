#include <stdio.h>
#include <stdlib.h>

int main(){
  
  int N,K;  
  scanf("%d%d",&N,&K);
  int Sums[N+1][N],i,j,lower,upper,left,right,lpoint,rpoint;
  char **array = malloc(sizeof(char *) * N);
  long long int count=0LL,sum=0LL;

  for(i=0;i<N;i++) {
      array[i] = malloc(sizeof(char) * (N+1)); 
      scanf("%s",array[i]);
  }

  for(j=0;j<N;j++){
      Sums[0][j]=0;
      for(i=1;i<N+1;i++){
          Sums[i][j]=Sums[i-1][j]+(array[i-1][j]-48); 
      }       
  }

  for(lower=1;lower<N+1;lower++){
      for (upper=lower; upper < N+1; upper++)
         {
           left=right=0;
           sum=0LL;
           sum+=Sums[upper][right]-Sums[lower-1][right];    
                     
           while(1){
            
               if(sum==K){
                  if(right>=N-1 && left>=N-1) {count++;break;}
                  rpoint=lpoint=1;
                  if (right<N-1) {   
                                                    
                    while(right<N-1 ){        
                      right++;
                      if(Sums[upper][right]-Sums[lower-1][right]==0) rpoint++;
                      else {sum+=Sums[upper][right]-Sums[lower-1][right];break;}
                    }

                  }
                  if(left<N-1){

                 
                    while(left<=N-1){ 
                       
                      if(Sums[upper][left]-Sums[lower-1][left]==0) {lpoint++;}  
                      else {sum-=Sums[upper][left]-Sums[lower-1][left];left++;break;}   
                       left++;                                 
                    }
                       
                    
                   }
                  count=count+(lpoint)*(rpoint);
               }
               else if(sum>K){
                   if(left>=N-1) break;
                   sum-=Sums[upper][left]-Sums[lower-1][left];
                   left++;
               }
               else{
                    if(right>=N-1) break;
                    right++;
                    sum+=Sums[upper][right]-Sums[lower-1][right];
               }
           }
      }
  }

  printf("%lld\n",count);
	return 0;
}


   

