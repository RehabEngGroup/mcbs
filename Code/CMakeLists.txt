cmake_minimum_required(VERSION 2.8)

project(multidimensionalcubicbspline)

IF(WIN32)
        SET(CMAKE_CXX_FLAGS "/EHsc")# -std=c++11 -fpermissive")
ENDIF(WIN32)
IF(UNIX)
        SET(CMAKE_BUILD_TYPE Debug)
        SET(CMAKE_CXX_FLAGS "-g -Wall")
        SET(CMAKE_EXE_LINKER_FLAGS "-g -Wall")
        SET(CMAKE_SHARED_LINKER_FLAGS "-g -Wall")
ENDIF(UNIX)


add_subdirectory(CppTest)
add_subdirectory(Freebies)
