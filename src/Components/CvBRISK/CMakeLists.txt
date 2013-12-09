# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create an executable file from sources:
ADD_LIBRARY(CvBRISK SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(CvBRISK ${DisCODe_LIBRARIES} ${OpenCV_LIBS})

INSTALL_COMPONENT(CvBRISK)
