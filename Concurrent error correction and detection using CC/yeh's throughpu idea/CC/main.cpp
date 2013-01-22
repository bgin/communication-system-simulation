/*msb ~~ lsb*/
/*convolutional code rate 1/2*/
#define G11 0133
#define G12 0171

#define m 6                                  //memory order = m
#define st_num 64                            //number of states = 2^m

#define N 5                                 //partition size
#define FRAME 100                             //frames to send
//frame�j�p�� N*k 

#define k 500                               //pac size
#define pacERROR 100                         //accumulated pac errors

#define lower 1.5//SNR                         //lower bound of SNR sampling value
#define upper 11//SNR                         //upper bound of SNR sampling value

#define start 0                              //there are 20 sample points         
#define end 10                                //loop runs for i = start ~ end-1  

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"declaration.h"
#include"gaussian.h"
#include"transmitter.h"
#include"receiver_2.h"

int main(){
    
    int i,j=0,t,temp,n,frame,fail=0;
    int u[(k+m)],v[2*(k+m)],y[(k+m)],bit_error,packet_error;
    int s,min,hour;
    float snr[20],snr_db[20],ber[20],T[20];
    FILE	*fp_output;
    char data[20]={"data.txt"};
    declaration();
    
    long temptime1,temptime2,temptime3,temptime4,temptime5;
    srand(time(&temptime1)%60);                             //seed
 
    fp_output=fopen(data,"w");

    for (i=start;i<end;i++){                                //sample SNR value
        snr_db[i]=lower+(float(upper-lower)/19)*i;
        snr[i]=pow(10,snr_db[i]/10);  
        }
    
    for (i=start;i<end;i++){
        
      printf("��%2d�� SNR=%.2f\n",i,snr_db[i]);
      bit_error=0;  
      packet_error=0;
      frame=0;
      n=0;
      
      //for (t=0;packet_error<pacERROR;t++){
      for (t=0;frame<FRAME;t++){
        
        input(u,v);                          //generate encoded sequece
        output(v,snr[i],y);                  //generate output under a SNR
        
        temp = bit_error;
        
        j=0;
        while (j<k){
              if (u[j]!=y[j]) bit_error++;
              j++;
              }
        
        if  (bit_error > temp) {
            packet_error++;
            fail=1;
            //printf("packet error = %d\n",packet_error);
            }
        
        n++;
        if  (n==N){
            
            if  (fail==1){
                fail=0; 
                }
        
            else{
                 frame++;
                 printf("frame transmitted = %d\n",frame);
                 }
            n=0;
            }
        }
      
      ber[i]=bit_error/(float)(k*t);
      T[i]=N*FRAME/(float)t;//t���ǰe�ݶǰe�L��subpac�� 
      
      printf("SNR = %f\n",snr_db[i]);
      fprintf(fp_output,"SNR = %f\n",snr_db[i]);
      printf("BER = %e\n",ber[i]); 
      fprintf(fp_output,"BER = %e\n",ber[i]);
      printf("THR = %f\n",T[i]); 
      fprintf(fp_output,"THR = %f\n",T[i]);
      }

    time(&temptime2);                              //calculate execution time
    s=temptime2-temptime1;
    hour=s/3600;
    s=s%3600;
    min=s/60;
    s=s%60;
    
    fprintf(fp_output,"--------------------------\n");
    printf("--------------------------\n");
    
    for (i=start;i<end;i++){                          // print SNR & BER
        fprintf(fp_output,"%f\n",snr_db[i]);
        printf("%f\n",snr_db[i]);
        }
    
    fprintf(fp_output,"--------------------------\n");
    printf("--------------------------\n");
    for (i=start;i<end;i++){
        fprintf(fp_output,"%e\n",ber[i]);
        printf("%e\n",ber[i]);
        }
    
    fprintf(fp_output,"--------------------------\n");
    printf("--------------------------\n");
    for (i=start;i<end;i++){
        fprintf(fp_output,"%f\n",T[i]);
        printf("%f\n",T[i]);
        }
    
    fprintf(fp_output,"--------------------------\n");
    printf("--------------------------\n");
    
    fprintf(fp_output,"�A�ΤF%2d�p�� %2d�� %2d��\n",hour,min,s);
    printf("�A�ΤF%2d�p�� %2d�� %2d��\n",hour,min,s);
    
    fclose(fp_output);
    
    system("pause");
    }
