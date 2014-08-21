/*
 * CvUndistort_Processor.cpp
 *
 *  Created on: Oct 20, 2010
 *      Author: mboryn
 */

#include "CvUndistort_Processor.hpp"

namespace Processors {

namespace CvUndistort {

using namespace std;
using namespace cv;
using namespace boost;
using namespace Base;

CvUndistort_Processor::CvUndistort_Processor(const std::string& n) :
	Component(n)
{

}

CvUndistort_Processor::~CvUndistort_Processor()
{
}

void CvUndistort_Processor::prepareInterface() {
	h_onNewImage.setup(this, &CvUndistort_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
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
bool CvUndistort_Processor::onStep()
{
	return true;
}

void CvUndistort_Processor::onNewImage()
{
	cv::Mat originalImage;

	originalImage = in_img.read();
	camera_info = in_camera_info.read();

	//cv::Mat undistortedImage = originalImage.clone();
	cv::Mat undistortedImage;

	// TODO: replace with initUndistortRectifyMap + remap
	//initUndistortRectifyMap(props.cameraMatrix, props.distCoeffs, Mat(), );
	//remap(originalImage, undistortedImage, map1, map2, interpolation);
	undistort(originalImage, undistortedImage, camera_info.cameraMatrix(), camera_info.distCoeffs());

	out_img.write(undistortedImage.clone());
}

} // namespace CvUndistort

} // namespace Processors
