/*!
 * \file
 * \brief ORB - oriented BRIEF
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvORB.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvORB {

CvORB::CvORB(const std::string & name) :
        Base::Component(name),
        nfeatures("nfeatures", 500, "nfeatures")
{
	// Register properties.
	registerProperty(nfeatures);
}

CvORB::~CvORB() {
}

void CvORB::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvORB::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvORB::onInit() {

	return true;
}

bool CvORB::onFinish() {
	return true;
}

bool CvORB::onStop() {
	return true;
}

bool CvORB::onStart() {
	return true;
}

void CvORB::onNewImage()
{
	LOG(LTRACE) << "CvORB::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read().clone();
		if (input.type() != CV_8UC1)
			cvtColor(input, input, COLOR_BGR2GRAY);

		//-- Step 1: Detect the keypoints using ORB Detector.
		cv::ORB orb( nfeatures/*, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, scoreType, patchSize*/);
		std::vector<KeyPoint> keypoints;
		Mat descriptors;
		orb(input, cv::Mat(), keypoints, descriptors);

		// Write features to the output.
		Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
		LOG(LERROR) << "CvORB::onNewImage failed\n";
	}
}



} //: namespace CvORB
} //: namespace Processors
