include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(Spline_SOURCES SplineBasisFunction.cpp)
set(Spline_HEADERS Spline.h SplineBasisFunction.h)
set(Spline_IMPL Spline.cpp)

add_library(Spline ${Spline_SOURCES} ${Spline_HEADERS} ${Spline_IMPL})

set(Spline_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Include dirs for Spline library" FORCE)
