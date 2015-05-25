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
	registerHandler("onNewImage", boost::bind(&CvUndistort_Processor::onNewImage, this));
	addDependency("onNewImage", &in_img);
	addDependency("onNewImage", &in_camera_info);

	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	registerStream("in_camera_info", &in_camera_info);
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

	return true;
}

bool CvUndistort_Processor::onFinish()
{
	return true;
}

void CvUndistort_Processor::onNewImage()
{
	cv::Mat originalImage;
	originalImage = in_img.read();
	
	Types::CameraInfo ci = in_camera_info.read();
	
	if ( (ci != camera_info) || (last_alpha != alpha) ) {
		camera_info = ci;
		last_alpha = alpha;
		CLOG(LINFO) << "New camera info!";
		
		if (stereo) {
			cv::initUndistortRectifyMap(camera_info.cameraMatrix(), camera_info.distCoeffs(), camera_info.rectificationMatrix(), camera_info.projectionMatrix(), originalImage.size(), CV_32FC1, map1, map2);
		} else {
			newK = cv::getOptimalNewCameraMatrix(camera_info.cameraMatrix(), camera_info.distCoeffs(), originalImage.size(), 0.01 * alpha);
			cv::initUndistortRectifyMap(camera_info.cameraMatrix(), camera_info.distCoeffs(), cv::Mat(), newK, originalImage.size(), CV_32FC1, map1, map2);
		}
	}

	//cv::Mat undistortedImage = originalImage.clone();
	cv::Mat undistortedImage;
	
	remap(originalImage, undistortedImage, map1, map2, interpolation);
	
	//undistort(originalImage, undistortedImage, camera_info.cameraMatrix(), camera_info.distCoeffs());

	out_img.write(undistortedImage.clone());
}

} // namespace CvUndistort

} // namespace Processors
