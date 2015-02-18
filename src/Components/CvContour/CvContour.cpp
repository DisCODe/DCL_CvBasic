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
		Base::Component(name),
		prop_min_size("min_size", 100)  {

	prop_min_size.addConstraint("0");
	prop_min_size.addConstraint("10000");
	registerProperty(prop_min_size);
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
	registerStream("out_moments", &out_moments);
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

		// Get the moments
		vector<Moments> mu(contours.size() );
		for( int i = 0; i < contours.size(); i++ ) { 
			mu[i] = moments( contours[i], false ); 
		}

		CLOG(LINFO) << "Found "<< contours.size() << " contours";

		// filter contours
		vector<vector<Point> >::iterator c_iter = contours.begin();
		vector<Vec4i>::iterator h_iter = hierarchy.begin();
		vector<Moments>::iterator m_iter = mu.begin();
		
		while (c_iter != contours.end()) {
			// check contours area
			if (m_iter->m00 < prop_min_size) {
				m_iter = mu.erase(m_iter);
				c_iter = contours.erase(c_iter);
				h_iter = hierarchy.erase(h_iter);
			} else {
				++m_iter;
				++c_iter;
				++h_iter;
			}
		}
		

		//  Get the mass centers:
		vector<Point2f> mc( contours.size() );
		for( int i = 0; i < contours.size(); i++ ) {
			mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
		}

		// Draw contours
		Mat drawing = Mat::zeros( input.size(), CV_8UC3 );
		for( int i = 0; i< contours.size(); i++ ) {
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
			circle( drawing, mc[i], 2, color, -1, 8, 0 );
		}

		// Write contours to the output.
		out_contours.write(contours);
		out_moments.write(mu);
		out_img.write(drawing);
	} catch (...) {
		CLOG(LERROR) << "CvContour::onNewImage failed\n";
	}
}



} //: namespace CvContour
} //: namespace Processors
