/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvCanny.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvCanny {

CvCanny::CvCanny(const std::string & name) :
		Base::Component(name),
		lowerThreshold("histeresis.lowerThreshold", 50, "range"),
		higherThreshold("histeresis.higherThreshold", 150, "range"),
		kernelSize("kernelSize", 3)
{
	// Constraints.
	lowerThreshold.addConstraint("0");
	lowerThreshold.addConstraint("100");
	higherThreshold.addConstraint("0");
	higherThreshold.addConstraint("300");

	// Register properties.
	registerProperty(lowerThreshold);
	registerProperty(higherThreshold);
	registerProperty(kernelSize);
}

CvCanny::~CvCanny() {
}

void CvCanny::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvCanny::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("in_mask", &in_mask);
	registerStream("out_img", &out_img);
}

bool CvCanny::onInit() {

	return true;
}

bool CvCanny::onFinish() {
	return true;
}

bool CvCanny::onStop() {
	return true;
}

bool CvCanny::onStart() {
	return true;
}


void CvCanny::onNewImage()
{
	LOG(LTRACE) << "CvCanny::onNewImage\n";
	try {
		cv::Mat in = in_img.read();
				
		//cvtColor(in, gray, COLOR_BGR2GRAY);

		// Create a matrix with the adequate size and depth.
		cv::Mat dst = cv::Mat::zeros(in.size(), CV_8U);

		// Canny edge detector.
		Canny( in, dst, lowerThreshold, higherThreshold, kernelSize );

		// Use mask if provided
		if (!in_mask.empty()) {
			cv::Mat mask = in_mask.read();
			cv::Mat tmp = cv::Mat::zeros(in.size(), CV_8U);
			dst.copyTo(tmp, mask);
			dst = tmp;
		}

		// Write the result to the output.
		out_img.write(dst);
	} catch (...) {
		LOG(LERROR) << "CvCanny::onNewImage failed\n";
	}
}



} //: namespace CvCanny
} //: namespace Processors
