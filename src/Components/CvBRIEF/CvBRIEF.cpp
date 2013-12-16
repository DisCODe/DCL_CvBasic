/*!
 * \file
 * \brief BRIEF
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvBRIEF.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvBRIEF {

CvBRIEF::CvBRIEF(const std::string & name) :
        Base::Component(name)
		{
			// Register properties.


		}

CvBRIEF::~CvBRIEF() {
}

void CvBRIEF::prepareInterface() {
	// Register handlers with their dependencies.
	h_onNewImage.setup(this, &CvBRIEF::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvBRIEF::onInit() {

	return true;
}

bool CvBRIEF::onFinish() {
	return true;
}

bool CvBRIEF::onStop() {
	return true;
}

bool CvBRIEF::onStart() {
	return true;
}

void CvBRIEF::onNewImage()
{
	LOG(LTRACE) << "CvBRIEF::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat img = in_img.read();
		cv::Mat gray;
		//cvtColor(img,yuv, COLOR_BGR2YCrCb);
		cvtColor(img, gray, COLOR_BGR2GRAY);

		//-- Step 1: Detect the keypoints using FAST Detector.
		cv::FastFeatureDetector detector(10);
		std::vector<KeyPoint> keypoints;
		detector.detect( gray, keypoints );


		//-- Step 2: Calculate descriptors (feature vectors).
		cv::BriefDescriptorExtractor extractor(32); //this is really 32 x 8 matches since they are binary matches packed into bytes
		Mat descriptors;
		extractor.compute( gray, keypoints, descriptors);

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
		LOG(LERROR) << "CvBRIEF::onNewImage failed\n";
	}
}



} //: namespace CvBRIEF
} //: namespace Processors
