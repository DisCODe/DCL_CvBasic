/*!
 * \file
 * \brief for grayscale images set; with FAST Detector
 * \author Jan Figat
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvFreak.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>


namespace Processors {
namespace CvFreak {

CvFreak::CvFreak(const std::string & name) :
		Base::Component(name)
{
	// Register properties.

}

CvFreak::~CvFreak() {
}

void CvFreak::prepareInterface() {
	// Register handlers with their dependencies.
    h_onNewImage.setup(this, &CvFreak::onNewImage);
    registerHandler("onNewImage", &h_onNewImage);
    addDependency("onNewImage", &in_img);

	// Input and output data streams.
    registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvFreak::onInit() {

	return true;
}

bool CvFreak::onFinish() {
	return true;
}

bool CvFreak::onStop() {
	return true;
}

bool CvFreak::onStart() {
	return true;
}

void CvFreak::onNewImage()
{
	LOG(LTRACE) << "CvFreak::onNewImage\n";
	try {
		// Input: a grayscale image.
        cv::Mat img = in_img.read();
        cv::Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);

        //-- Step 1: Detect the keypoints using FAST Detector.
        std::vector<KeyPoint> keypoints;
        //cv::FAST(gray,keypoints,10);
        cv::FastFeatureDetector detector(10);
        detector.detect( gray, keypoints );



		//-- Step 2: Calculate descriptors (feature vectors) using Freak descriptor.
        cv::FREAK extractor;
        cv::Mat descriptors;
        extractor.compute( gray, keypoints, descriptors);

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
		LOG(LERROR) << "CvFreak::onNewImage failed\n";
	}
}



} //: namespace CvFreak
} //: namespace Processors
