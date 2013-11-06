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
        nfeatures("nfeatures", 500, "nfeatures")/*,
                scaleFactor("scaleFactor", 1.2f, "scaleFactor"),
                nlevels("nlevels", 8, "nlevels"),
		        edgeThreshold("edgeThreshold", 31, "edgeThreshold"),
		        firstLevel("firstLevel", 0, "firstLevel"),
		        WTA_K("WTA_K", 2, "WTA_K"), //The default value 2 means the BRIEF where we take a random point pair and compare their brightnesses, so we get 0/1 response. Other possible values are 3 and 4. For example, 3 means that we take 3 random points (of course, those point coordinates are random, but they are generated from the pre-defined seed, so each element of BRIEF descriptor is computed deterministically from the pixel rectangle), find point of maximum brightness and output index of the winner (0, 1 or 2). Such output will occupy 2 bits, and therefore it will need a special variant of Hamming distance, denoted as NORM_HAMMING2 (2 bits per bin). When WTA_K=4, we take 4 random points to compute each bin (that will also occupy 2 bits with possible values 0, 1, 2 or 3).
                scoreType("scoreType", FAST::HARRIS_SCORE, "scoreType"),
                patchSize("patchSize", edgeThreshold31, "patchSize")
                */
		{
			// Register properties.
            registerProperty(nfeatures);
            /*registerProperty(scaleFactor);
            registerProperty(nlevels);
			registerProperty(edgeThreshold);
			registerProperty(scoreType);*/
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
	registerStream("out_descriptors", &out_descriptors);
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
    LOG(LTRACE) << "CvFAST::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read();
		cv::Mat gray;
		cvtColor(input, gray, COLOR_BGR2GRAY);

        //-- Step 1: Detect the keypoints using FAST Detector.
        cv::FastFeatureDetector detector( nfeatures/*, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, scoreType, patchSize*/);
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
        LOG(LERROR) << "CvFAST::onNewImage failed\n";
	}
}



} //: namespace CvFAST
} //: namespace Processors
