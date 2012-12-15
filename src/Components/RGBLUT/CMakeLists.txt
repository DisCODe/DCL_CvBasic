# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create an executable file from sources:
ADD_LIBRARY(RGBLUT SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(RGBLUT ${DisCODe_LIBRARIES} )

# Link external libraries
TARGET_LINK_LIBRARIES(RGBLUT ${OpenCV_LIBS} ${DisCODe_LIBRARIES} )

INSTALL_COMPONENT(RGBLUT)
