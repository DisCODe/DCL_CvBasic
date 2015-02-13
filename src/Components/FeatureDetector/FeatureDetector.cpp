/*!
 * \file
 * \brief
 * \author Maciej Stefanczyk
 */

#include <memory>
#include <string>

#include "FeatureDetector.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace FeatureDetector {

FeatureDetector::FeatureDetector(const std::string & name) :
		Base::Component(name) , 
		orb_nfeatures("orb.nfeatures", 500), 
		orb_scale_factor("orb.scale_factor", 1.2), 
		orb_nlevels("orb.nlevels", 8), 
		orb_edge_threshold("orb.edge_threshold", 31), 
		orb_wta_k("orb.wta_k", 2), 
		orb_score_type("orb.score_type", cv::ORB::HARRIS_SCORE), 
		orb_patch_size("orb.patch_size", 31), 
		brisk_threshold("brisk.threshold", 30), 
		brisk_octaves("brisk.octaves", 3), 
		brisk_pattern_scale("brisk.pattern_scale", 1.0), 
		active_extractor("active_extractor", std::string("ORB"), "combo") {
	
	orb_nfeatures.addConstraint("0");
	orb_nfeatures.addConstraint("999");
	registerProperty(orb_nfeatures);
	registerProperty(orb_scale_factor);
	
	orb_nlevels.addConstraint("1");
	orb_nlevels.addConstraint("10");
	registerProperty(orb_nlevels);
	registerProperty(orb_edge_threshold);
	registerProperty(orb_wta_k);
	registerProperty(orb_score_type);
	registerProperty(orb_patch_size);
	
	registerProperty(brisk_threshold);
	registerProperty(brisk_octaves);
	registerProperty(brisk_pattern_scale);
	
	active_extractor.addConstraint("ORB");
	active_extractor.addConstraint("BRISK");
	registerProperty(active_extractor);

}

FeatureDetector::~FeatureDetector() {
}

void FeatureDetector::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img_A", &in_img_A);
	registerStream("in_img_B", &in_img_B);
	registerStream("out_features_A", &out_features_A);
	registerStream("out_features_B", &out_features_B);
	registerStream("out_descriptors_A", &out_descriptors_A);
	registerStream("out_descriptors_B", &out_descriptors_B);
	// Register handlers
	registerHandler("onNewImageA", boost::bind(&FeatureDetector::onNewImageA, this));
	addDependency("onNewImageA", &in_img_A);
	registerHandler("onNewImageB", boost::bind(&FeatureDetector::onNewImageB, this));
	addDependency("onNewImageB", &in_img_B);

}

bool FeatureDetector::onInit() {

	return true;
}

bool FeatureDetector::onFinish() {
	return true;
}

bool FeatureDetector::onStop() {
	return true;
}

bool FeatureDetector::onStart() {
	return true;
}

void FeatureDetector::onNewImageA() {
	cv::Mat img = in_img_A.read();
	
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;
	
	if (active_extractor == "ORB") {
		cv::ORB orb(orb_nfeatures, orb_scale_factor, orb_nlevels, orb_edge_threshold, 0, orb_wta_k, orb_score_type, orb_patch_size);
		orb(img, cv::Mat(), keypoints, descriptors);
	}
	
	if (active_extractor == "BRISK") {
		cv::BRISK brisk(brisk_threshold, brisk_octaves, brisk_pattern_scale);
		brisk.detect( img, keypoints );
		brisk.compute( img, keypoints, descriptors);
	}
	
	if (keypoints.size() < 1) {
		CLOG(LERROR) << "No keypoints found!";
		return;
	}

	// Write features to the output.
	Types::Features features(keypoints);
	out_features_A.write(features);

	// Write descriptors to the output.
	out_descriptors_A.write(descriptors);
}

void FeatureDetector::onNewImageB() {
	cv::Mat img = in_img_B.read();
	
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;
	
	if (active_extractor == "ORB") {
		cv::ORB orb(orb_nfeatures, orb_scale_factor, orb_nlevels, orb_edge_threshold, 0, orb_wta_k, orb_score_type, orb_patch_size);
		orb(img, cv::Mat(), keypoints, descriptors);
	}
	
	if (active_extractor == "BRISK") {
		cv::BRISK brisk(brisk_threshold, brisk_octaves, brisk_pattern_scale);
		brisk.detect( img, keypoints );
		brisk.compute( img, keypoints, descriptors);
	}
	
	if (keypoints.size() < 1) {
		CLOG(LERROR) << "No keypoints found!";
		return;
	}

	// Write features to the output.
	Types::Features features(keypoints);
	out_features_B.write(features);

	// Write descriptors to the output.
	out_descriptors_B.write(descriptors);
}



} //: namespace FeatureDetector
} //: namespace Processors
