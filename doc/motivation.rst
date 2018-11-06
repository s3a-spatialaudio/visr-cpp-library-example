.. Copyright Andreas Franck 2018 <a.franck@soton.ac.uk> --- All rights reserved.
   Copyright Institute of Sound and Vibration Research,
   University of Southampton --- All rights reserved.

Motivation - Why using C++?
===========================

As explained in the VISR user documentation, implementing new atomic or composite components in Python is the quickest and easiest way to extend the VISR framework.

However, there are several circumstances where it is beneficial to create new components a compiled language such as C++:

Computational efficiency
  Depending on the algorithm, an implementation in a compiled language can outperform a straightforward Python implementation.
  However, this is not true in general.
  If high-performance libraries, for example from numerical libraries as :code:`numpy` or :code:`scipy`, can be used, it is unlikely that a naive C++ implementation is faster.
  On the other hand, the Python implementation requires highly scalar, non-vectorised code, for example fine-grained loops, a C or C++ implementation can achieve significant speedups.

Self-contained implementation and easier configuration
  Using Python components requires a compatible Python distribution and a correct configuration of the VISR framework(for example setting the :code:`PYHTONPATH` and :code:`PYHTONHOME`.
  Signal flows containing such components are therefore harder to disctribute, especially to users that are not developers and do not require the flexibility offered by the Python integration, but where a VISR signal flow is used internally, for example, in a DAW plugin or Max/MSP external.
  A C++ implementation results in an implementation which fewer external dependencies that is easier to use and less dependent on the system configuration of the host computer.
Interfacing C/C++libraries
  In some cases, the purpose of a component is to make functionality that exists in a C or C++ software library available in the VISR framework.
  This is easy to achieve by implementing a component in C++.
  While there are other possibilities (such as using existing Python binding of the library or implementing one yourself), an implementation within a C+++ VISR component is often the most straightforward way to do that, and results in a compact implementation, where the complete interface code is contained in the component itself.

The tutorial example: Dynamic Range Compression library (DRC)
-------------------------------------------------------------

In this 
