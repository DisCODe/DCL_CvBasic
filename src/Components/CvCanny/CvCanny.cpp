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
	h_onNewImage.setup(this, &CvCanny::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
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
		cv::Mat gray;
		//cvtColor(in, gray, COLOR_BGR2GRAY);

		// Create a matrix with the adequate size and depth.
		cv::Mat out;
		out.create(in.size(), CV_8U);

		// Canny edge detector.
		Canny( in, out, lowerThreshold, higherThreshold, kernelSize );

		// Use canny as mask
		//dst = Scalar::all(0);
		//src.copyTo( dst, detected_edges);

		// Write the result to the output.
		out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "CvCanny::onNewImage failed\n";
	}
}



} //: namespace CvCanny
} //: namespace Processors
