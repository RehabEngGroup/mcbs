# Multidimensional cubic Bsplines software

This directory includes the software for multidimensional cubic Bsplines (MCBS).

There are six main directories:

- _lib_: core files implementing the multidimensional cubic Bspline library
- _src_: source code for the useful _osimSpliner_ executable, that computes spline coefficients from an OpenSim model.
- _tests_:
    - _cppTest_: a test for the library, using C++
    - _matlabTest_: a test using the matlab interface
- _examples_: a set of programs that show how to use the MCBS library
- _scripts_: currently, a couple of scripts to analyze the results of the example programs

## How to compile

MCBS is cross-platform software that relies on the  [CMake](http://cmake.org/) build system.
Please refer to [official CMake documentation](https://cmake.org/runningcmake/) for instructions on how to configure and build this software on the operating system and with the compiler/build environment that you are using.

__Important note:__ There are no _hard_ dependencies that are required to build the MCBS library; however, to build some of the examples and the _osimSpliner_ executable, you need to have [OpenSim](http://opensim.stanford.edu) installed in your system. Please ensure that the OpenSim installation is found by CMake during configuration.

