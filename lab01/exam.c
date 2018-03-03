#include <stdio.h>

int main(){
   
  int N,K,extra_time=0,position=0;
  long long int Time,Time2,upper=0,lower=0,count,count2,final_count2=0;
 
   scanf("%d%d",&N,&K);
   int Tutors[N];

  for(int i=0;i<N;i++) { scanf ("%d", &Tutors[i]);if( Tutors[i] > upper) upper= Tutors[i];}
  upper= upper*K;

  while(lower<=upper){    
	    Time = (upper + lower)/2;
      count =count2=0;
	    for(int i=0; i<N; i++){                  
           count=count+ (1+Time/Tutors[i]);
           count2=count2+ (1 + (Time-1)/Tutors[i]);
       }
      if(count>=K) { Time2=Time; final_count2=count2; upper=Time-1; }
      else {lower=Time+1; } 
  }  
  count=K-final_count2;
  for(int i=0; i<N; i++){                  
          if( (Time2%Tutors[i]==0)) { if(count==0) break;count--; extra_time=Tutors[i];position=i;}
    }

  printf("%d %lld\n",position+1,Time2+extra_time);
  return 0;
}