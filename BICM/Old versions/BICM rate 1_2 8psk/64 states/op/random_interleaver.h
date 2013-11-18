void deinterleave1(double *);
void deinterleave2(double *);
void deinterleave3(double *);

void deinterleave(double *x1,double *x2){
     int i,j;
     double *temp;
     double metric1_0[(k+m)*2/3],metric2_0[(k+m)*2/3],metric3_0[(k+m)*2/3];
     double metric1_1[(k+m)*2/3],metric2_1[(k+m)*2/3],metric3_1[(k+m)*2/3];
     
     
     for (i=0;i<(k+m)*2/3;i++){
         metric1_0[i]=x1[3*i];
         metric2_0[i]=x1[3*i+1];
         metric3_0[i]=x1[3*i+2];
         metric1_1[i]=x2[3*i];
         metric2_1[i]=x2[3*i+1];
         metric3_1[i]=x2[3*i+2];
         }
     
        deinterleave1(metric1_0);
        deinterleave2(metric2_0);
        deinterleave3(metric3_0);
        deinterleave1(metric1_1);
        deinterleave2(metric2_1);
        deinterleave3(metric3_1);
     
     for (i=0;i<(k+m)*2/3;i++){
         x1[3*i]=metric1_0[i];
         x1[3*i+1]=metric2_0[i];
         x1[3*i+2]=metric3_0[i];
         x2[3*i]=metric1_1[i];
         x2[3*i+1]=metric2_1[i];
         x2[3*i+2]=metric3_1[i];
         }
     }


void interleave1(int *x){
     int i,j;
     int temp[(k+m)*2/3];
     
     for (i=0;i<(k+m)*2/3;i++){
         temp[i]=x[record1[i]];
         }
     
     for (i=0;i<(k+m)*2/3;i++){
         x[i]=temp[i];
         }
     
     }
     
void deinterleave1(double *x){
     int i,j;
     double temp[(k+m)*2/3];
      
     for (i=0;i<(k+m)*2/3;i++){
         temp[record1[i]]=x[i];
         }
     
     for (i=0;i<(k+m)*2/3;i++){
         x[i]=temp[i];
         }
     
     }
     
void interleave2(int *x){
     int i,j;
     int temp[(k+m)*2/3];
     
     for (i=0;i<(k+m)*2/3;i++){
         temp[i]=x[record2[i]];
         }
     
     for (i=0;i<(k+m)*2/3;i++){
         x[i]=temp[i];
         }
     
     }
     
void deinterleave2(double *x){
     int i,j;
     double temp[(k+m)*2/3];
      
     for (i=0;i<(k+m)*2/3;i++){
         temp[record2[i]]=x[i];
         }
     
     for (i=0;i<(k+m)*2/3;i++){
         x[i]=temp[i];
         }
     
     }
     
void interleave3(int *x){
     int i,j;
     int temp[(k+m)*2/3];
     
     for (i=0;i<(k+m)*2/3;i++){
         temp[i]=x[record3[i]];
         }
     
     for (i=0;i<(k+m)*2/3;i++){
         x[i]=temp[i];
         }
     
     }
     
void deinterleave3(double *x){
     int i,j;
     double temp[(k+m)*2/3];
      
     for (i=0;i<(k+m)*2/3;i++){
         temp[record3[i]]=x[i];
         }
     
     for (i=0;i<(k+m)*2/3;i++){
         x[i]=temp[i];
         }
     
     }
