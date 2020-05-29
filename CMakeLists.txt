########################################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
# @author Remi Ete, DESY
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 FATAL_ERROR )
########################################################

# project name
PROJECT( SIO )
 
# project version
SET( SIO_VERSION_MAJOR 0 )
SET( SIO_VERSION_MINOR 0 )
SET( SIO_VERSION_PATCH 2 )

### DEPENDENCIES ############################################################

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

# options
OPTION( SIO_EXAMPLES                 "Switch ON to build SIO examples" ON )
OPTION( INSTALL_DOC                  "Set to OFF to skip build/install Documentation" OFF )
OPTION( SIO_RELEASE_OFAST            "Set to ON to set CMAKE_CXX_FLAGS_RELEASE to -Ofast (else to -O3)" OFF )
OPTION( SIO_PROFILING                "Set to ON to enable SIO profiling (-pg)" OFF )
OPTION( SIO_BUILTIN_ZLIB             "Set to OFF to use system zlib library" ON )
OPTION( SIO_MACROS_WITH_EXCEPTION    "Set to ON to enable try/catch handling in SIO macros" OFF )
SET(    SIO_LOGLVL                   "0" CACHE STRING "The SIO verbosity level" )

IF( NOT SIO_LOGLVL MATCHES "^[0-9]+$" )
  MESSAGE( SEND_ERROR "SIO log level variable not a number (${SIO_LOGLVL})" )
ENDIF()

# ZLIB package
IF( SIO_BUILTIN_ZLIB )
  ADD_SUBDIRECTORY( zlib )
ELSE()
  FIND_PACKAGE( ZLIB REQUIRED )
ENDIF()

INCLUDE_DIRECTORIES( SYSTEM ${ZLIB_INCLUDE_DIR} )

# load SIO build settings
INCLUDE( SIOBuild )

# sio library
ADD_SUBDIRECTORY( source )

### DOCUMENTATION ###########################################################
IF( INSTALL_DOC )
    ADD_SUBDIRECTORY( ./doc )
    INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} doc)" )
ENDIF()

# display some variables and write them to cache
SIO_DISPLAY_STD_VARIABLES()

# generate and install following configuration files
SIO_GENERATE_PACKAGE_CONFIGURATION_FILES( SIOConfig.cmake SIOConfigVersion.cmake )
