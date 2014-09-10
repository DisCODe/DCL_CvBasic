/*!
 * \file
 * \brief StarDetector detector with SURF descriptor
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvStarDetector.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvStarDetector {

CvStarDetector::CvStarDetector(const std::string & name) :
        Base::Component(name),
        nfeatures("nfeatures", 500, "nfeatures")
{
	// Register properties.
	registerProperty(nfeatures);
}

CvStarDetector::~CvStarDetector() {
}

void CvStarDetector::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvStarDetector::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvStarDetector::onInit() {

	return true;
}

bool CvStarDetector::onFinish() {
	return true;
}

bool CvStarDetector::onStop() {
	return true;
}

bool CvStarDetector::onStart() {
	return true;
}

void CvStarDetector::onNewImage()
{
    LOG(LTRACE) << "CvStarDetector::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read();
		cv::Mat gray;
		cvtColor(input, gray, COLOR_BGR2GRAY);

        //-- Step 1: Detect the keypoints using StarDetector Detector.
        cv::StarDetector /*StarFeatureDetector*/ detector( nfeatures/*, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, scoreType, patchSize*/);
		std::vector<KeyPoint> keypoints;
		detector.detect( gray, keypoints );


		//-- Step 2: Calculate descriptors (feature vectors) - SURF descriptor.
        cv::SurfDescriptorExtractor extractor;
		cv::Mat descriptors;
		extractor.compute( gray, keypoints, descriptors);

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
        LOG(LERROR) << "CvStarDetector::onNewImage failed\n";
	}
}



} //: namespace CvStarDetector
} //: namespace Processors
