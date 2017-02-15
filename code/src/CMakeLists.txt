
if(TARGET LmtMaFromOpenSim)

   # include_directories(${OPENSIMSIMBODY_INCLUDE_DIRS})
    add_executable(osimSpliner osimSplinerGeneric.cpp)

    target_link_libraries(osimSpliner LmtMaFromOpenSim SplineSetHelper Spline)

    install(TARGETS osimSpliner
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    message(STATUS "OpenSim not found, disabling compilation of osimSpliner")
endif()
