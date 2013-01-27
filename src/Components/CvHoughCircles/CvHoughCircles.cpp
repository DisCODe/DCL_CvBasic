/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "CvHoughCircles.hpp"
#include "Common/Logger.hpp"

#include <opencv2/imgproc/imgproc.hpp>

namespace Processors {
namespace CvHoughCircles {

using namespace cv;
using namespace std;
using Types::Circles;

CvHoughCircles_Processor::CvHoughCircles_Processor(const std::string & name) :
	Base::Component(name), inverseRatioOfAccumulatorResolution("inverseRatioOfAccumulatorResolution"),
			minDist("minDist"), cannyHigherThreshold("cannyHigherThreshold"),
			accumulatorThreshold("accumulatorThreshold"), minCircleRadius("minCircleRadius"),
			maxCircleRadius("maxCircleRadius")
{
	registerProperty(inverseRatioOfAccumulatorResolution);
	registerProperty(minDist);
	registerProperty(cannyHigherThreshold);
	registerProperty(accumulatorThreshold);
	registerProperty(minCircleRadius);
	registerProperty(maxCircleRadius);
}

CvHoughCircles_Processor::~CvHoughCircles_Processor()
{
	LOG(LTRACE) << "Good bye CvHoughCircles_Processor\n";
}

void CvHoughCircles_Processor::prepareInterface() {
	// Register data streams, events and event handlers HERE!

	h_onNewImage.setup(this, &CvHoughCircles_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	registerStream("in_img", &in_img);
	registerStream("out_circles", &out_circles);
}

bool CvHoughCircles_Processor::onInit()
{
	LOG(LTRACE) << "CvHoughCircles_Processor::initialize\n";

	

	return true;
}

bool CvHoughCircles_Processor::onFinish()
{
	return true;
}

bool CvHoughCircles_Processor::onStop()
{
	return true;
}

bool CvHoughCircles_Processor::onStart()
{
	return true;
}

void CvHoughCircles_Processor::onNewImage()
{
	if (in_img.empty()) {
		LOG(LFATAL) << "Component " << name() << " " << "in_img input stream is empty.";
		return;
	}
	Mat image = in_img.read();

	if (image.channels() != 1) {
		LOG(LFATAL) << "Component " << name() << " " << "in_img received image must have only one channel.";
		return;
	}

	Circles c;

	HoughCircles(image, c.circles, CV_HOUGH_GRADIENT, inverseRatioOfAccumulatorResolution, minDist, cannyHigherThreshold, accumulatorThreshold, minCircleRadius, maxCircleRadius);

	out_circles.write(c);
}

}//: namespace CvHoughCircles
}//: namespace Processors
