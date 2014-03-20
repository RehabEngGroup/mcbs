#!/bin/bash

EXPECTED_ARGS=2

if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` ProgramDirectory DataDirectory"
  exit
fi




PROGRAM_DIRECTORY=$1
DATA_DIRECTORY=$2
PROGRAM_NAME="/generateLmtGridWithOpenSim"


if [ ! -d "$DATA_DIRECTORY" ]; then
  echo "$DATA_DIRECTORY does not exists"
  exit 
fi

if [ ! -d $DATA_DIRECTORY$PROGRAM_NAME ]; then
  echo "$DATA_DIRECTORY does not include a set of directory to test FreeBies"
  exit
fi

if [ ! -d "$PROGRAM_DIRECTORY" ]; then
  echo "$PROGRAM_DIRECTORY does not exists"
  exit 
fi

if [ ! -f $PROGRAM_DIRECTORY$PROGRAM_NAME ]; then
  echo "$PROGRAM_DIRECTORY does not include the set of FreeBies programs"
  exit
fi


echo "==================================================="
echo "           1. generate Lmt Grid with OpenSim"
echo "==================================================="

$PROGRAM_DIRECTORY/generateLmtGridWithOpenSim $DATA_DIRECTORY/generateLmtGridWithOpenSim/Input/ $DATA_DIRECTORY/generateLmtGridWithOpenSim/Output/


echo "==================================================="
echo "           2. compute Spline Coefficients"
echo "==================================================="
cp $DATA_DIRECTORY/generateLmtGridWithOpenSim/Output/lmt.in $DATA_DIRECTORY/computeCoefficients/Input/

$PROGRAM_DIRECTORY/computeCoefficients $DATA_DIRECTORY/computeCoefficients/Input/ $DATA_DIRECTORY/computeCoefficients/Output/


echo "==================================================="
echo "           3. generate Lmt and Ma with Splines"
echo "==================================================="

$PROGRAM_DIRECTORY/generateLmtMaWithSplineCoefficients $DATA_DIRECTORY/computeCoefficients/Output/ $DATA_DIRECTORY/generateLmtMaWithSplineCoefficients/Input/ $DATA_DIRECTORY/generateLmtMaWithSplineCoefficients/Output/


echo "==================================================="
echo "  4. generate Lmt with OpenSim (ma still missing)"
echo "==================================================="

$PROGRAM_DIRECTORY/generateLmtMaEvalGridsWithOpenSim $DATA_DIRECTORY/generateLmtMaEvalGridsWithOpenSim/Input/ $DATA_DIRECTORY/generateLmtMaEvalGridsWithOpenSim/Output/


echo "==============================================================="
echo "You can compare the lmt values produced with Spline and OpenSim"
echo "with the the python program in Code/PythonScripts/compare.py"
echo "copy the lmt.in generated with OpenSim"
echo " and the lmt.out generated with Splines"
echo "in the PythonScripts directory and run"
echo "python compare.py"
echo "==============================================================="
