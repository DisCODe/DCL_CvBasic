FILE(GLOB headers *.hpp)

install(
    FILES ${headers}
    DESTINATION include/Types
    COMPONENT sdk
)

