.. Copyright Andreas Franck 2018 <a.franck@soton.ac.uk> --- All rights reserved.
   Copyright Institute of Sound and Vibration Research,
   University of Southampton --- All rights reserved.

About
=====

This tutorial describes how the VISR framework can be extended by code written in compiled languages,specifically C++.
This way of extending the complimentary to the extension interfaces in the Python language - which are often more accessible and better suited for getting to know the VISR framework and prototyping algorithm.

This tutorial consists of source code (C++ and Python) and this document, available in PDF and online in HTML format.

Target audience
---------------

This tutorial is aimed at software developers that are (to some level) familiar with C++.
They should have same basic understanding of the VISR framework, e.g.,components, ports, and using signal from a Pyuthon interpreter.

Introducing the Dynamic Range Control (DRC) library
---------------------------------------------------

The tutorial describes the implementation and us of a dynamic range compression library in C++ within the VISR framework.
