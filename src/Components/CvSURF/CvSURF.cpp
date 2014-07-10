/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvSURF.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvSURF {

CvSURF::CvSURF(const std::string & name) :
		Base::Component(name),
        minHessian("minHessian", 400, "minHessian")
{
	// Register properties.
	registerProperty(minHessian);
}

CvSURF::~CvSURF() {
}

void CvSURF::prepareInterface() {
	// Register handlers with their dependencies.
	h_onNewImage.setup(this, &CvSURF::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvSURF::onInit() {

	return true;
}

bool CvSURF::onFinish() {
	return true;
}

bool CvSURF::onStop() {
	return true;
}

bool CvSURF::onStart() {
	return true;
}

void CvSURF::onNewImage()
{
	LOG(LTRACE) << "CvSURF::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read();


		//-- Step 1: Detect the keypoints using SURF Detector.
		SurfFeatureDetector detector( minHessian );
		std::vector<KeyPoint> keypoints;
		detector.detect( input, keypoints );


		//-- Step 2: Calculate descriptors (feature vectors).
        SurfDescriptorExtractor extractor;
		Mat descriptors;
		extractor.compute( input, keypoints, descriptors);

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
		LOG(LERROR) << "CvSURF::onNewImage failed\n";
	}
}



} //: namespace CvSURF
} //: namespace Processors
