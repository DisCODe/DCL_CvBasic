# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

FIND_FILE(NONFREE_INCLUDE "opencv2/nonfree/features2d.hpp" PATHS ${OpenCV_INCLUDE_DIRS})

IF (NONFREE_INCLUDE OR ${OpenCV_VERSION} VERSION_LESS 2.4)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(CvSIFT SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(CvSIFT ${DisCODe_LIBRARIES} ${OpenCV_LIBS} )

INSTALL_COMPONENT(CvSIFT)

ELSE()

MESSAGE(STATUS "Skipping SIFT - required nonfree module not found.")

ENDIF()
