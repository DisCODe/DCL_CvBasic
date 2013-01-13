/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvSIFT.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvSIFT {

CvSIFT::CvSIFT(const std::string & name) :
		Base::Component(name)  {

}

CvSIFT::~CvSIFT() {
}

void CvSIFT::prepareInterface() {
	// Register handlers with their dependencies.
	h_onNewImage.setup(this, &CvSIFT::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
}

bool CvSIFT::onInit() {

	return true;
}

bool CvSIFT::onFinish() {
	return true;
}

bool CvSIFT::onStop() {
	return true;
}

bool CvSIFT::onStart() {
	return true;
}

void CvSIFT::onNewImage()
{
	LOG(LTRACE) << "CvSIFT::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read();

	    cv::SiftFeatureDetector detector;
	    std::vector<cv::KeyPoint> keypoints;
	    detector.detect(input, keypoints);

	    // Add results to image and save.
	    cv::Mat output;
	    cv::drawKeypoints(input, keypoints, output);

		// Write the result to the output.
		out_img.write(output);
	} catch (...) {
		LOG(LERROR) << "CvSIFT::onNewImage failed\n";
	}
}


} //: namespace CvSIFT
} //: namespace Processors
