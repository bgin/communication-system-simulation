#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "LAB524Rand.h"

// Return random data bits
int *generateDataBits(int FrameLength, LAB524Rand* rng);

// Return modulated symbols
complex<double> *tx(int *originalBitSequence, double totalEnergyPerOFDMSymbol, int modulationFormat, int bitsPerSymbol, double powerBoosting, double* totalNumSubcarrierUsed);

#endif
