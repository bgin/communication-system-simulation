#define Ps 1.0         //constellation point for systematic bits
//#define Pp 0.88881944  //constellation point for parity bits

float Pp;

void output(int *,float,int *);                     //main function,producing the decoded sequence
void viterbi(void);                                 //sub function,calculate the weights and optimal path
void decode(int *);                                 //sub function,construct the decoded sequence using  optimal path

void puncture(float *y){
     int i;
     
     for (i=0;i<(2*k_m);i++){
         if ((i&3)==1) { y[i]=0; }
     }
}


void output(int *x_c,float snr,int *r_d){
    int i,j=0;
    float N0,sd,*r;                             //temp = sqrt(Eb)
      
    r=(float *) malloc((2*k_m)*sizeof(float));
    
    N0=2/snr;                                    //SNR = Eb*2/N0
    sd=sqrt(N0/2);                                    //standard deviation of gaussian r.v.
   
    while (j<(2*k_m)){
    
              if(j&1==0) r[j]=2*Ps*x_c[j]-Ps;                 //r is the received vector                                 
              else r[j]=2*Pp*x_c[j]-Pp; 
              
              r[j]+=gaurand()*sd;        //add noise
              
              //ray[j]=rayrand();
              //r[j]=gaurand()*sd+temp*ray[j];
              j++;
              }
     
     //puncture(r);
     
     j=0;  
     while (j<k_m){ //compute path cost
              
              path_metric[j][0]= -Ps*r[2*j] + -Pp*r[2*j+1];  //cost of output 00
              path_metric[j][1]= -Ps*r[2*j] +  Pp*r[2*j+1];  //cost of output 01
              path_metric[j][2]=  Ps*r[2*j] + -Pp*r[2*j+1];  //cost of output 10
              path_metric[j][3]=  Ps*r[2*j] +  Pp*r[2*j+1];  //cost of output 11
              
              
             /* path_metric[j][0]= -1*r[2*j]*ray[2*j] + -1*r[2*j+1]*ray[2*j+1];  //cost of output 00
              path_metric[j][1]= -1*r[2*j]*ray[2*j] +  1*r[2*j+1]*ray[2*j+1];  //cost of output 01
              path_metric[j][2]=  1*r[2*j]*ray[2*j] + -1*r[2*j+1]*ray[2*j+1];  //cost of output 10
              path_metric[j][3]=  1*r[2*j]*ray[2*j] +  1*r[2*j+1]*ray[2*j+1];  //cost of output 11*/
              
              j++;
              }
   
   /*-----------------decoding--------------------*/ 
   
    viterbi();
        
    decode(r_d);

    free(r);
    }

void viterbi(){
     int i,j,next1,next0;
     float w1,w0;
     float *cost,*temp,*pass;
     
     cost=(float *) malloc(st_num*sizeof(float));
     temp=(float *) malloc(st_num*sizeof(float)); 
     
     for (i=0;i<st_num;i++){
         cost[i]=0;
         temp[i]=0;
     }
     
     cost[0]=65536;
     
     for (i=0;i<k_m;i++){
        
        for(j=0;j<st_num;j++){
             
             next1 = next_table[j][1];
             next0 = next_table[j][0];
             
             if (cost[j]!=0){
                   
               w0=path_metric[i][out_table_index[j][0]];      //path costs
               w1=path_metric[i][out_table_index[j][1]];
               
               if ( temp[next1] == 0 || (cost[j] + w1) > temp[next1] ){
                  node[(i+1)][next1].from=&node[i][j]; 
                  node[(i+1)][next1].in=1;
                  temp[next1]=cost[j] + w1;
                  }                       
               
               if ( temp[next0] == 0 || (cost[j] + w0) > temp[next0] ){
                  node[(i+1)][next0].from=&node[i][j]; 
                  node[(i+1)][next0].in=0;
                  temp[next0]=cost[j] + w0;
                  }
               }
             }
 
             pass=cost;
             cost=temp;
             temp=pass;
             for(j=0;j<st_num;j++) {
                 temp[j]=0;
                 }
         }
      free(cost);
      free(temp);
      }


void decode(int *r_d){
     
     struct state *temp1;
     int i;

     temp1=&node[k_m][0];
     for (i=k_m-1;i>=0;i--){
            r_d[i]=temp1->in;
            temp1=temp1->from;
            }
     }
