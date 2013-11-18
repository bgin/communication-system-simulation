/* This is a simulation of the power allocation of ECC
 * Date : 2007/3/6
 */
#define k 6                          //pac length
#define h 0                          //parity bits

#define start 0                      //starting SNR value
#define end 5                        //ending SNR value

#define ERROR 327680                    //number of accumulated packet errors 
#define step 1                       //spacing of SNR values

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include"gaussian.h"
#include"transmitter.h"
#include"channel.h"
#include"decoder.h"

int main(){
    int i,j=0,t,temp,temp1;
    int u[k],v[k+h],y[k],bit_error,packet_error;
    int points=(int)((float)(end-start)/step) + 1;
    float r[k+h];
    float *snr,*snr_db,*ber;
    int s,min,hour;
    FILE	*fp_output;
    char data[20]={"data.txt"};
    long temptime1,temptime2;
    
    snr=(float*)malloc(points*sizeof(float));
    snr_db=(float*)malloc(points*sizeof(float));
    ber=(float*)malloc(points*sizeof(float));
    srand(time(&temptime1)%60);//seed
    fp_output=fopen(data,"w");
      
    ber[-1]=1;
    
    for (i=0;i<points;i++){//sample SNR value
        snr_db[i]=start+i*step;
        snr[i]=pow(10,snr_db[i]/10);  
    }                                                
    
    //for (i=0;i<points;i++){
    for (i=0;i<points && ber[i-1]>1e-5 ;i++){//simulate till BER ~= 1e-5   
        printf("��%2d��\n",i);
        bit_error=0;  
        packet_error=0;
      
        for (t=0;packet_error<ERROR;t++){
        //for (t=0;t<repetition;t++){  
         
            
            encode(u,v);
            
            awgn(v,snr[i],r);
            
            decode(r,y);
        
            temp = bit_error;
            
            j=0;
            while (j<k){
                  if (u[j]!=y[j]) bit_error++;
                  j++;
            }
        
            if(bit_error > temp) {
              packet_error++;
              //printf("packet error = %d\n",packet_error);
            }
        
        }
     
        ber[i]=bit_error/(float)(k*t);
        
        printf("SNR = %f\n",snr_db[i]);
        fprintf(fp_output,"SNR = %f\n",snr_db[i]);
        printf("BER = %e\n",ber[i]); 
        fprintf(fp_output,"BER = %e\n",ber[i]);
      
        temp1=i+1;
    }
    
    time(&temptime2);//calculate execution time
    s=temptime2-temptime1;
    hour=s/3600;
    s=s%3600;
    min=s/60;
    s=s%60;
    
    fprintf(fp_output,"--------------------------\n");
    printf("--------------------------\n");
    
    for (i=0;i<temp1;i++){// print SNR & BER
        fprintf(fp_output,"%f\n",snr_db[i]);
        printf("%f\n",snr_db[i]);
    }
    
    fprintf(fp_output,"--------------------------\n");
    printf("--------------------------\n");
    for (i=0;i<temp1;i++){
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
