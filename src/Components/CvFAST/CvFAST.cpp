/*!
 * \file
 * \brief FAST detector with SURF descriptor
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvFAST.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvFAST {

CvFAST::CvFAST(const std::string & name) :
        Base::Component(name),
        m_threshold("threshold", 10, "range")
{
	m_threshold.addConstraint("1");
	m_threshold.addConstraint("255");
	registerProperty(m_threshold);
}

CvFAST::~CvFAST() {
}

void CvFAST::prepareInterface() {
	// Register handlers with their dependencies.
    h_onNewImage.setup(this, &CvFAST::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
}

bool CvFAST::onInit() {

	return true;
}

bool CvFAST::onFinish() {
	return true;
}

bool CvFAST::onStop() {
	return true;
}

bool CvFAST::onStart() {
	return true;
}

void CvFAST::onNewImage()
{
    CLOG(LTRACE) << "CvFAST::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read();

        //-- Step 1: Detect the keypoints using FAST Detector.
        cv::FastFeatureDetector detector(m_threshold);
		std::vector<KeyPoint> keypoints;
		detector.detect( input, keypoints );

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);
	} catch (...) {
        CLOG(LERROR) << "CvFAST::onNewImage failed\n";
	}
}



} //: namespace CvFAST
} //: namespace Processors
