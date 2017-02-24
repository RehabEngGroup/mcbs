# Examples

Most of the applications of MCBS in biomechanics go through the following steps:

Preparation:
  - 1. Compute musculotendon length (L<sub>mt</sub>, or Lmt) values on a grid of dof configurations
  - 2. Compute coefficients for splines that interpolate values at point 1.

Execution:
  - 3. Reload the coefficients to compute Lmt and moment arm (MA) values for a set of dof configurations

Optionally, for testing only:
  - 4. Compute Lmt and MA for the same dof configurations as point 3.

For each step, you have a program ready to be used in this directory.
In [src](../src) you can find a (slightly more sophisticated) program _osimSpliner_ than combines the first two steps.
Please __note__ that the programs for steps 1 and 4 are __only__ created if an OpenSim installation is found in your system.

### 1. generateLmtGridWithOpenSim ###

The generation of Lmt values is based on the OpenSim tool.

How to Run:

    generateLmtGridWithOpenSim inputDirectory outputDirectory

The __inputDirectory__ must include the following files:
* _model.osim_: a model of the subject in OpenSim format
* _muscles.in_: list of muscles (present in model.osim) for which Lmt values will be computed (one muscle per line)
* _dofAngles.cfg_: dof configurations

An example of a _dofAngles.cfg_ file follows:

    ndof 4
    ankle_angle_l -0.5 0.3 9
    knee_angle_l  -1.1 0.2 9
    hip_adduction_l -0.1 0.15 9
    hip_flexion_l -0.4 0.5 9

The first line defines the number of degrees of freedom.
For each degree of freedom a line in the file describes:
* name of the degree (should be present in model.osim)
* minimum value
* maximum value
* number of intervals

The execution of the program produces a _lmt.in_ file in the __outputDirectory__.
An example of a _lmt.in_ file is:

    ndof 4
    ankle_angle_l -0.5 0.3 9
    knee_angle_l -1.1 0.2 9
    hip_adduction_l -0.1 0.15 9
    hip_flexion_l -0.4 0.5 9
    bifemlh_l   bifemsh_l   lat_gas_l   med_gas_l
    0.367802    0.212479    0.400292    0.404358

The first five lines reproduce the information from _dofAngles.cfg_
The fifth line is the list of muscles from _muscle.in_
Then, for each dof configuration, a line reports the Lmt values for all the muscles.

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

### 3. generateLmtMaWithSplineCoefficients ###

The third program:

* sets up a set of splines based on the coefficients computed at step 2.
* reads a list of dof configurations (angles.in)
* for each dof configuration, computes musculotendon lenghts and moment arms

How to Run:

    generateLmtMaWithSplineCoefficients coeffDirectory inputDirectory  outputDirectory

The __coeffDir__ must include the files generated at step 2.

The __inputDirectory__ includes the following files:

* _muscles.in_: the list of muscles (one per line) for which Lmt and Ma will be computed. Each muscle is followed by the list of dofs that the muscle acts upon (this information is used to output only relevant muscles in MA files)
* _angles.in_: list of dof configurations

An example of _muscles.in_ is the following:

    bifemlh_l knee_angle_l hip_adduction_l hip_flexion_l
    bifemsh_l knee_angle_l
    lat_gas_l knee_angle_l ankle_angle_l
    med_gas_l knee_angle_l ankle_angle_l
    per_brev_l ankle_angle_l
    ...

An example of _angles.in_ is:

    hip_flexion_l    hip_adduction_l    knee_angle_l    ankle_angle_l
    -0.289026524    0.046774824    -0.143116999    0.1712168
    -0.29356438    0.033859387    -0.197047673    0.144164196
    -0.288502925    0.01727876    -0.265115513    0.096342175
    -0.268780705    -0.003316126    -0.35203291    0.019373155
    -0.234572251    -0.025830873    -0.454832803    -0.076096355
    -0.185877565    -0.046949357    -0.570199067    -0.169820536
    -0.124791042    -0.063180919    -0.691150384    -0.233350521
    -0.055152404    -0.071558499    -0.805643983    -0.245567826
    ...

The first line includes the names of the degrees of freedom, then each line
represents the corresponding values for a configuration.

After the execution, the following files will be created in the __outputDirectory__:

* _lmt.out_ with the musculotendon lenght of the muscle in _muscle.in_ for each configuration in _angles.in_
* a _ma*.out_ file for each degree of freedom with the moment arm of the muscle contributing to the torque at the joint.

## 4. generateLmtMaEvalGridWithOpenSim ##

This testing program provides the same output as the third program, but computes Lmt and MA through OpenSim instead of using the splines. This allows to compare the results of the OpenSim and MCBS approaches.

How to Run:

    generateLmtMaEvalGridWithOpenSim inputDirectory outputDirectory

The __inputDirectory__ includes the following files:

* _muscles.in_: the list of muscles (one per line) for which Lmt and Ma will be computed, as in program 3.
* _model.osim_: a model of the subject in OpenSim format
* _angles.in_: list of dof configurations, as in program 3.

### Simple set of data to play with ###

We have included a set of data to test all these programs.
In the [exampleData](../../data/exampleData) folder there are four directories with the same names as
the programs. In each directory you will find an Input and Output directory.
You can use these data to try out the previous procedures.

On UNIX systems, you can also use the runExamples.sh script to perform the four steps. You can then use the [Python scripts](../scripts) to compare the results obtained with programs 3 and 4.
