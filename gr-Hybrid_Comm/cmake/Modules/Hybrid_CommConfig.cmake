INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_HYBRID_COMM Hybrid_Comm)

FIND_PATH(
    HYBRID_COMM_INCLUDE_DIRS
    NAMES Hybrid_Comm/api.h
    HINTS $ENV{HYBRID_COMM_DIR}/include
        ${PC_HYBRID_COMM_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    HYBRID_COMM_LIBRARIES
    NAMES gnuradio-Hybrid_Comm
    HINTS $ENV{HYBRID_COMM_DIR}/lib
        ${PC_HYBRID_COMM_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/Hybrid_CommTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HYBRID_COMM DEFAULT_MSG HYBRID_COMM_LIBRARIES HYBRID_COMM_INCLUDE_DIRS)
MARK_AS_ADVANCED(HYBRID_COMM_LIBRARIES HYBRID_COMM_INCLUDE_DIRS)
