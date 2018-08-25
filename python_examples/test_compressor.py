# -*- coding: utf-8 -*-
"""

@author: Andreas Franck
"""

# Usage in debugger:
#exec(open("./test_compressor.py").read())


import numpy as np
from time import time

import matplotlib.pyplot as plt

import visr
# import pml
import rrl

import drc_visr

fs = 48000
blockSize = 64

context = visr.SignalFlowContext( period=blockSize, samplingFrequency=fs )

numIterations = 1024

numSamples = blockSize * numIterations

numChannels = 32

comp = drc_visr.Compressor( context, "Compressor", None,
                            numberOfChannels = numChannels,
                            averagingTimeSeconds=0.01,
                            attackTimeSeconds = 0.001,
                            releaseTimeSeconds=0.005,
                            compressorThresholdDB = -6.02,
                            compressorSlope = 1
                            )


flow = rrl.AudioSignalFlow( comp )

fSig = 80

fMod = 2;

t = 1.0/fs * np.arange( numSamples )

modSig = 0.5*(1.0+np.cos( fMod*2*np.pi * t ))
toneSig = np.cos( fSig*2*np.pi* t )

inputSignal = np.zeros( (numChannels, numSamples), dtype=np.float32 )
inputSignal[0,:] = toneSig * modSig
# inputSignal[1,:] = 2.5*toneSig * modSig



outputSignal = np.zeros( (numChannels, numSamples), dtype=np.float32 )


tStart = time()

for idx in range( numIterations ):

    outputSignal[:,blockSize*idx:blockSize*(idx+1)] = flow.process(
      inputSignal[:,blockSize*idx:blockSize*(idx+1)] )

tEnd = time()

print( "Time elapsed per second of audio: %f s" % ((tEnd-tStart)/(numSamples/fs)) )

plt.figure()
plt.plot( t, inputSignal[0,:], 'bo', t, outputSignal[0,:], 'r.-' )
#plt.plot( t, 20*np.log10(np.abs(inputSignal[0,:])), 'bo', t, 20*np.log10(np.abs(outputSignal[0,:])), 'r.-' )