SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( OpenCV REQUIRED )

FILE(GLOB h_circles *.hpp)

install(
    FILES ${h_circles}
    DESTINATION include/Types/Circles
    COMPONENT sdk
)
