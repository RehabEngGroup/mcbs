#Authors: Luca Tagliapietra, Elena Ceseracciu

set(Spline_SOURCES SplineBasisFunction.cpp)

set(Spline_HEADERS include/mcbs/Spline.h
                   include/mcbs/SplineBasisFunction.h)

set(Spline_TEMPLATE_IMPLEMENTATIONS include/mcbs/Spline.cpp)

source_group("Headers" FILES ${Spline_HEADERS})
source_group("Sources" FILES ${Spline_SOURCES})
source_group("Template implementations" FILES ${Spline_TEMPLATE_IMPLEMENTATIONS})

set_source_files_properties(${Spline_TEMPLATE_IMPLEMENTATIONS} PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(Spline ${Spline_SOURCES} ${Spline_HEADERS} ${Spline_TEMPLATE_IMPLEMENTATIONS})

target_include_directories(Spline PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(FILES ${Spline_HEADERS} ${Spline_TEMPLATE_IMPLEMENTATIONS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mcbs)

install(TARGETS Spline
        EXPORT mcbsTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
