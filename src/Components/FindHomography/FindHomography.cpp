/*!
 * \file
 * \brief
 * \author Maciej Stefanczyk
 */

#include <memory>
#include <string>

#include "FindHomography.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace FindHomography {

FindHomography::FindHomography(const std::string & name) :
		Base::Component(name)  {

}

FindHomography::~FindHomography() {
}

void FindHomography::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_matches", &in_matches);
	registerStream("in_features0", &in_features0);
	registerStream("in_features1", &in_features1);
	registerStream("out_homography", &out_homography);
	// Register handlers
	registerHandler("calculate", boost::bind(&FindHomography::calculate, this));
	addDependency("calculate", &in_matches);
	addDependency("calculate", &in_features0);
	addDependency("calculate", &in_features1);

}

bool FindHomography::onInit() {

	return true;
}

bool FindHomography::onFinish() {
	return true;
}

bool FindHomography::onStop() {
	return true;
}

bool FindHomography::onStart() {
	return true;
}

void FindHomography::calculate() {
	std::vector<cv::DMatch> good_matches = in_matches.read();
	Types::Features f0 = in_features0.read();
	Types::Features f1 = in_features1.read();
	
	if (good_matches.size() < 4) {
		CLOG(LWARNING) << "Not enough points to calculate homography!";
		return;
	}
	
	CLOG(LINFO) << "Find homography based on " << good_matches.size() << " matches";
	
	//-- Localize the object
  std::vector<cv::Point2f> obj;
  std::vector<cv::Point2f> scene;

  for( int i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    obj.push_back( f0.features[ good_matches[i].queryIdx ].pt );
    scene.push_back( f1.features[ good_matches[i].trainIdx ].pt );
  }

	cv::Mat mask;
  cv::Mat H = findHomography( obj, scene, CV_RANSAC, 3, mask );
  CLOG(LINFO) << "Homography: \n" << H;
  CLOG(LINFO) << "Correct matches: " << cv::sum(mask)[0];
  
  out_homography.write(H);
}



} //: namespace FindHomography
} //: namespace Processors
