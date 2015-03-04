include_directories(${Spline_INCLUDE_DIRS})
add_definitions("/D_USE_MATH_DEFINES")

add_executable(testSpline testSpline.cpp SplineData.cpp SplineData.h)
target_link_libraries(testSpline Spline)
 
