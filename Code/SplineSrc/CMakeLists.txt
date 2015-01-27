include_directories(include)

set(Spline_SOURCES SplineBasisFunction.cpp)
set(Spline_HEADERS include/mcbs/Spline.h include/mcbs/SplineBasisFunction.h)
set(Spline_IMPL include/mcbs/Spline.cpp)

source_group("Headers" FILES ${Spline_HEADERS})
source_group("Sources" FILES ${Spline_SOURCES})
source_group("Template implementations" FILES ${Spline_IMPL})

add_library(Spline ${Spline_SOURCES} ${Spline_HEADERS} ${Spline_IMPL})

set(Spline_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "Include dirs for Spline library")

install(FILES ${Spline_HEADERS} ${Spline_IMPL}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mcbs)
install(TARGETS Spline
        EXPORT mcbsTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
