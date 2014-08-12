/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvContour.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvContour {

CvContour::CvContour(const std::string & name) :
		Base::Component(name)  {

}

CvContour::~CvContour() {
}

void CvContour::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvContour::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_contours", &out_contours);
	registerStream("out_img", &out_img);
}

bool CvContour::onInit() {

	return true;
}

bool CvContour::onFinish() {
	return true;
}

bool CvContour::onStop() {
	return true;
}

bool CvContour::onStart() {
	return true;
}

RNG rng(12345);

void CvContour::onNewImage()
{
	CLOG(LTRACE) << "CvContour::onNewImage\n";
	try {
		// Input: a binary or edge image.
		cv::Mat input = in_img.read();

		// Find contours.
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours( input, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

/*		  /// Get the moments
		  vector<Moments> mu(contours.size() );
		  for( int i = 0; i < contours.size(); i++ )
		     { mu[i] = moments( contours[i], false ); }

		  ///  Get the mass centers:
		  vector<Point2f> mc( contours.size() );
		  for( int i = 0; i < contours.size(); i++ )
		     { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }*/

		CLOG(LINFO) << "Found "<< contours.size() << " contours";
		// Select only the external contour
/*		if (contours.size() > 1)
			contours.erase (contours.begin()+1,contours.end());*/


		  /// Draw contours
		  Mat drawing = Mat::zeros( input.size(), CV_8UC3 );
		  for( int i = 0; i< contours.size(); i++ )
		     {
		       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//		       circle( drawing, mc[i], 4, color, -1, 8, 0 );
		     }

		// Write contours to the output.
		out_contours.write(contours);
		out_img.write(drawing);
	} catch (...) {
		CLOG(LERROR) << "CvContour::onNewImage failed\n";
	}
}



} //: namespace CvContour
} //: namespace Processors
