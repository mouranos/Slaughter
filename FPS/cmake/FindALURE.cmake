# - Try to find ALURE
# Once done this will define
#
#  ALURE_FOUND - system has ALURE
#  ALURE_INCLUDE_DIRS - the ALURE include directory
#  ALURE_LIBRARIES - Link these to use ALURE
#

find_path(ALURE_INCLUDE_DIR
    NAMES
      alure.h
      PATH_SUFFIXES AL OpenAL
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
  )

IF (NOT ALURE_MIN_VERSION)
  SET(ALURE_MIN_VERSION "1.1")
ENDIF(NOT ALURE_MIN_VERSION)

SET(ALURE_VERSION_OK TRUE)
IF(ALURE_INCLUDE_DIR)
    FILE(READ ${ALURE_INCLUDE_DIR}/alure.h ALURE_VERSION_CONTENT)
    STRING (REGEX MATCH "ALURE_API.*.alureUpdateInterval" ALURE_VERSION_MATCH "${ALURE_VERSION_CONTENT}")
    IF(ALURE_VERSION_MATCH)
        SET(ALURE_VERSION "1.1")
    ELSE(ALURE_VERSION_MATCH)
        SET(ALURE_VERSION_OK FALSE)
        MESSAGE(FATAL_ERROR "Can not find a recent enough version of ALURE. Please install ${ALURE_MIN_VERSION} or newer.")
    ENDIF(ALURE_VERSION_MATCH)
ENDIF(ALURE_INCLUDE_DIR)

find_library(ALURE_LIBRARY
    NAMES
    alure alure 32
    PATHS
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
)


INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ALURE_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALURE DEFAULT_MSG ALURE_LIBRARY ALURE_INCLUDE_DIR)
# show the ALURE_INCLUDE_DIRS and ALURE_LIBRARIES variables only in the advanced view
mark_as_advanced(ALURE_INCLUDE_DIR ALURE_LIBRARY)
