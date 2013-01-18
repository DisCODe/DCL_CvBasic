FILE(GLOB headers *.hpp)

install(
    FILES ${headers}
    DESTINATION include/Types
    COMPONENT sdk
)


FILE(GLOB objects3d Objects3D/*.hpp)

install(
    FILES ${objects3d}
    DESTINATION include/Types/Objects3D
    COMPONENT sdk
)

add_subdirectory(Circles)
