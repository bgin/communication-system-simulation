void *input(int *,int *);

void *input(int *u,int *z){
    int i,j,temp;
    
    int u1[(k+m)/2],u2[(k+m)/2],v1[(k+m)/2],v2[(k+m)/2],v3[(k+m)/2];
    
    
    for (i=-m;i<(k+m);i++){
        if (-1<i && i<k) u[i]=rand()%2;
        else u[i]=0;
        }
   
    for (i=-(m/2);i<(k+m)/2;i++){
        u1[i]=u[i*2];
        u2[i]=u[i*2+1];
        }
  
    for (i=0;i<(k+m)/2;i++){                                   //encoding
        v1[i]=u1[i]^u1[i-1]^u1[i-3]^u2[i-2]^u2[i-1];
        v2[i]=u1[i-2]^u1[i-1]^u2[i]^u2[i-1]^u2[i-3];
        v3[i]=u1[i]^u1[i-1]^u1[i-3]^u2[i]^u2[i-1]^u2[i-2]^u2[i-3];
        }
   
   interleave1(v1);
   interleave2(v2);
   interleave3(v3);
   
   for (i=0;i<(k+m)/2;i++){                                   //encoding
        z[3*i]  =v1[i];
        z[3*i+1]=v2[i];
        z[3*i+2]=v3[i];
        }
   
   
   
   
   /*free(u1);
   free(u2);
   free(v1);
   free(v2);
   free(v3);*/
   
   
   }
