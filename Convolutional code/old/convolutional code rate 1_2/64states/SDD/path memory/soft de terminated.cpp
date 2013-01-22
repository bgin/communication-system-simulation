/* terminated , soft decision   path memory    ex*/

#define repetition 1000
#define k 1000                             //length of input bit sequence
#define m 6                                //memory order = 6
#define st_num (int)pow(2,m)
#define k_m (k+m)
#define path 32

#define lower 0//SNR                       //lower bound of SNR sampling value
#define upper 9.5//SNR                     //upper bound of SNR sampling value

#define start 0                            //there are 20 sample points         
#define end 1                             //loop runs for i = start ~ end-1 

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"gaussian.h"
#include"transmitter.h"
#include"receiver_3.h"

int main(){                                //polar NRZ , A = +- 0.1   T = 20^-3.
    
    int i,j=0,t,temp;
    int *u,*v,*y,bit_error,packet_error;
    int s,min,hour;
    float snr[20],snr_db[20],ber[20];
    FILE	*fp_output;
    char data[20]={"data.txt"};
    
    long temptime1,temptime2,temptime3,temptime4,temptime5;                           //using system time to choose a seed
    srand(time(&temptime1)%60);
    
    
    u=(int *) malloc((k+m)*sizeof(int));
    v=(int *) malloc((2*(k+m))*sizeof(int));
    y=(int *) malloc((k+m)*sizeof(int));
    for(i=0;i<k;i++){
          y[i]=-1;
          }
    
    fp_output=fopen(data,"w");

    for (i=start;i<end;i++){                                //sample SNR value
        snr_db[i]=lower+(float(upper-lower)/19)*i;
        snr[i]=pow(10,snr_db[i]/10); 
        }

    table();
    
    for (i=start;i<end;i++){                  //�D�j�� 
        
      printf("��%2d��\n",i);
      bit_error=0;  
      packet_error=0;
      
      time(&temptime4);
      //for (t=0;bit_error<100;t++){          //3 �� 1  ���� 
      for (t=0;packet_error<100;t++){
      //for (t=0;t<repetition;t++){ 

        input(u,v);
        output(v,snr[i],y);
        
        temp = bit_error;
        
        j=0;
        while (j<k){
              if (u[j]!=y[j])  bit_error++;
              j++;
              }
       
        if(bit_error > temp) {
           packet_error++;
           printf("packet error = %d\n",packet_error);
           }
        
       //printf("t = %d\n",t);
        
        }
      time(&temptime5);
      
      ber[i]=bit_error/(float)(k*t);
        
      printf("SNR = %f\n",snr_db[i]);
      fprintf(fp_output,"SNR = %f\n",snr_db[i]);
      printf("BER = %e\n",ber[i]); 
      fprintf(fp_output,"BER = %e\n",ber[i]);
      
      /*s=temptime5-temptime4;
      hour=s/3600;
      s=s%3600;
      min=s/60;
      s=s%60;
      printf("...%2d�p�� %2d�� %2d��",hour,min,s); 
      
      s=temptime5-temptime1;
      hour=s/3600;
      s=s%3600;
      min=s/60;
      s=s%60;
      printf("...%2d�p�� %2d�� %2d��\n",hour,min,s); */
      }
    free(u);
    //free(v);
    //free(y);                                     // will cause crash?? 
    
    time(&temptime2);                            //calculate execution time
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
    
    fprintf(fp_output,"�A�ΤF%2d�p�� %2d�� %2d��\n",hour,min,s);
    printf("�A�ΤF%2d�p�� %2d�� %2d��\n",hour,min,s);
    
    fclose(fp_output);
    
    system("pause");
    }