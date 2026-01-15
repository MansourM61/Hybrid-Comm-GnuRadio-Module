INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_RF_COMM RF_Comm)

FIND_PATH(
    RF_COMM_INCLUDE_DIRS
    NAMES RF_Comm/api.h
    HINTS $ENV{RF_COMM_DIR}/include
        ${PC_RF_COMM_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    RF_COMM_LIBRARIES
    NAMES gnuradio-RF_Comm
    HINTS $ENV{RF_COMM_DIR}/lib
        ${PC_RF_COMM_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/RF_CommTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RF_COMM DEFAULT_MSG RF_COMM_LIBRARIES RF_COMM_INCLUDE_DIRS)
MARK_AS_ADVANCED(RF_COMM_LIBRARIES RF_COMM_INCLUDE_DIRS)
