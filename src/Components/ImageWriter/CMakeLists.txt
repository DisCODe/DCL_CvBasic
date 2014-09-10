# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(ImageWriter SHARED ${files})

# Add aother libraries if necessary, for example:
TARGET_LINK_LIBRARIES(ImageWriter ${DisCODe_LIBRARIES} ${OpenCV_LIBS} )

INSTALL_COMPONENT(ImageWriter)
