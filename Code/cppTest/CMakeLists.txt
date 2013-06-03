cmake_minimum_required(VERSION 2.6)

include_directories(
  ../src
)

add_executable(testSpline testSpline.cpp SplineData.cpp ../src/SplineBasisFunction.cpp)
 
