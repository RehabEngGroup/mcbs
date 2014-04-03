include_directories(${Spline_INCLUDE_DIRS})

 
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenSim) #which is the minimum required version?

if(OPENSIM_FOUND)

include_directories(${OPENSIM_INCLUDE_DIRS} ${SIMTK_INCLUDE_DIRS})
#LINK_DIRECTORIES(${OPENSIM_LIBRARY_DIRS})
 

#----------------------------------------------------

set(generateLmtMaEvalGridsWithOpenSim_SOURCES
    generateLmtMaEvalGridsWithOpenSim.cpp
    LmtMaFromOpenSim.h
    LmtMaFromOpenSim.cpp
    OpenSim2StdTools.h
    MusclesCfg.h
    MusclesCfg.cpp
)

add_executable(generateLmtMaEvalGridsWithOpenSim
    ${generateLmtMaEvalGridsWithOpenSim_SOURCES}
)

target_link_libraries(generateLmtMaEvalGridsWithOpenSim ${OPENSIM_LIBRARIES})

#----------------------------------------------------

set(generateLmtGridWithOpenSim_SOURCES
    generateLmtGridWithOpenSim.cpp
    LmtMaFromOpenSim.h
    LmtMaFromOpenSim.cpp
    OpenSim2StdTools.h
    DofCfg.h
    DofCfg.cpp
    MusclesCfg.h
    MusclesCfg.cpp
)

add_executable(generateLmtGridWithOpenSim
    ${generateLmtGridWithOpenSim_SOURCES}
)


target_link_libraries(generateLmtGridWithOpenSim ${OPENSIM_LIBRARIES}
#    osimCommon
#    osimSimulation
#    osimAnalyses
#    osimActuators
#    osimTools
#    SimTKcommon
#    SimTKsimbody
#    SimTKmath
)
endif()
#----------------------------------------------------
  
set(computeCoefficients_SOURCES
    computeCoefficients.cpp
    SplineSet.h
    SplineSet.cpp
)

add_executable(computeCoefficients 
    ${computeCoefficients_SOURCES}
)
target_link_libraries(computeCoefficients Spline)

#----------------------------------------------------
 
set(generateLmtMaWithSplineCoefficients_SOURCES
    generateLmtMaWithSplineCoefficients.cpp
    SplineSet.h
    SplineSet.cpp
    MusclesCfg.h
    MusclesCfg.cpp
)

add_executable(generateLmtMaWithSplineCoefficients
    ${generateLmtMaWithSplineCoefficients_SOURCES}
)

target_link_libraries(generateLmtMaWithSplineCoefficients Spline)

if(UNIX)
  file(COPY runFreebies.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  #maybe there is a more portable destination (CMAKE_RUNTIME_OUTPUT_DIRECTORY ?)
endif()

