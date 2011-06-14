SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( OpenCV REQUIRED )

FILE(GLOB h_circles *.hpp)

install(
    FILES ${h_circles}
    DESTINATION include/Types/Circles
    COMPONENT sdk
)

SET(circles Circles.cpp)
#FILE(GLOB circles *.cpp)
ADD_LIBRARY(TypesCircles SHARED ${circles})
SET_TARGET_PROPERTIES (TypesCircles PROPERTIES DEFINE_SYMBOL  "COMPILING_DLL" )
TARGET_LINK_LIBRARIES(TypesCircles ${OpenCV_LIBRARIES})

INSTALL(
  TARGETS TypesCircles
  RUNTIME DESTINATION bin COMPONENT applications
  LIBRARY DESTINATION lib COMPONENT applications
  ARCHIVE DESTINATION lib COMPONENT sdk
)
