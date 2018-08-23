# -*- coding: utf-8 -*-
"""

@author: Andreas Franck
"""

# Usage in debugger:
#exec(open("./test_compressor.py").read())

import visr
import pml
import rrl

import drc_visr

import numpy as np

fs = 48000
blockSize = 64

context = visr.SignalFlowContext( period=blockSize, samplingFrequency=fs )

numIterations = 128

numSamples = blockSize * numIterations

numChannels = 1

comp = drc_visr.Compressor( context, "Compressor", None,
                            numberOfChannels = numChannels,
                            attackTimeSeconds = 0.001,
                            releaseTimeSeconds=0.05,
                            compressorThresholdDB = -6,
                            compressorSlopeDB = 10.0,
                            limiterThresholdDB=np.inf,
                            limiterSlopeDB=-60.0 )


flow = rrl.AudioSignalFlow( comp )

fSig = 800

fMod = 10;

t = 1.0/fs * np.arange( numSamples )

modSig = 0.5*(1.0+np.sin( fMod*2*np.pi * t ))
toneSig = np.cos( fSig*2*np.pi* t )

inputSignal = np.zeros( (1,numSamples), dtype=np.float32 )
inputSignal[0,:] = toneSig * modSig

outputSignal = np.zeros( (1,numSamples), dtype=np.float32 )


for idx in range( numIterations ):

    outputSignal[:,blockSize*idx:blockSize*(idx+1)] = flow.process(
      inputSignal[:,blockSize*idx:blockSize*(idx+1)] )
