/*!
 * \file
 * \brief
 * \author Jan Figat
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvBruteForce.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvBruteForce {

CvBruteForce::CvBruteForce(const std::string & name) :
		Base::Component(name),
		distance_recalc("recalculate_distance", true),
		print_stats("print_stats", true),
		dist("distance", 0.15)
{
	registerProperty(distance_recalc);
	registerProperty(print_stats);
	registerProperty(dist);
}

CvBruteForce::~CvBruteForce() {
}

void CvBruteForce::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvBruteForce::onNewImage, this));
	addDependency("onNewImage", &in_features0);
	addDependency("onNewImage", &in_features1);
	addDependency("onNewImage", &in_descriptors0);
	addDependency("onNewImage", &in_descriptors1);
	addDependency("onNewImage", &in_img0);
	addDependency("onNewImage", &in_img1);

	// Input and output data streams.
	registerStream("in_features0", &in_features0);
	registerStream("in_features1", &in_features1);
	registerStream("in_descriptors0", &in_descriptors0);
	registerStream("in_descriptors1", &in_descriptors1);
	registerStream("in_img0", &in_img0);
	registerStream("in_img1", &in_img1);
	registerStream("out_img", &out_img);
	registerStream("out_matches", &out_matches);
}

bool CvBruteForce::onInit() {

	return true;
}

bool CvBruteForce::onFinish() {
	return true;
}

bool CvBruteForce::onStop() {
	return true;
}

bool CvBruteForce::onStart() {
	return true;
}


void CvBruteForce::onNewImage()
{
	CLOG(LTRACE) << "CvBruteForce::onNewImage\n";
	try {
		// Read input features.
		Types::Features features_1 = in_features0.read();
		Types::Features features_2 = in_features1.read();
		// Read input descriptors.
		cv::Mat img_1 = in_img0.read();
		cv::Mat img_2 = in_img1.read();
		// Read input images.
		cv::Mat descriptors_1 = in_descriptors0.read();
		cv::Mat descriptors_2 = in_descriptors1.read();

		// Matching descriptor vectors using BruteForce matcher.
		BFMatcher matcher(cv::NORM_HAMMING); //BruteForce
		std::vector< DMatch > matches;
		matcher.match( descriptors_1, descriptors_2, matches );

		if (distance_recalc) {
			double max_dist = 0;
			double min_dist = 100;
			//-- Quick calculation of max and min distances between keypoints.
			for( int i = 0; i < descriptors_1.rows; i++ )
			{
				double dist = matches[i].distance;
				if( dist < min_dist ) min_dist = dist;
				if( dist > max_dist ) max_dist = dist;
			}
			dist = 2*min_dist;
			CLOG(LINFO) << " Max dist : " << (double)max_dist;
			CLOG(LINFO) << " Min dist : " << (double)min_dist;
			CLOG(LINFO) << " Dist : " << (double)dist << std::endl;
		}

		//Draw only "good" matches (i.e. whose distance is less than 2*min_dist ).
		//PS.- radiusMatch can also be used here.
		std::vector< DMatch > good_matches;
		for( int i = 0; i < descriptors_1.rows; i++ )
		{
			if( matches[i].distance < dist )
				good_matches.push_back( matches[i]);
		}

		//-- Draw only "good" matches
		Mat img_matches;
		drawMatches( img_1, features_1.features, img_2, features_2.features,
				   good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
				   vector<char>(), DrawMatchesFlags::DEFAULT );

		// Print stats.
		if (print_stats) {
			for( int i = 0; i < good_matches.size(); i++ )
			{
				CLOG(LINFO) << " Good Match [" << i <<"] Keypoint 1: " << good_matches[i].queryIdx << "  -- Keypoint 2: " << good_matches[i].trainIdx;
			}
			CLOG(LINFO) << std::endl;
		}


		// Write the result to the output.
		out_img.write(img_matches);
		out_matches.write(good_matches);
	} catch (...) {
		CLOG(LERROR) << "CvBruteForce::onNewImage failed\n";
	}
}



} //: namespace CvBruteForce
} //: namespace Processors
