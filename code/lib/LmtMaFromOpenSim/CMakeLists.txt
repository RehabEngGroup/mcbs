#Authors: Luca Tagliapietra, Elena Ceseracciu
find_package(OpenSim)

if(OPENSIM_FOUND)

set(LmtMa_SOURCES LmtMaFromOpenSim.cpp)

set(LmtMa_HEADERS LmtMaFromOpenSim.h)

source_group("Headers" FILES ${LmtMa_HEADERS})
source_group("Sources" FILES ${LmtMa_SOURCES})

add_library(LmtMaFromOpenSim STATIC ${LmtMa_SOURCES} ${LmtMa_HEADERS})

target_include_directories(LmtMaFromOpenSim PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> ${OPENSIMSIMBODY_INCLUDE_DIRS})
target_link_libraries(LmtMaFromOpenSim ${OPENSIMSIMBODY_LIBRARIES})

else()
    message(STATUS "OpenSim not found, disabling compilation of OpenSim related executables")
endif()


