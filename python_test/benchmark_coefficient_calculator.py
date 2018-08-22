# -*- coding: utf-8 -*-
"""
Test and benchmarking script to compare the C++ implementation
ctcdsp::components::CoefficientCalculator to the Python component
TransauralCoefficientCalculator
@author: Andreas Franck and Marcos Simon
"""

# Usage in debugger:
# exec(open("./compare_coefficient_calculator.py").read())


import visr
import rrl
import efl

import ctc

# The ctc-visr repo must be on the Python search path
from transauralCoefficientCalculator import TransauralCoefficientCalculator

import numpy as np
from matplotlib import pylab

import time
pylab.close('all')

fs = 48000
N_fft = 1024
mod_delay = 0.0020 #modeling delay for the DAS filters
c0 =344

f = np.linspace(0., fs/2., ( N_fft/2)+1) #frequency vector used to debug

blockSize = 4096

numberOfPeople = 2

numIterations = 100
listTrajectory =  np.linspace(0, .00000001, numIterations, dtype=np.float32)

positionStepSize = 0.001

c = visr.SignalFlowContext(blockSize, fs )
#creation of array centred at position
L = 16 #number of loudspeakerss
d = 0.0386 #separation between the loudspeakers
r_e = 0.19 #separation between the listen
regularisation = 1e-4 #regularisation factor

lcx = np.linspace(((-L+1)/2)*d, ((L-1)/2)*d, L, dtype=np.float32)
lcy = np.zeros( lcx.shape )
lcz = np.zeros( lcx.shape)
lc = np.vstack((lcx, lcy, lcz) )

#parameters for simulation
listeners_offset = 0.8 #distance between both filters
angle_listeners = np.deg2rad(np.array([69.7, 110]))
r_v = 2.0
angle_vis_linear = r_e/(2*r_v)
angle_vis = (np.arctan(angle_vis_linear))


mod_delay = 0.2 #modellin delay for the DAS filters
#%%

nBench = 1000

# Python reference implementation.
ccPy = TransauralCoefficientCalculator( c, "CoefficientCalculator", None, arrayConfiguration=lc, numberOfListeners = numberOfPeople, firLength = N_fft, speedOfSound = c0,
                                       betaR = regularisation, tMod = mod_delay)
flowPy = rrl.AudioSignalFlow(ccPy)
listPortPy = flowPy.parameterReceivePort("listenerPos")

tStart = time.time()
for idx in range(nBench):
    listPos = np.array( listPortPy.data(), copy=False)
    listPos[:, 0]  = [r_v*np.cos(angle_listeners[0] - angle_vis), r_v*np.sin(angle_listeners[0] - angle_vis), 0.0]
    listPos[:, 1] = [r_v*np.cos(angle_listeners[0]  + angle_vis), r_v*np.sin(angle_listeners[0] + angle_vis), 0.0]
    listPortPy.swapBuffers()
    flowPy.process()
    filterQueuePy = flowPy.parameterSendPort("filters")
    while not filterQueuePy.empty():
        filterStruct = filterQueuePy.front()
        del filterStruct
        filterQueuePy.pop()
tEnd = time.time()
print("Time for calculation (Python) %s" % str((tEnd-tStart)/nBench) )




ccCpp = ctc.CoefficientCalculator( c, "CoefficientCalculator", None, arrayConfiguration=efl.BasicMatrixFloat(lc), numberOfListeners = numberOfPeople,
                                  firLength = N_fft, speedOfSound = c0, betaR = regularisation, modelingDelay = mod_delay )
flowCpp = rrl.AudioSignalFlow(ccCpp)
listPortCpp = flowCpp.parameterReceivePort("listenerPos")

tStart = time.time()
for idx in range(nBench):
    listPos = np.array( listPortCpp.data(), copy=False)
    listPos[:, 0]  = [r_v*np.cos(angle_listeners[0] - angle_vis), r_v*np.sin(angle_listeners[0] - angle_vis), 0.0]
    listPos[:, 1] = [r_v*np.cos(angle_listeners[0]  + angle_vis), r_v*np.sin(angle_listeners[0] + angle_vis), 0.0]
    listPortCpp.swapBuffers()
    flowCpp.process()

    filterQueueCpp = flowCpp.parameterSendPort("filters")
    while not filterQueueCpp.empty():
        filterStruct = filterQueueCpp.front()
        del filterStruct
        filterQueueCpp.pop()
tEnd = time.time()
print("Time for calculation (C++) %s" % str((tEnd-tStart)/nBench) )