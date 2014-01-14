#define m 4                               //memory order = 4
#define st_num 16
#define pi 3.1415926
            
struct complex{
       double re;
       double im;
       };

int next_table[st_num][2]; //from input 0 to 1
int out_table[st_num][4];

int from_table[st_num][2];
int from_out_table[st_num][4];

double ray[k+m]; //record or the rayleigh r.v.
int h_table[8][3];

double bit_metric[2][k+m];
double dec1_out[2][k];
double feed[2][k];//dec2 out

double lookup_table[100000]={};//the real delta = index/1000

int u[k+m],v[(k+m)*3],y[k];

int record[k];

void declaration(void);
void table(void);
void lookup(void);

void declaration(){
     
     table(); 
     lookup();
     
     
     }

void lookup(void){
     int i;
     for (i=0;i<100000;i++){
         
         lookup_table[i]=exp(-(double)i/1000);
         
         }
     }

void table(void){
     int x,i,j,h,temp,s[4];
     int t1,t2,t3,t4;
     
     for (j=0;j<8;j++){
             temp=j;
             t1=(int)temp/4;
             temp=temp%4;
             t2=(int)temp/2;
             temp=temp%2;
             t3=temp;
          h_table[j][0]=t1;
          h_table[j][1]=t2;
          h_table[j][2]=t3;
          }
     
     for (j=0;j<16;j++){
         
         temp=j;
         for(h=m-1;h>=0;h--){ //derive current state in binary form
             s[h]=temp/(int)pow(2,h);
             if(h!=0) temp=temp%(int)pow(2,h);
             }
        
        next_table[j][0]=(0^s[0]^s[1]^s[2]^s[3])+2*s[0]+4*s[1]+8*s[2];
        next_table[j][1]=(1^s[0]^s[1]^s[2]^s[3])+2*s[0]+4*s[1]+8*s[2];        
    
        out_table[j][0]=0;
        out_table[j][1]=(0^s[0]^s[1]^s[2]^s[3])^s[3];
        out_table[j][2]=1;
        out_table[j][3]=(1^s[0]^s[1]^s[2]^s[3])^s[3];
        }
     
    //construct "from" tables
    for (i=0;i<16;i++){
        x=0;
        for (j=0;j<16;j++){
        
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
      }

double fmax(double x1,double x2){
       if(x1<x2) return x2;
       else return x1;
       }

double logsum(double a,double b){
       double max,delta;
       max=a;
       if (b>max) max=b;
       delta=a-b;
       if (delta<0) delta=0-delta;
       if (delta>100)delta=100;
       delta=log(1+exp(-delta));
       return max+delta;
       }

/*double logsum(double a,double b){    //aided by table look ups
       double max,delta;
       max=a;
       if (b>max) max=b;
       delta=a-b;
       if (delta<0) delta=0-delta;
       if (delta>100) delta=99.999;
       delta=log(1+lookup_table[int(delta*1000)]);
       return max+delta;
       }*/
