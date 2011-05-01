# Find Boost, at least ver. 1.41
FIND_PACKAGE(Boost 1.41.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# If it's necessary, add another libraries here.

# For OpenCV, uncomment following line
# FIND_PACKAGE( OpenCV REQUIRED )

# Add all components here using ADD_COMPONENT(<COMPONENT_DIRECTORY>)
ADD_COMPONENT(CvFindChessboardCorners)

ADD_COMPONENT(CvThreshold)

ADD_COMPONENT(CvUndistort)

ADD_COMPONENT(Movie)

ADD_COMPONENT(CvMorphology)

ADD_COMPONENT(CvVideoWriter)

ADD_COMPONENT(CvColorConv)

ADD_COMPONENT(CvGaussianBlur)

ADD_COMPONENT(CvSolvePnP)

ADD_COMPONENT(Sequence)

ADD_COMPONENT(CvPixelOperator)

ADD_COMPONENT(CvSplit)

ADD_COMPONENT(CvFilter2D)

ADD_COMPONENT(CameraOpenCV)

ADD_COMPONENT(CvWindow)
