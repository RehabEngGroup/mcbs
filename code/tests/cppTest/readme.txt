This directory includes the c++ test program.

The software has been tested on Windows (Visual Studio Express 2010), Linux (GCC 4.5.2), and Mac(GCC 4.2.1).
A simple CMakeLists.txt file has been included to simplify the creation of executable file on several platforms.
Once that you have installed CMake  on your platform, open a terminal and write either:
cmake -G "Visual Studio 2010" 
to produce a visual studio project, or
cmake .
to produce Makefiles

See http://www.cmake.org/ to download CMake and for additional information on its use.

The test program included with the software requires a command line argument which is the path of required data, ex:
testSpline ../../Data/4DofHrHaHfKf/Extended

During the execution, the test will 
a. compute the spline coefficients based on the lmt.in file in the InputData directory.
b. evaluate lmt and ma on the same nodes used at step a.
c. evaluate lmt and ma on a novel set of evaluation points located midway between each pair of consecutive nodes

Results of the test are available in the new .out files inside the data directory 
