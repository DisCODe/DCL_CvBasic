# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(ImageWriter SHARED ${files})

FIND_PACKAGE(Boost REQUIRED COMPONENTS date_time)
 
# Add aother libraries if necessary, for example:
TARGET_LINK_LIBRARIES(ImageWriter ${Boost_DATE_TIME_LIBRARY} ${OpenCV_LIBS} ${DisCODe_LIBRARIES} )

INSTALL_COMPONENT(ImageWriter)
