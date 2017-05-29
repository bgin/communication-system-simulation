void Minterleave(double *x1, int k){
     int i,j;
     double temp1[k];
     
     for (i=0;i<k;i++){
         temp1[i]=x1[record[i]];
         }
     
     for (i=0;i<k;i++){
         x1[i]=temp1[i];
         }
     }

void interleave(int *x, int k){
     int i,j;
     int temp[k];
     
     for (i=0;i<k;i++){
         temp[i]=x[record[i]];
         }
     
     for (i=0;i<k;i++){
         x[i]=temp[i];
         }
     
     }
     
void deinterleave(double *x1, int k){
     int i,j;
     double temp1[k];
      
     for (i=0;i<k;i++){
         temp1[record[i]]=x1[i];
         }
     
     for (i=0;i<k;i++){
         x1[i]=temp1[i];
         }
     
     }

void rand_permute(int *a,int len){
     int i,j,x;
     int temp[len];
     
     for (i=0;i<len;i++){
         temp[i]=i;
     }
     
     for (i=0;i<len;i++){
         x=(rand()%(len-i));
         
         a[i]=temp[x];
         
         for (j=x;j<len-1-i;j++){
             temp[j]=temp[j+1];
             
         }
     }
}

inline int isNumOfBit1Odd(int value, int bitCount)
{
   int num = 0;
   int iShift = 0;
   while (iShift <= bitCount-1)
   {
       num ^= (value>>iShift);
       iShift++;
   }
   return num&1;
}

void table(void);
void lookup(void);

inline void initialize()
{
   table(); 
   lookup();
}

void lookup(void)
{
   for (int i=0;i<100000;i++)
   {
      lookup_table[i] = exp(-i/1000.0);
   }
}

void table(void){
     int x,i,j,h,s[m],t0,t1;
     
     for (j=0;j<st_num;j++){

         for (h=0;h<m;h++){//s[] stores the content of shift registers
            s[h]=j>>h&1;
         }
        
        
         t0=0;
         t1=0;
         for (h=0;h<m;h++){
             t0=t0^s[h]*(G11>>(m-h-1)&1);
             t1=t1^s[h]*(G12>>(m-h-1)&1);
         }
         next_table[j][0]=(j<<1& st_num-1) + (t0^0);
         next_table[j][1]=(j<<1& st_num-1) + (t0^1);  
         
         //printf("%d ",next_table[j][0]);
         //printf("%d \n",next_table[j][1]);
         
         out_table[j][0]=0;
         out_table[j][1]=(t0^0)^t1;
         out_table[j][2]=1;
         out_table[j][3]=(t0^1)^t1;
        
     }
     
    //construct "from" tables
    for (i=0;i<st_num;i++){
        x=0;
        for (j=0;j<st_num;j++){
        
            for (h=0;h<2;h++){
        
                if (next_table[j][h]==i){
                                     
                   from_table[i][x]=j;
                        
                   from_out_table[i][0+2*x]=out_table[j][0+2*h];
                   from_out_table[i][1+2*x]=out_table[j][1+2*h];
                   
                   x++;  
                }
            }
        }
    }
    
    // Rewrite the look up tables
    for (int SRvalue=0;SRvalue<st_num;SRvalue++)
    {  
       int inputBitToSR;
       inputBitToSR = isNumOfBit1Odd(G11&SRvalue,m) ^ 0;
       stateTransition[SRvalue][0] = (SRvalue >> 1) | (inputBitToSR << (m-1));
       stateOutput[SRvalue][0] = isNumOfBit1Odd(G12&SRvalue,m) ^ (inputBitToSR&(G12>>m));

       inputBitToSR = isNumOfBit1Odd(G11&SRvalue,m) ^ 1;
       stateTransition[SRvalue][1] = (SRvalue >> 1) | (inputBitToSR << (m-1));
       stateOutput[SRvalue][1] = isNumOfBit1Odd(G12&SRvalue,m) ^ (inputBitToSR&(G12>>m));
    }
}

/*
double logsum(double a,double b)
{
    double max = (a>b) ? a : b;
    
    double delta = (a>b) ? (a-b) : (b-a);
    
    if (delta>100)delta=100;
    
    delta=log(1+exp(-delta));
    
    return max+delta;
}
*/


// Aided by table look up
inline double logsum(double a,double b)
{
    double max = (a>b) ? a : b;
    
    double delta = (a>b) ? (a-b) : (b-a);
    
    if (delta>100) { delta = 99.999; }

    delta = log(1+lookup_table[int(delta*1000)]);
    
    return max+delta;
}

