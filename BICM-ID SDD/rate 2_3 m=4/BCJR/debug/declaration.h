#define m 4                               //memory order = 4
#define st_num (int)pow(2,m)
#define pi 3.1415926
            
struct complex{
       float re;
       float im;
       };

struct complex mapping[8];

int next_table[st_num][4]; //from input 00 to 11
int out_table[st_num][12];

int from_table[st_num][4];
int from_in_table[st_num][8];
int from_out_table[st_num][12];

float ray[(k+m)/2]; //record or the rayleigh r.v.
int h_table[8][3];

double bit_metric[2][(k+m)/2*3];
double llr[(k+m)/2*3];
double id[2][(k+m)/2*3];
double out[2][k+m]={};

int u[k+m],v[(k+m)/2*3],y[k+m];

int record1[(k+m)/2];
int record2[(k+m)/2];
int record3[(k+m)/2];

void declaration(void);
void table(void);

void declaration(){
     
     table(); 
     
     rand_permute((k+m)/2,record1);
     rand_permute((k+m)/2,record2);
     rand_permute((k+m)/2,record3);
     
     }

void table(void){
     int x,i,j,h,temp,s[m],t[12],temp1[4];
     int t1,t2,t3;
     
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
     
     for (j=0;j<st_num;j++){
         
         temp=j;
         for(h=m-1;h>=0;h--){ //derive current state in binary form
             s[h]=temp/(int)pow(2,h);
             if(h!=0) temp=temp%(int)pow(2,h);
             }        
          
          next_table[j][0]=2*s[0]+8*s[2];      //table for the next states
          next_table[j][1]=2*s[0]+4+8*s[2];
          next_table[j][2]=1+2*s[0]+8*s[2];
          next_table[j][3]=1+2*s[0]+4+8*s[2];
          
          t[0]=s[0]^s[1]^s[2];   //table for the path outputs
          t[1]=s[1]^s[3];
          t[2]=s[2]^s[3];
                 
          t[3]=s[0]^s[1]^s[2]; 
          t[4]=1^s[1]^s[3];
          t[5]=1^s[2]^s[3];
          
          t[6]=1^s[0]^s[1]^s[2]; 
          t[7]=s[1]^s[3];
          t[8]=1^s[2]^s[3];
         
          t[9]=1^s[0]^s[1]^s[2];
          t[10]=1^s[1]^s[3];
          t[11]=s[2]^s[3];
         
         
          for(i=0;i<12;i++){
              out_table[j][i] = t[i];
              
              }        
         }
     
    for (i=0;i<16;i++){
         
        x=0;
        for (j=0;j<16;j++){
        
            for (h=0;h<4;h++){
        
            
                if (next_table[j][h]==i){
                                     
                   from_table[i][x]=j;
                   
                   t1=h;
                   from_in_table[i][2*x]=t1/2;
                   t1=t1%2;
                   from_in_table[i][1+2*x]=t1;
                   
                   
                   from_out_table[i][0+3*x]=out_table[j][0+3*h];
                   from_out_table[i][1+3*x]=out_table[j][1+3*h];
                   from_out_table[i][2+3*x]=out_table[j][2+3*h];
               
                   x++;  
                   }
                }
             }
        }
      }
