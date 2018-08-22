# -*- coding: utf-8 -*-
"""
Test script for the C++ implementation of the
ctcdsp::components::ListenerPositionDecoder component
@author: Andreas Franck
"""

# Usage in debugger:
#exec(open("./test_listener_position_decoder.py").read())

# Import reference Python implementation.
from listenerPositionDecoder import ListenerPositionDecoder

import ctc

import visr
import pml
import rrl

import numpy as np
import json

fs = 48000
blockSize = 1064

context = visr.SignalFlowContext( period=blockSize, samplingFrequency=fs )

numberOfPeople = 2

compPy = ListenerPositionDecoder( context, "PosDecPy", None, numberOfListeners=numberOfPeople )
compCpp = ctc.ListenerPositionDecoder( context, "PosDecCpp", None, numberOfListeners=numberOfPeople )

flowPy = rrl.AudioSignalFlow(compPy)
flowCpp = rrl.AudioSignalFlow(compCpp)

msgPortPy = flowPy.parameterReceivePort("UDPMessage")
msgPortCpp = flowCpp.parameterReceivePort("messageIn")

listPortPy = flowPy.parameterSendPort("listenerPosition")
listPortCpp = flowCpp.parameterSendPort("listenerPosition")

numIterations = 10

for idx in range( numIterations ):
    pos = 2.0*(np.random.rand( 3, 2*numberOfPeople)-0.5)
    # Note for some reason the JSON message is expected in the shape  #numberOfEars x 3
    posStr = json.dumps( pos.T.tolist() )
    posParam = pml.StringParameter( posStr )

    msgPortPy.enqueue( posParam )
    msgPortCpp.enqueue( posParam )

    flowPy.process()
    flowCpp.process()

    if listPortPy.changed():
        posPy = np.array( listPortPy.data() )
        errPy = np.max(np.abs( posPy - pos ) )
        print( "Position difference for Python implementation max. %f" % errPy )
        listPortPy.resetChanged()
    else:
        print( "No output generated for Python implementation in iteration %d" % idx )

    if listPortCpp.changed():
        posCpp = np.array( listPortCpp.data() )
        errCpp = np.max(np.abs( posCpp - pos ) )
        print( "Position difference for C++ implementation max. %f" % errCpp )
        listPortCpp.resetChanged()
    else:
        print( "No output generated for C++ implementation in iteration %d" % idx )
