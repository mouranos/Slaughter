# Try to find leveldb
# Once done, this will define
#
# LEVELDB_FOUND       - system has leveldb
# LEVELDB_INCLUDE_DIR - the leveldb include directory
# LEVELDB_LIB_DIR     - the leveldb library directory
#
# LEVELDB_DIR may be defined as a hint for where to look
# LEVELDB_LIBRARIES   - the level db library name(s)

find_path(LEVELDB_INCLUDE_DIR leveldb/db.h
    HINTS
    ${LEVELDB_INCLUDE_DIR}
    $ENV{LEVELDB_INCLUDE_DIR}
    /usr
    /usr/local
    /opt/
    PATH_SUFFIXES include/
    PATHS /opt
    )

find_library(LEVELDB_LIBRARY leveldb
    HINTS
    ${LEVELDB_DIR}
    $ENV{LEVELDB_DIR}
    /usr
    /usr/local
    /opt/leveldb/
    PATH_SUFFIXES lib
    )


set(LEVELDB_LIBRARIES ${LEVELDB_LIBRARY})

get_filename_component( LEVELDB_LIB_DIR ${LEVELDB_LIBRARY} PATH )
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(leveldb DEFAULT_MSG LEVELDB_LIB_DIR LEVELDB_INCLUDE_DIR )
