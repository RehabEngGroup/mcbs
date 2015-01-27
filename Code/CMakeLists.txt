cmake_minimum_required(VERSION 2.8)

project(multidimensionalcubicbspline)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc ")# -fpermissive")
elseif(UNIX)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
    endif()
    option(MCBS_ENABLE_WARNINGS "Add -Wall flag" FALSE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") #this is actually needed for OpenSim...
    if(MCBS_ENABLE_WARNINGS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wall")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wall")
    endif()
endif()

add_subdirectory(SplineSrc)
add_subdirectory(CppTest)
add_subdirectory(Freebies)
