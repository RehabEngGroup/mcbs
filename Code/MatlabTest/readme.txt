This directory includes the file for the matlab interface.

To test the software you need to create the mex files with the following commands:
mex createSpline.cpp ../src/SplineBasisFunction.cpp
mex evalSpline.cpp ../src/SplineBasisFunction.cpp

Now you can run the test:
splineMatlab

This test does the following steps on the data in the Data/4DofHrHaHfKf/Reduced directory (you can change the data directory at the beginning of the file):
a. Reads the input data from the lmt.in file inside the InputData directory
b. Calls createSpline to compute spline coefficients
c. Reads the angles inside the directory NodesData
d. Evaluate lmt and ma on this angles and write the values on *Matlab.out files
e. Reads the angles inside the directory BetweenNodesData
f. Evaluate lmt and ma on this angles and write the values on *Matlab.out files

You can evaluate the spline directly in matlab with the function evalSplineMatlab.
I.e.  [lmt ma]=evalSpline(C, evalData*pi/180); 
returns the same results of:
[lmt2 ma2]=evalSplineMatlab(C, evalData*pi/180);

