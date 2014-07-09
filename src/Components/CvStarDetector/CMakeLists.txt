# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

FIND_FILE(NONFREE_INCLUDE "opencv2/nonfree/features2d.hpp" PATHS ${OpenCV_INCLUDE_DIRS})
IF (${OpenCV_VERSION} VERSION_GREATER 2.3.9 AND NONFREE_INCLUDE)

# Create an executable file from sources:
ADD_LIBRARY(CvStarDetector SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(CvStarDetector ${DisCODe_LIBRARIES} ${OpenCV_LIBS})

INSTALL_COMPONENT(CvStarDetector)

ELSE ()

MESSAGE ( STATUS "Skipping STAR - required OpenCV 2.4 with nonfree module. Detected OpenCV ${OpenCV_VERSION}." )

ENDIF ()
