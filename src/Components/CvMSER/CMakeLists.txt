# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

IF (${OpenCV_VERSION} VERSION_GREATER 2.3.9)

# Create an executable file from sources:
ADD_LIBRARY(CvMSER SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(CvMSER ${DisCODe_LIBRARIES} ${OpenCV_LIBS})

INSTALL_COMPONENT(CvMSER)

ELSE ()

MESSAGE ( STATUS "Skipping MSER - required OpenCV 2.4. Detected OpenCV ${OpenCV_VERSION}." )

ENDIF ()