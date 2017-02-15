set(SplineSetHelper_HEADERS include/mcbs/SplineSetBase.h
                            include/mcbs/SplineSet.h
                            include/mcbs/SplineSetCreator.h
                            include/mcbs/MusclesCfg.h
                            include/mcbs/DofCfg.h)

set(SplineSetHelper_SOURCES SplineSet.cpp
                            SplineSetCreator.cpp
                            MusclesCfg.cpp
                            DofCfg.cpp)

add_library(SplineSetHelper ${SplineSetHelper_HEADERS} ${SplineSetHelper_SOURCES})
target_link_libraries(SplineSetHelper Spline)

target_include_directories(SplineSetHelper PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(FILES ${SplineSetHelper_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mcbs)

install(TARGETS SplineSetHelper
        EXPORT mcbsTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
