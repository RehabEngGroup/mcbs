
if(TARGET LmtMaFromOpenSim)
    add_executable(generateLmtMaEvalGridsWithOpenSim generateLmtMaEvalGridsWithOpenSim.cpp)
    target_link_libraries(generateLmtMaEvalGridsWithOpenSim LmtMaFromOpenSim SplineSetHelper Spline)

    add_executable(generateLmtGridWithOpenSim generateLmtGridWithOpenSim.cpp)
    target_link_libraries(generateLmtGridWithOpenSim LmtMaFromOpenSim SplineSetHelper Spline)
endif()

add_executable(computeCoefficients computeCoefficients.cpp)
target_link_libraries(computeCoefficients SplineSetHelper Spline)

add_executable(generateLmtMaWithSplineCoefficients generateLmtMaWithSplineCoefficients.cpp)
target_link_libraries(generateLmtMaWithSplineCoefficients SplineSetHelper Spline)

if(UNIX)
    file(COPY runExamples.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
     #maybe there is a more portable destination (CMAKE_RUNTIME_OUTPUT_DIRECTORY ?)
endif()
