/*!
 * \file
 * \brief
 * \author Maciej Stefanczyk
 */

#include <memory>
#include <string>

#include "PerspectiveTransform.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace PerspectiveTransform {

PerspectiveTransform::PerspectiveTransform(const std::string & name) :
		Base::Component(name)  {

}

PerspectiveTransform::~PerspectiveTransform() {
}

void PerspectiveTransform::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_point", &in_point);
	registerStream("in_points", &in_points);
	registerStream("in_homography", &in_homography);
	registerStream("out_point", &out_point);
	registerStream("out_points", &out_points);
	registerStream("out_draw", &out_draw);
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("transformPoint", boost::bind(&PerspectiveTransform::transformPoint, this));
	addDependency("transformPoint", &in_point);
	addDependency("transformPoint", &in_homography);
	registerHandler("transformPoints", boost::bind(&PerspectiveTransform::transformPoints, this));
	addDependency("transformPoints", &in_points);
	addDependency("transformPoints", &in_homography);
	registerHandler("transformImage", boost::bind(&PerspectiveTransform::transformImage, this));
	addDependency("transformImage", &in_img);
	addDependency("transformImage", &in_homography);
	registerHandler("clearHomography", boost::bind(&PerspectiveTransform::clearHomography, this));
	addDependency("clearHomography", &in_homography);

}

bool PerspectiveTransform::onInit() {

	return true;
}

bool PerspectiveTransform::onFinish() {
	return true;
}

bool PerspectiveTransform::onStop() {
	return true;
}

bool PerspectiveTransform::onStart() {
	return true;
}

void PerspectiveTransform::transformPoint() {
	std::vector<cv::Point2f> points;
	std::vector<cv::Point2f> out;
	cv::Point2f pt = in_point.read();
	points.push_back(pt);
	cv::Mat H = in_homography.read();
	
	cv::perspectiveTransform(points, out, H);
	Types::Points pts;
	pts.addPoint(out[0]);
	
	out_point.write(out[0]);
	out_draw.write(pts);
}

void PerspectiveTransform::transformPoints() {
	std::vector<cv::Point2f> points = in_points.read();
	std::vector<cv::Point2f> out;
	cv::Mat H = in_homography.read();
	
	cv::perspectiveTransform(points, out, H);
	Types::Points pts;
	pts.setPoints(out);
	
	out_points.write(out);
	out_draw.write(pts);
}

void PerspectiveTransform::clearHomography() {
	in_homography.read();
}

void PerspectiveTransform::transformImage() {
	cv::Mat img = in_img.read();
	cv::Mat H = in_homography.read();
	cv::Mat out;
	cv::warpPerspective(img, out, H, cv::Size(1280, 960));
	out_img.write(out);
}



} //: namespace PerspectiveTransform
} //: namespace Processors
