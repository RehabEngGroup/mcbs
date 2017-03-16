# Include GNUInstallDirs to get canonical paths
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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

set(CMAKE_DEBUG_POSTFIX "_d")

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)

# Create mbcsConfig.cmake etc.
set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/mcbs)
install(EXPORT mcbsTargets
        FILE mcbsTargets.cmake
        NAMESPACE mcbs::
        DESTINATION ${ConfigPackageLocation})

include(CMakePackageConfigHelpers)
configure_package_config_file(mcbsConfig.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/mcbsConfig.cmake
                              INSTALL_DESTINATION ${ConfigPackageLocation}
                              PATH_VARS CMAKE_INSTALL_INCLUDEDIR)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/mcbsConfigVersion.cmake
                                    VERSION 1.0
                                    COMPATIBILITY ExactVersion )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mcbsConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/mcbsConfigVersion.cmake
              DESTINATION ${ConfigPackageLocation})

# Uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
