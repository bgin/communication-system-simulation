void deinterleave1(double *);
void deinterleave2(double *);
void deinterleave3(double *);

void deinterleave(double *x1){
     int i,j;
     double *temp;
     double metric1_0[(k+m)/2],metric2_0[(k+m)/2],metric3_0[(k+m)/2];
     
     for (i=0;i<(k+m)/2;i++){
         metric1_0[i]=x1[4*i];
         metric2_0[i]=x1[4*i+1];
         metric3_0[i]=x1[4*i+2];
         }
     
        deinterleave1(metric1_0);
        deinterleave2(metric2_0);
        deinterleave3(metric3_0);
        
     for (i=0;i<(k+m)/2;i++){
         x1[4*i]=metric1_0[i];
         x1[4*i+1]=metric2_0[i];
         x1[4*i+2]=metric3_0[i];
         }
     }


void interleave1(int *x){
     int i,j,*temp;
     temp=(int *) malloc((k+m)/2*sizeof(int));
     
     for (i=0;i<40;i++){
         for (j=0;j<50;j++){
             temp[50*i+j]=x[40*j+i];
             }
         }
     
     for (i=0;i<2000;i++){
         x[i]=temp[i];
         }
     
     free(temp);
     }

void deinterleave1(double *x){
     int i,j;
     double *temp;
     temp=(double *) malloc((k+m)/2*sizeof(double));
     
     for (i=0;i<40;i++){
         for (j=0;j<50;j++){
             temp[40*j+i]=x[50*i+j];
             }
         }
     
     for (i=0;i<2000;i++){
         x[i]=temp[i];
         }
     
     free(temp);
     }
     
void interleave2(int *x){
     int i,j,*temp;
     temp=(int *) malloc((k+m)/2*sizeof(int));
     
     for (i=0;i<80;i++){
         for (j=0;j<25;j++){
             temp[25*i+j]=x[80*j+i];
             }
         }
     
     for (i=0;i<2000;i++){
         x[i]=temp[i];
         }
     
     free(temp);
     }

void deinterleave2(double *x){
     int i,j;
     double *temp;
     temp=(double *) malloc((k+m)/2*sizeof(double));
     
     for (i=0;i<80;i++){
         for (j=0;j<25;j++){
             temp[80*j+i]=x[25*i+j];
             }
         }
     
     for (i=0;i<2000;i++){
         x[i]=temp[i];
         }
     
     free(temp);
     }
     
void interleave3(int *x){
     int i,j,*temp;
     temp=(int *) malloc((k+m)/2*sizeof(int));
     
     for (i=0;i<100;i++){
         for (j=0;j<20;j++){
             temp[20*i+j]=x[100*j+i];
             }
         }
     
     for (i=0;i<2000;i++){
         x[i]=temp[i];
         }
     
     free(temp);
     }

void deinterleave3(double *x){
     int i,j;
     double *temp;
     temp=(double *) malloc((k+m)/2*sizeof(double));
     
     for (i=0;i<100;i++){
         for (j=0;j<20;j++){
             temp[100*j+i]=x[20*i+j];
             }
         }
     
     for (i=0;i<2000;i++){
         x[i]=temp[i];
         }
     
     free(temp);
     }
