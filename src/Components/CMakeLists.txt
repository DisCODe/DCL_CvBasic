FIND_PACKAGE(Boost 1.41.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Sources
ADD_COMPONENT(CameraOpenCV)
ADD_COMPONENT(Movie)
ADD_COMPONENT(Sequence)

# Sinks
ADD_COMPONENT(CvWindow)
ADD_COMPONENT(CvVideoWriter)

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

