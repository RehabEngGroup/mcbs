include_directories(
    ${CMAKE_SOURCE_DIR}/SplineSrc/
)
 
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/Freebies)

find_package(OpenSim REQUIRED)
include_directories(${OPENSIM_INCLUDE_DIRS} ${SIMTK_INCLUDE_DIRS})
#LINK_DIRECTORIES(${OPENSIM_LIBRARY_DIRS})
 

#----------------------------------------------------

set(generateLmtMaEvalGridsWithOpenSim_SOURCES
    generateLmtMaEvalGridsWithOpenSim.cpp
    LmtMaFromOpenSim.cpp
    OpenSim2StdTools.cpp
    DofCfg.cpp
)

add_executable(generateLmtMaEvalGridsWithOpenSim
    ${generateLmtMaEvalGridsWithOpenSim_SOURCES}
)

target_link_libraries(generateLmtMaEvalGridsWithOpenSim ${OPENSIM_LIBRARIES})

#----------------------------------------------------

set(generateLmtGridWithOpenSim_SOURCES
    generateLmtGridWithOpenSim.cpp
    LmtMaFromOpenSim.cpp
    OpenSim2StdTools.cpp
    DofCfg.cpp
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

#----------------------------------------------------
  
set(computeCoefficients_SOURCES
    computeCoefficients.cpp
    SplineSet.cpp
    ../SplineSrc/SplineBasisFunction.cpp
)

add_executable(computeCoefficients 
    ${computeCoefficients_SOURCES}
)

#----------------------------------------------------
 
set(generateLmtMaWithSplineCoefficients_SOURCES
    generateLmtMaWithSplineCoefficients.cpp
    SplineSet.cpp
    ${CMAKE_SOURCE_DIR}/SplineSrc/SplineBasisFunction.cpp
)

add_executable(generateLmtMaWithSplineCoefficients
    ${generateLmtMaWithSplineCoefficients_SOURCES}
)

if(UNIX)
  file(COPY runFreebies.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  #maybe there is a more portable destination (CMAKE_RUNTIME_OUTPUT_DIRECTORY ?)
endif()

