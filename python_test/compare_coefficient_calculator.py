# -*- coding: utf-8 -*-
"""
Test and benchmarking script to compare the C++ implementation
ctcdsp::components::CoefficientCalculator to the Python component
TransauralCoefficientCalculator
@author: Andreas Franck and Marcos Simon
"""

# Usage in debugger:
# exec(open("./compare_coefficient_calculator.py").read())

# The ctc-visr repo must be on the Python search path
from transauralCoefficientCalculator import TransauralCoefficientCalculator

import visr
import rrl
import efl

import ctc

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import pylab

import time
pylab.close('all')

fs = 48000
N_fft = 1024
mod_delay = 0.0020 #modeling delay for the DAS filters
c0 =344

f = np.linspace(0., fs/2., ( N_fft/2)+1) #frequency vector used to debug

blockSize = 4096

numberOfPeople = 4

numIterations = 10
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

#creation of tcc components
tcc = TransauralCoefficientCalculator( c, "CoefficientCalculator", None, arrayConfiguration=lc, numberOfListeners = numberOfPeople, firLength = N_fft, speedOfSound = c0, betaR = regularisation, tMod = mod_delay)

ccCpp = ctc.CoefficientCalculator( c, "CoefficientCalculator", None, arrayConfiguration=efl.BasicMatrixFloat(lc), numberOfListeners = numberOfPeople,
                                  firLength = N_fft, speedOfSound = c0, betaR = regularisation, modelingDelay = mod_delay )

flow = rrl.AudioSignalFlow(tcc)

flowCpp = rrl.AudioSignalFlow(ccCpp)


# Workaround: Create an zero-channel audio signal because the process() call requires it.
inSig = np.zeros( (0,blockSize), dtype = np.float32 )

#pylab.figure()#plotting control geometry
#pylab.plot(lcx, lcy, 'bs', markersize=12, label='Loudspeakers')
firTapsPy = np.zeros((4*numberOfPeople*numberOfPeople , N_fft))
firTapsCpp = np.zeros((4*numberOfPeople*numberOfPeople , N_fft))
start = time.time()

listPortPy = flow.parameterReceivePort("listenerPos")
listPortCpp = flowCpp.parameterReceivePort("listenerPos")



for idx in range(0, numIterations ):
    # Retrieve the listener position matrix
    listPos = np.array( listPortPy.data(), copy=False)
    listPos[:, 0]  = [r_v*np.cos(angle_listeners[0] - angle_vis), r_v*np.sin(angle_listeners[0] - angle_vis), 0.0]
    listPos[:, 1] = [r_v*np.cos(angle_listeners[0]  + angle_vis), r_v*np.sin(angle_listeners[0] + angle_vis), 0.0]
# Only one listener ATMx
#    listPos[:, 2]  =  [r_v*np.cos(angle_listeners[1]  - angle_vis), r_v*np.sin(angle_listeners[1] - angle_vis), 0.0]
#    listPos[:, 3] = [r_v*np.cos(angle_listeners[1]  + angle_vis), r_v*np.sin(angle_listeners[1] + angle_vis), 0.0]

    listPosCpp = np.array(listPortCpp.data(), copy=False )
    listPosCpp[...] = listPos
#    listPos[:, 0]  = [-r_e/2 + listTrajectory[idx] - listeners_offset/2, 2.0, 0.0]
#    listPos[:, 1] = [r_e/2 + listTrajectory[idx] - listeners_offset/2, 2.0, 0.0]
#    listPos[:, 2]  = [-r_e/2 + listTrajectory[idx] + listeners_offset/2, 2.0, 0.0]
#    listPos[:, 3] = [r_e/2 + listTrajectory[idx] + listeners_offset/2, 2.0, 0.0]

     #Signal the changed position to the external input port.
    listPortPy.swapBuffers()
    listPortCpp.swapBuffers()
    # Execute the flow. There is no audio, so we put in an 0*blockSize signal and discard the
    # 0*blockSize output
    flow.process()
    flowCpp.process()

    # Collect the computed outputs from the flow's external outputs and analyse them.
    if not flow.parameterSendPort("dsbGain").changed():
        print( "DSB gain not computed in iteration %d" % idx )
    else:
        gainsPy = np.array( flow.parameterSendPort("dsbGain").data() )
       #print( "DSB gain in iteration %d: %s" % (idx, str(gains) ) )
        flow.parameterSendPort("dsbGain").resetChanged()
    if not flowCpp.parameterSendPort("dsbGain").changed():
        print( "DSB gain not computed in iteration %d" % idx )
    else:
        gainsCpp = np.array( flowCpp.parameterSendPort("dsbGain").data() )
       #print( "DSB gain in iteration %d: %s" % (idx, str(gains) ) )
        flowCpp.parameterSendPort("dsbGain").resetChanged()



    if not flow.parameterSendPort("dsbDelay").changed():
        print( "DSB Delays not computed in iteration %d" % idx )
    else:
        delaysPy = np.array( flow.parameterSendPort("dsbDelay").data() )
        #rint( "DSB Delays in iteration %d: %s" % (idx, str(delays) ) )
        flow.parameterSendPort("dsbDelay").resetChanged()
    if not flowCpp.parameterSendPort("dsbDelay").changed():
        print( "DSB Delays not computed in iteration %d" % idx )
    else:
        delaysCpp = np.array( flowCpp.parameterSendPort("dsbDelay").data() )
        #rint( "DSB Delays in iteration %d: %s" % (idx, str(delays) ) )
        flowCpp.parameterSendPort("dsbDelay").resetChanged()

    # Visualise the filter update requests

    filterQueue = flow.parameterSendPort("filters")
    while not filterQueue.empty():
        filterStruct = filterQueue.front()
        #print( "Received filter update for index %d in iteration %d." % (filterStruct.index, idx ) )
        # Retrieve the filter coefficients
        firTapsPy[filterStruct.index] = np.array(filterStruct.value)
        # TODO: Analyse the filters.

        # Slight oddity: We have to delete the Python variable because it would screw up the memory management
        # if the underlying C++ object is deleted in pop() while the Python representation is still alive.

        del filterStruct

        filterQueue.pop()

    filterQueueCpp = flowCpp.parameterSendPort("filters")
    while not filterQueueCpp.empty():
        filterStruct = filterQueueCpp.front()
        #print( "Received filter update for index %d in iteration %d." % (filterStruct.index, idx ) )
        # Retrieve the filter coefficients
        firTapsCpp[filterStruct.index] = np.array(filterStruct.value)
        # TODO: Analyse the filters.

        # Slight oddity: We have to delete the Python variable because it would screw up the memory management
        # if the underlying C++ object is deleted in pop() while the Python representation is still alive.

        del filterStruct

        filterQueueCpp.pop()


#    filtCalc.calculateRadiattionPattern(lc, gains, delays, firTaps, f, 70, c0 , fs)

end =time.time()

print("Time for calculation %s" % str((end-start)/numIterations) )
   # test_distances = filtCalc.distancesTF(listPos, lc)

    #test_filters  =  filtCalc.calculateInverse(test_distances, regularisation, f, N_fft, c0)
    #filtCalc.calculateRadiattionPattern2(lc, gains, delays, firTaps, f, 35, c0 , fs)
