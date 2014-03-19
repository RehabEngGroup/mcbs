include_directories(
  ../SplineSrc
)

add_executable(testSpline testSpline.cpp SplineData.cpp ../SplineSrc/SplineBasisFunction.cpp)
 
