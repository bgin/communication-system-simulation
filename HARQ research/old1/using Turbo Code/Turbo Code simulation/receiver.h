void process1(int,float *,float);
void process2(void);
void puncture(float *,float *);

void siso(int,float *,int,float);
void decode(int *);

int output(float *r,float snr,int *r_d)
{
    
   float N0=3.0/snr;//SNR = (3*Eb)/N0

   int j=0;
   float x[k+m],y1[k+m],y2[k+m];
   while (j<k+m)
   {
      x[j] =r[3*j];
      y1[j]=r[3*j+1];
      y2[j]=r[3*j+2];                        
      j++;
   }
  
//    puncture(y1,y2);

   /*-----------------decoding--------------------*/ 
    
   for (int i=0;i<iteration;i++)
   {   
      process1(i,x,N0);
       
      siso(i,y1,1,N0);
      
      process2();
      
      Minterleave(LLR);

      siso(i,y2,2,N0);//will produce feed[][]

      deinterleave(z);
   }
   deinterleave(LLR);
    
   decode(r_d);

   return 1;
}

void puncture(float *y1,float *y2)
{
   for (int i=0;i<k+m;i++)
   {
      if((i&1) == 0) { y1[i]=0; }
      else { y2[i]=0; }
   }
}

/*float max(float *a,int l){
      float temp;
      int i;
      
      temp=a[0];
      for(i=0;i<l;i++){
           if(a[i]>temp) temp=a[i];
           }
      
      return temp;
      }*/

void process1(int pass,float *x,float N0)
{
   if(pass == 0)
   {
      for (int i=0;i<k+m;i++)
      {
         x_metric[i]=4*x[i]/N0;
            
         LLR[i]=x_metric[i];
            
         z[i]=0;//z initialize
      }
   }
     
   else
   {
      for (int i=0;i<k+m;i++)
      {
         LLR[i]=x_metric[i];
      }
   }
}
     
void process2(void)
{     
   for (int i=0;i<k;i++)
   {
      LLR[i]+=x_metric[i];
   }
   for (int i=k;i<k+m;i++)
   {
      LLR[i]=0;
   }
   //z�k0 
   for (int i=0;i<k+m;i++)
   {
      z[i]=0;
   }
}

void siso(int pass,float *y,int option,float N0){
   float temp[2],t0[st_num],t1[st_num];
   float pI_sym[k+m][4][2];
   float A[((k+m)+1)][st_num];
   float B[((k+m)+1)][st_num];
     
   for (int j=0;j<st_num;j++){
       A[0][j]=-100000;
       B[k+m][j]=-100000;
       }
     
   A[0][0]=0;
   B[k+m][0]=0;
     
   //branch metric Pk[c(e);I]        
   for (int j=0;j<k+m;j++)
   {     
      for (int h=0;h<4;h++)
      {
         float b0=2*(h>>1)-1;
         float b1=2*(h&1)-1;
              
         //1 for complete path metric ,0 for extrisic compute
         pI_sym[j][h][1]= b0*z[j]/2 + b0*LLR[j]/2 + b1*y[j]*2/N0;
         pI_sym[j][h][0]= b1*y[j]*2/N0;
      }
   }

   //forward recursion A
   for (int j=1;j<k+m+1;j++){
      for (int l=0;l<st_num;l++){
       for (int h=0;h<2;h++){//for input 0 , 1
             
           int out1=from_out_table[l][0+2*h];
           int out2=from_out_table[l][1+2*h];
           int temp1=2*out1+out2;
             
           float test=pI_sym[j-1][temp1][1];
           temp[h]=A[j-1][from_table[l][h]]+test;
           }
        A[j][l]=logsum(temp[0],temp[1]);
        //A[j][l]=max(temp,2);
        }
     } 
       
   //backward recursion B
   for (int j=k+m-1;j>=0;j--)
   {
      for (int l=0;l<st_num;l++)
      {
         //for input 0 , 1
         for (int h=0;h<2;h++)
         {
            int out1=out_table[l][0+2*h];
            int out2=out_table[l][1+2*h];
            int temp1=2*out1+out2;
             
            float test=pI_sym[j][temp1][1];
            temp[h]=B[j+1][next_table[l][h]]+test;
         }
         B[j][l]=logsum(temp[0],temp[1]);
         //B[j][l]=max(temp,2);
      }
   }
       
   //extrinsic information
   // K
   for (int j=0;j<k;j++)
   {
      int count0=0,count1=0;
      // state
      for (int l=0;l<st_num;l++)
      {    
         for (int h=0;h<2;h++)
         {
            if (h == 0)
            {        
               int out1=out_table[l][0+2*h];
               int out2=out_table[l][1+2*h];
                      
               int temp1=2*out1+out2;
                      
               t0[count0]=A[j][l]+B[j+1][next_table[l][h]]+pI_sym[j][temp1][0];  
               count0++;
            }
            else
            {
               int out1=out_table[l][0+2*h];
               int out2=out_table[l][1+2*h];
             
               int temp1=2*out1+out2;
                      
               t1[count1]=A[j][l]+B[j+1][next_table[l][h]]+pI_sym[j][temp1][0];   
               count1++;
            }
         }
      }
               
      float temp2=logsum(t0[0],t0[1]);
      float temp3=logsum(t1[0],t1[1]);
          
      for (int s=2;s<st_num;s++)
      {
         temp2=logsum(temp2,t0[s]);
         temp3=logsum(temp3,t1[s]);
      }
  
      if(option==1) { LLR[j]=temp3-temp2; }
          
      else { z[j]=temp3-temp2; }
   }
}

void decode(int *r_d)
{
   for (int i=0;i<k;i++)
   {
      if((z[i]+LLR[i])<0) r_d[i]=0;
      else r_d[i]=1;
   }
}

