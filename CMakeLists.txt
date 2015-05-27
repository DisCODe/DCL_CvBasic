# Project name
PROJECT(CvBasic)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Find DisCODe installation
FIND_PACKAGE(DisCODe REQUIRED)

# Use CMake macros provided with DisCODe
set(CMAKE_MODULE_PATH ${DisCODe_DIR} ${CMAKE_MODULE_PATH})
include(DisCODeMacros)

# Set default install prefix to DisCODe location
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
      ${CMAKE_SOURCE_DIR}/dist CACHE PATH "${CMAKE_PROJECT_NAME} install prefix" FORCE
  )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(data)

REBUILD_DCL_CACHE()

