# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

IF (${OpenCV_VERSION} VERSION_GREATER 2.3.9)

# Create an executable file from sources:
ADD_LIBRARY(CvORB SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(CvORB ${DisCODe_LIBRARIES} ${OpenCV_LIBS})

INSTALL_COMPONENT(CvORB)

ELSE ()

MESSAGE ( STATUS "Skipping ORB - required OpenCV 2.4. Detected OpenCV ${OpenCV_VERSION}." )

ENDIF ()
