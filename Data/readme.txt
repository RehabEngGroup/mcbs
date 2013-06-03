This directory includes data to test the multidimensional cubic BSpline.
Data refers to a 4DOF problem including
- Ha: hip adduction-abduction
- Hf: hip flexion-extension
- Hr: Hip internal-external rotation
- Kf: Knee flexion-extension 

The Extension test data set is larger than the Reduced test data set.

Each data set includes three directories:
- InputData: the file lmt.in includes the equidistant nodes that are input, together with the experimental musculotendon length computed by OpenSim at the selected nodes. This is the ONLY input file used to compute spline coefficients.
- NodesData and BetweenNodesData: angles.in are the angles where lmt and ma will be computed by the test, lmt.in, maHa.in maHf.in maHr.in, and MaKf.in are the target values that should be matched by the interpolation with the splines. After the running of the program several new .out files will be available with the results of the test.
