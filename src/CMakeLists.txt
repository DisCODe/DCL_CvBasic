SET(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(.)

# Find Boost, at least ver. 1.41
FIND_PACKAGE(Boost 1.41.0 REQUIRED COMPONENTS system thread filesystem date_time)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

FIND_PACKAGE( OpenCV REQUIRED )

DISCODE_FIND_DCL(CvCoreTypes)

# Find EIGEN
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


SET(DisCODe_LIBRARIES ${DisCODe_LIBRARIES} ${Boost_LIBRARIES})

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs") 

# CvBasic components
ADD_SUBDIRECTORY(Components)

ADD_SUBDIRECTORY(Types)

CONFIGURE_FILE(CvBasicConfig.cmake.in ${CMAKE_INSTALL_PREFIX}/CvBasicConfig.cmake @ONLY)
