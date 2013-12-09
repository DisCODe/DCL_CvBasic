/*!
 * \file
 * \brief
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvBRISK.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvBRISK {

CvBRISK::CvBRISK(const std::string & name) :
        Base::Component(name),
        thresh("thresh",60,"thresh")
        //octaves("octaves",3,"octaves"),
        //patternScale("patternScale",1.0f,"patternScale")
{
	// Register properties.
    registerProperty(thresh);
}

CvBRISK::~CvBRISK() {
}

void CvBRISK::prepareInterface() {
	// Register handlers with their dependencies.
    h_onNewImage.setup(this, &CvBRISK::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	// objectImg and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);

}

bool CvBRISK::onInit() {

	return true;
}

bool CvBRISK::onFinish() {
	return true;
}

bool CvBRISK::onStop() {
	return true;
}

bool CvBRISK::onStart() {
	return true;
}

void CvBRISK::onNewImage()
{
    LOG(LTRACE) << "CvBRISK::onNewImage\n";
	try {
		// objectImg: a grayscale image.
		cv::Mat objectImg = in_img.read();

        //-- Step 1: Detect the keypoints using Brisk Detector.
        //cv::BriskFeatureDetector detector;//(thresh,3,1.0f);
		cv::FeatureDetector * detector = new cv::BRISK(thresh,3,1.0f);
		std::vector<cv::KeyPoint> keypoints;
		detector->detect( objectImg, keypoints );


		//-- Step 2: Calculate descriptors (feature vectors).
        //cv::BriskDescriptorExtractor extractor;
		cv::DescriptorExtractor * extractor = new cv::BRISK();
		cv::Mat descriptors;
		extractor->compute( objectImg, keypoints, descriptors);

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
        LOG(LERROR) << "CvBRISK::onNewImage failed\n";
	}
}



} //: namespace CvBRISK
} //: namespace Processors
