/* 
   This is a simple test program for the cc and viterbi functions
   Multisample encoding
   CC pac size = B*N
*/
/*
   Two kinds of encoding output
   v[]    -> bits stored in array
   test[] -> bit-packed form
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define B 16 //quantization bits
#define N 2  //number of samples to be encoded

#include "viterbi.h"

int main(){
    short i,j;
    short x[N]={0xae58,0x25};//sample1, sample2, sample3...
    short test[2*N+1]={0};
    
    short v[(B*N+m)*2],y[B*N];
    short z[N];//stores the bit-packed output, need to be initialized to all zero each time    
    float r[(B*N+m)*2];
      
    table();
      
    /*print from the msb of the last element in x*/
    printf("<- sample3 <- sample2 <- sample1\n");
    
    for (i=N-1;i>=0;i--){
         printf("<-0x");
         for (j=0;j<B;j++){
              printf("%d",(x[i]>>(B-j-1))&mask);
              }
         }
    printf("\n");
    
    cc(x,v,test);
    
    /*printf("\n");
    printf("Encoding sequence\n");
    printf("              ");
    for (i=0;i<(B*N+m)*2;i++){
         if((i-6)%B==0)printf("<-0x");
         printf("%d",v[(B*N+m)*2-i-1]);
         }
    
    printf("\n\n");
    printf("Test encoding sequence\n");
    for (i=2*N;i>=0;i--){
         printf("<-0x");
         for (j=0;j<B;j++){
              printf("%d",(test[i]>>(B-j-1))&mask);
              }
         }
    printf("\n");*/
    
    
    /*idiot channel*/
    for (i=0;i<(B*N+m)*2;i++){
         r[i]=2.0*v[i]-1;
         }

    viterbi(r,y,z);
    
    printf("\n");
    printf("Decoded sequence\n");
    for (i=0;i<B*N;i++){
         if(i%B==0)printf("<-0x");
         printf("%d",y[B*N-i-1]);
         }
    
    printf("\n");
    printf("test z[]\n");
    for (i=N-1;i>=0;i--){
         printf("<-0x");
         for (j=0;j<B;j++){
              printf("%d",(z[i]>>(B-j-1))&mask);
              }
         }
    printf("\n");
    
    getchar();
    }
