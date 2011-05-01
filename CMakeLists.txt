# Project name
PROJECT(CvBasic_Components)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(DisCODe REQUIRED)

set(CMAKE_MODULE_PATH ${DisCODe_DIR}/share/DisCODe ${CMAKE_MODULE_PATH})

include(DisCODeMacros)

# set default install prefix
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
      ${CMAKE_SOURCE_DIR}/dist CACHE PATH "${CMAKE_PROJECT_NAME} install prefix" FORCE
  )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

ADD_SUBDIRECTORY(src)
