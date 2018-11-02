.. Copyright Andreas Franck 2018 <a.franck@soton.ac.uk> --- All rights reserved.
   Copyright Institute of Sound and Vibration Research,
   University of Southampton --- All rights reserved.

Creating Python bindings
------------------------

Even if a component is implemented in C++, there are many several reasons
why accessing them from Python is a good idea.
For example, they can be tested and compared with other Python-based components, for example a reference implementation of the same component, in an interactive environments.
On the other hand, creating complex signal flows can be put together and configured much quicker in Python without sacrificing the potential performance advantage of a C++ implementation.

To make a C++ component available in Python, we create a so-called *Python binding*.
