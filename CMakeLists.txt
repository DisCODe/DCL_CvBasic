# Project name
PROJECT(CvBasic_Components)

# CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

FIND_PACKAGE(DisCODe REQUIRED)

set(CMAKE_MODULE_PATH ${DisCODe_DIR}/share/DisCODe ${CMAKE_MODULE_PATH})

include(DisCODeMacros)

# set default install prefix
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
      ${DisCODe_DIR} CACHE PATH "${CMAKE_PROJECT_NAME} install prefix" FORCE
  )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Sources
ADD_COMPONENT(CameraOpenCV)

# Sinks
ADD_COMPONENT(CvWindow)

# Processors
ADD_COMPONENT(CvFilter2D)
ADD_COMPONENT(CvGaussianBlur)
ADD_COMPONENT(CvPixelOperator)
ADD_COMPONENT(CvSplit)
ADD_COMPONENT(CvUndistort)
ADD_COMPONENT(CvColorConv)     
ADD_COMPONENT(CvFindChessboardCorners)
ADD_COMPONENT(CvMorphology)
ADD_COMPONENT(CvSolvePnP)
ADD_COMPONENT(CvThreshold)

