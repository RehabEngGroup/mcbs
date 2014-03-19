# - Try to find OpenSim and SimTK
# Once done this will define
#  OPENSIM_FOUND - System has OpenSim
#  OPENSIM_INCLUDE_DIRS - The OpenSim include directories
#  SIMTK_INCLUDE_DIRS - The SimTK include directories
#  OPENSIM_LIBRARIES - The libraries needed to use Opensim
#  OPENSIM_DEFINITIONS - Compiler switches required for using OpenSim


set(OPENSIM_POSSIBLE_DIRECTORIES 
	~/Programs/OpenSim
	~/ThirdPartyPrograms/OpenSim
	$ENV{OPENSIM_HOME}
	"C:/OpenSim\ 3.0/"
	)

find_path(OPENSIM_INCLUDE_DIRS OpenSim/OpenSim.h
          PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES sdk/include )
MESSAGE( STATUS "OpenSim Include path: ${OPENSIM_INCLUDE_DIRS}")

find_path(SIMTK_INCLUDE_DIRS Simbody.h
          PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES sdk/include/SimTK/include)
#MESSAGE( STATUS "SimTK Include path: ${SIMTK_INCLUDE_DIRS}")
		  
find_path(OPENSIM_LIBS_DIR 
		  NAMES
			  libosimCommon.so
        PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
        PATH_SUFFIXES lib)
		  
find_path(OPENSIM_DLL_DIR 
		  NAMES
			  osimCommon.dll
          PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES bin)

SET(OPENSIM_LIBRARY_DIRS ${OPENSIM_LIBS_DIR} )
#${OPENSIM_DLL_DIR})

MESSAGE( STATUS "OpenSim Lib path: ${OPENSIM_LIBRARY_DIRS}")

SET(OPENSIM_ALL_LIBS
	debug osimSimulation_d	optimized osimSimulation
	debug osimActuators_d	optimized osimActuators
	debug osimCommon_d		optimized osimCommon
	debug osimAnalyses_d	optimized osimAnalyses
	debug osimTools_d		optimized osimTools
	debug ${NameSpace}SimTKcommon_d optimized ${NameSpace}SimTKcommon
	debug ${NameSpace}SimTKmath_d optimized  ${NameSpace}SimTKmath
	debug ${NameSpace}SimTKsimbody_d optimized ${NameSpace}SimTKsimbody
	SimTKlapack
	${PLATFORM_LIBS}
)

IF(WIN32)
	SET(PLATFORM_LIBS  pthreadVC2)
ELSE (WIN32)
  SET(NameSpace "")
  IF(APPLE)
	SET(PLATFORM_LIBS  SimTKAtlas)
  ELSE(APPLE)
	SET(PLTAFORM_LIBS SimTKAtlas_Lin_generic)
  ENDIF(APPLE)
ENDIF (WIN32)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set OPENSIM_FOUND to TRUE
# if all listed variables are TRUE
# DEFAULT_MSG is predefined... change only if you need a custom msg
find_package_handle_standard_args(OpenSim DEFAULT_MSG
                                  OPENSIM_LIBRARY_DIRS OPENSIM_INCLUDE_DIRS SIMTK_INCLUDE_DIRS)

# Mark the following variables as advanced (they are not displayed in any 
# of the cmake GUIs unless the show advanced option is on.
mark_as_advanced(LIBXML2_INCLUDE_DIR LIBXML2_LIBRARY )


