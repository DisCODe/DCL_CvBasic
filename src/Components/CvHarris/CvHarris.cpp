/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
	modified by Jan Figat
 */

#include <memory>
#include <string>

#include "CvHarris.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvHarris {

CvHarris::CvHarris(const std::string & name) :
		Base::Component(name),
		blockSize("blockSize", 2),
		apertureSize("apertureSize", 3),
		k("k", 0.04),
		thresh("thresh", 200, "range")
{
	// Constraints.
	thresh.addConstraint("0");
	thresh.addConstraint("255");

	// Register properties.
	registerProperty(blockSize);
	registerProperty(apertureSize);
	registerProperty(k);
	registerProperty(thresh);
}

CvHarris::~CvHarris() {
}

void CvHarris::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvHarris::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
}

bool CvHarris::onInit() {

	return true;
}

bool CvHarris::onFinish() {
	return true;
}

bool CvHarris::onStop() {
	return true;
}

bool CvHarris::onStart() {
	return true;
}

void CvHarris::onNewImage()
{
	LOG(LTRACE) << "CvHarris::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat in = in_img.read();


		Mat dst, dst_norm, dst_norm_scaled;
		dst = Mat::zeros( in.size(), CV_32FC1 );

		/// Detecting corners
		cornerHarris( in, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

		/// Normalizing
		normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
		//convertScaleAbs( dst_norm, dst_norm_scaled );

	    std::vector<cv::KeyPoint> keypoints;
		/// Drawing a circle around corners
		for( int j = 0; j < dst_norm.rows ; j++ )
		   { for( int i = 0; i < dst_norm.cols; i++ )
		        {
		          if( (int) dst_norm.at<float>(j,i) > thresh )
		            {
		        	  keypoints.push_back(cv::KeyPoint(i,j,5));
		            }
		        }
		   }

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);
	} catch (...) {
		LOG(LERROR) << "CvHarris::onNewImage failed\n";
	}
}

} //: namespace CvHarris
} //: namespace Processors
