# Freebies :) 

Start working with MCBS library can be a little tricky: everything 
is in C++ and we love efficient solutions that sometimes are too nerdy :) 
In the first release of the library, we included a couple of test programs
(in matlab and C++). They are great and still included in this distribution
but they can be a little difficult to deal with... 

With this new release, we have included several freebies. 
They are our presents for people that want to use MCBS but need solutions 
ready to be used.

Please help us in increasing the number of freebies in this directory: 
either send us your software if you think that it can be useful for other
developers, or send us ideas about new freebies that you would like to 
be included.

Now, sit down and enjoy...

## Three little programs cover most of your needs ##
Most of the applications of MCBS goes through the following three steps:

* Before:
  1. Compute musculotendon lenght values on a grid of dof configurations
  2. Compute coefficients for splines that interpolate values at point 1. 
  Compute coefficients for splines to interpolate the lmt values
* After:
  3. Reload the coefficients to compute lmt and ma values for a set of dof configurations

For each step, you have a program ready to be used in this directory. 

### 1. generateLmtGridWithOpenSim ###
The generation of Lmt values is based on the OpenSim tool. 

How to Run:

    generateLmtGridWithOpenSim inputDirectory outputDirectory

The __inputDirectory__ must include the following files:
* _model.osim_: a scaled model of the subject in OpenSim format
* _muscle.in_: list of muscles whose lmt values will be computed
* _dofAngles.cfg_: dof configurations 

An example of a _dofAngles.cfg_ file follows:

    ndof 4 
    R_knee_fe       -65     10      9
    R_hip_aa        -35     0       9
    R_hip_fe        -15     50      9
    R_hip_rot       -50     -10     9

The first line define the number of degrees of freedom. 
For each degree of freedom a line in the file describe:
* name of the degree 
* minimum value
* maximum value
* number of intervals 

The execution of the program produces a _lmt.in_ file in the __outputDirectory__.
An example of a _lmt.in_ file is:

    R_knee_fe -65 10 9
    R_hip_aa -35 0 9
    R_hip_fe -15 50 9
    R_hip_rot -50 -10 9
    addbrev_r addlong_r addmag1_r addmag2_r addmag3_r ....
    0.161533        0.248868        0.145215        0.222197       ...
    ...
    
The first four lines, reproduce the information from _dofAngles.cfg_ 
The fifth line is the list of muscles from _muscle.in_ 
Then, for each dof configuration, a line reports the lmt values for all the muscles.

### 2. computeCoefficients ###
The generation of the spline coefficients is based on the algorithm described in: 
_M. Sartori, M. Reggiani, A. J. van den Bogert, and D. G. Lloyd. Estimation of musculoten-
don kinematics in large musculoskeletal models using multidimensional B-splines. Journal of
Biomechanics, 45(3):595-601, February 2012._

How to Run:

    computeCoefficients inputDirectory outputDirectory
    
The __inputDirectory__ must includes only an lmt.in file.
If the file is not produced with generateLmtGridWithOpenSim, 
please follow the format previously described.

After the execution, the following files will be created in the __outputDirectory__

* for each muscle, a binary file storing the coefficients of the interpolating spline
* a _dofAngles.cfg_ (required for step 3)

### 3. compute lmt and ma for a set of dof configurations ###
The third program:

* setups a set of splines based on the coefficients computed at step 2.
* read a list of dof configurations (angles.in)
* for each dof configuration compute musculotendon lenghts and moment arms

How to Run: 
    
    generateLmtMaWithSplineCoefficients coeffDirectory inputDirectory  outputDirectory
    
The __coeffDir__ must include the files generated at step 2.

The __inputDirectory__ includes the following files:

* _muscle.in_: the list of muscles whose lmt will be computed
* _dofs.in_: for each dof, the list of muscles contributing to its torque
* _angles.in_: list of dof configurations 

An example of _dofs.in_ is the following:

    R_knee_fe bicfemlh_r  bicfemsh_r  gaslat_r  gasmed_r  gra_r  recfem_r  sar_r  semimem_r  semiten_r  tfl_r  vasint_r  vaslat_r  vasmed_r
    R_hip_aa addbrev_r  addlong_r  addmag1_r  addmag2_r  addmag3_r  bicfemlh_r  gmax1_r  gmax2_r  gmax3_r  gmed1_r  gmed2_r  gmed3_r  gmin1_r  gmin2_r  gmin3_r  gra_r  illiacus_r  psoas_r  recfem_r  sar_r  semimem_r  semiten_r  tfl_r
    R_hip_fe addbrev_r  addlong_r  addmag1_r  addmag2_r  addmag3_r  bicfemlh_r  gmax1_r  gmax2_r  gmax3_r  gmed1_r  gmed2_r  gmed3_r  gmin1_r  gmin2_r  gmin3_r  gra_r  illiacus_r  psoas_r  recfem_r  sar_r  semimem_r  semiten_r  tfl_r
    R_hip_rot addbrev_r addlong_r addmag1_r addmag2_r addmag3_r bicfemlh_r gmax1_r gmax2_r gmax3_r gmed1_r gmed2_r gmed3_r gmin1_r gmin2_r gmin3_r gra_r illiacus_r psoas_r recfem_r sar_r semimem_r semiten_r tfl_r

each line includes the name of the degree of freedom followed by 
the list of muscle names.

An example of _angles.in_ is:
    10000
    -50 -15 -35 -65  
    -50 -15 -35 -56.6667 
    -50 -15 -35 -48.3333 
    -50 -15 -35 -40  
    ...
    
First line includes the number of configurations, then each line 
includes the dof values for each degree of freedom in the following order:

    R_hip_rot R_hip_fe R_hip_aa R_knee_fe
    
After the execution, the following files will be created in the __outputDirectory__:

* lmt.out with the musculotendon lenght of the muscle in _muscle.in_ for each configuration in _angles.in_
* a ma*.out file for each degree of freedom with the moment arm of the muscle contributing to the torque at the joint.

### Simple set of data to play  ###
We have included a set of data in the release to test the previous three programs.
In the _Data/DataForFreebies_ there are three directories with the same names of 
the three programs. In each directory you will find an Input and Output directory. 
You can use this data to try the previous procedures.

    
## A program to test spline behavior ##
Another program included in the directory compute Lmt and Ma with the OpenSim tools. 
The objective is to build exactly the same data and compare the results of OpenSim 
and Spline approaches.

So far, we have implemented only the Lmt computation... stay tuned