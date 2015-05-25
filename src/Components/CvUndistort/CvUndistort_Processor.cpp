/*
 * CvUndistort_Processor.cpp
 *
 *  Created on: Oct 20, 2010
 *      Author: mboryn
 */

#include "CvUndistort_Processor.hpp"

#include <opencv2/opencv.hpp>

namespace Processors {

namespace CvUndistort {

using namespace std;
using namespace cv;
using namespace boost;
using namespace Base;

CvUndistort_Processor::CvUndistort_Processor(const std::string& n) :
	Component(n),
	alpha("alpha", 0, "range"),
	stereo("stereo", false)
{
	registerProperty(alpha);
	last_alpha = -1;
	
	registerProperty(stereo);
}

CvUndistort_Processor::~CvUndistort_Processor()
{
}

void CvUndistort_Processor::prepareInterface() {
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	registerStream("in_camera_info", &in_camera_info);

	registerHandler("onNewImage", boost::bind(&CvUndistort_Processor::onNewImage, this));
	addDependency("onNewImage", &in_img);
	addDependency("onNewImage", &in_camera_info);

}

bool CvUndistort_Processor::onStart()
{
	return true;
}

bool CvUndistort_Processor::onStop()
{
	return true;
}

bool CvUndistort_Processor::onInit()
{
	interpolation = INTER_LINEAR;

/*	// Initialize rectify matrices...
	if (stereo) {
		// ... with variables (including image size) taken from camera_info (!).
		cv::initUndistortRectifyMap(camera_info.cameraMatrix(), camera_info.distCoeffs(), camera_info.rectificationMatrix(), camera_info.projectionMatrix(), camera_info.size(), CV_32FC1, map1, map2);
	} else {
		newK = cv::getOptimalNewCameraMatrix(camera_info.cameraMatrix(), camera_info.distCoeffs(), camera_info.size(), 0.01 * alpha);
		cv::initUndistortRectifyMap(camera_info.cameraMatrix(), camera_info.distCoeffs(), cv::Mat(), newK, camera_info.size(), CV_32FC1, map1, map2);
	}
*/
	return true;
}

bool CvUndistort_Processor::onFinish()
{
	return true;
}

void CvUndistort_Processor::onNewImage()
{
	cv::Mat originalImage, undistortedImage;

	// Read input image and camera info.
	originalImage = in_img.read();
	Types::CameraInfo ci = in_camera_info.read();
	
	// Check if camera info was changed.
	if ( (ci != camera_info) || (last_alpha != alpha) ) {
		CLOG(LINFO) << "New camera info!";
		camera_info = ci;
		last_alpha = alpha;

		// Reinitialize rectify matrices.
		if (stereo) {
			cv::initUndistortRectifyMap(camera_info.cameraMatrix(), camera_info.distCoeffs(), camera_info.rectificationMatrix(), camera_info.projectionMatrix(), originalImage.size(), CV_32FC1, map1, map2);
		} else {
			newK = cv::getOptimalNewCameraMatrix(camera_info.cameraMatrix(), camera_info.distCoeffs(), originalImage.size(), 0.01 * alpha);
			cv::initUndistortRectifyMap(camera_info.cameraMatrix(), camera_info.distCoeffs(), cv::Mat(), newK, originalImage.size(), CV_32FC1, map1, map2);
		}
	}//: if

	// Remap (undistort & rectify) input image.
	remap(originalImage, undistortedImage, map1, map2, interpolation);
//	undistort(originalImage, undistortedImage, camera_info.cameraMatrix(), camera_info.distCoeffs());

	out_img.write(undistortedImage.clone());
}

} // namespace CvUndistort

} // namespace Processors
