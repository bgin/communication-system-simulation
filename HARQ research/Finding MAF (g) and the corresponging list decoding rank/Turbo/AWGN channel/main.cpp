/* Correct the mechanism for searching k
 * Change channel.cpp back; the mechanism to buffer noise has been removed
 * Put the g and k search in {}
 * Date : 2007/11/26
 */

#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

#include "LAB524Rand.h"

#include "timer.h"
#include "transmitter.h"
#include "channel.h"
#include "receiver.h"
#include "fileIO.h"

const double ACCURACY = getValue("Allowable deviation", "config.txt");

int diffBits(int *a, int *b, int len)
{
   int count = 0;
   for (int i=0;i<len;i++)
   {
      if (a[i +1] != b[i +1]) { count++; }
   }
   return count;   
}

int main()
{

   const double start = getValue("From SNR", "config.txt");
   const double end = getValue("To SNR", "config.txt");
   const double step = getValue("SNR step", "config.txt");
   const int FL = (int)getValue("Frame length", "config.txt");

   const double codeRate = 1.0/3.0;

   const int SNRpoints=(int)round((end-start)/step + 1);
   double snr[SNRpoints], snrdB[SNRpoints], ber[SNRpoints], fer[SNRpoints];
   // Sample SNR value
   for (int iSNR=0;iSNR<SNRpoints;iSNR++)
   {
      snrdB[iSNR] = start + iSNR*step;
      snr[iSNR] = pow(10,snrdB[iSNR]/10);  
   }                                                

   // Choose random seed   
//   srand(time(NULL)%60);
   LAB524Rand rng(time(NULL));

   Timer timer;
    
   ofstream data;
   data.open("data.txt",ios::app);

   ofstream gData;
   gData.open("gData.txt",ios::app);
   
   int nLoop;

   const int nRetran = (int)getValue("Number of retransmission for avg","config.txt");

   double meanMAF[SNRpoints],meanSquareMAF[SNRpoints],varianceMAF[SNRpoints];
                  
   for (int iSNR=0;iSNR<SNRpoints;iSNR++)
   {
      cout << "Loop " << iSNR+1 << ", SNR = " << fixed << snrdB[iSNR] << endl;

      // Eb is the energy per bit of the modulated signal
      double Eb = 1.0;
      double N0 = Eb/snr[iSNR]/codeRate;         

      double *MAFrecord = new double [nRetran]; 
      
      int nRetranError = 0;
         
      int iRetran;
      for (iRetran=0;iRetran<nRetran;)
      {
         int *u = generateDataBits(FL,&rng);
          
         double *x = tx(u,Eb);
     
         // Valid for BPSK 
         const int symLen = (int)x[0];
            
         double *r = AWGN(x,N0,&rng);
//            double *ray = Rayleigh(double *x, double N0, double mP);
      
         int *y = rx(r,N0);
      
         bool decodable = !(diffBits(u,y,FL));
         delete [] y;
         
         // If 1st decoding succeeds, continue
         if (decodable == true)
         {  
               delete [] r;delete [] u;delete [] x;
               continue;
         }
         
         /*************Find MAF*********************************************/
         // Assign an initial value to start the search
         {
            double g = 1.0;
            
            double r2[symLen +1]; 
            r2[0] = symLen;
         
            // Double the initial value repeatedly untill >= the Answer
            while (1)
            {
               // Apply MAF to x[]
               for (int l=0;l<symLen;l++)
               {
                  r2[l +1] = g*x[l +1] + r[l +1];
               }

               int *y = rx(r2,N0);
            
               bool decodable = !(diffBits(u,y,FL));
               delete [] y;
            
               // Check if can be decoded correctly
               if (decodable == true) { break; }
               else { g *= 2; }
            }

            g /= 2;
            double gStep = g/2;
            // Begin the binary search
            while (1)
            {
               // Apply MAF to x[]
               for (int l=0;l<symLen;l++)
               {
                  r2[l +1] = g*x[l +1] + r[l +1];
               }
            
               int *y = rx(r2,N0);
            
               bool decodable = !(diffBits(u,y,FL));
               delete [] y;
            
               if (decodable == true) { g -= gStep; }
               else { g += gStep; }
                
               if (gStep <= ACCURACY)
               {
                  MAFrecord[iRetran] = g;
                  gData << g << endl;
                 break;
               }
            
               gStep /= 2;
            }
         }
         /****************************************************************/

         delete [] u;delete [] x;delete [] r;

         cout << "iRetran = " << iRetran << endl;
         iRetran++;
      }//end of Retran loop

         
      // Clalculate mean, mean square MAF
      double total1 = 0, total2 = 0;
      for (int j=0;j<nRetran;j++)
      {
            total1 += MAFrecord[j];
            total2 += MAFrecord[j]*MAFrecord[j];
      }
      meanMAF[iSNR] = total1/(double)nRetran;
      meanSquareMAF[iSNR] = total2/(double)nRetran;
      varianceMAF[iSNR] = meanSquareMAF[iSNR] - meanMAF[iSNR]*meanMAF[iSNR];      
      
      if (iSNR == 0)
      {
         data << "********************************************" << endl;
         data << "Simulation started at  : "; data << timer.getStartTime(); data << endl;
      }
      
      cout << "SNR = " << fixed << snrdB[iSNR] << endl;
      data << "SNR = " << fixed << snrdB[iSNR] << endl;
      cout << "mean MAF = " << meanMAF[iSNR] << endl;
      data << "mean MAF = " << meanMAF[iSNR] << endl;
      cout << "mean square MAF = " << meanSquareMAF[iSNR] << endl;
      data << "mean square MAF = " << meanSquareMAF[iSNR] << endl;
      cout << "variance MAF = " << varianceMAF[iSNR] << endl << endl;
      data << "variance MAF = " << varianceMAF[iSNR] << endl << endl;
      
      delete [] MAFrecord;
      
      nLoop = iSNR+1;
      cout << endl << endl;;
   }
    
   // print SNR & BER
   for (int i=0;i<nLoop;i++)
   {
      data << "--------------------------" << endl;
      cout << "--------------------------" << endl;

      cout << "SNR = " << fixed << snrdB[i] << endl;
      data << "SNR = " << fixed << snrdB[i] << endl;      
      
      data << "mean MAF------------------" << endl;
      cout << "mean MAF------------------" << endl;
      
      cout << meanMAF[i] << endl;
      data << meanMAF[i] << endl;
      
      data << "mean square MAF-----------" << endl;
      cout << "mean square MAF-----------" << endl;
      
      cout << meanSquareMAF[i] << endl;
      data << meanSquareMAF[i] << endl;
      
      data << "variance MAF-------------" << endl;
      cout << "variance MAF--------------" << endl;
      
      cout << varianceMAF[i] << endl;
      data << varianceMAF[i] << endl;
   }
   
   data << "--------------------------" << endl;
   cout << "--------------------------" << endl;
    
   cout << "Simulation started at  : "; cout << timer.getStartTime() << endl;
   cout << "Simulation finished at : "; cout << timer.getCurrentTime() << endl;
   cout << "Time elapsed : "; cout << timer.getElapsedTime() << endl;
   
   data << "Simulation finished at : "; data << timer.getCurrentTime() << endl;
   data << "Time elapsed : "; data << timer.getElapsedTime() << endl;

   data << "********************************************" << endl;
   data.close();gData.close();
   
   system("pause");
   
   return 0;
}
