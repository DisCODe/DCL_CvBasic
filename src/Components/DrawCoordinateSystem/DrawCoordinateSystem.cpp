/*!
 * \file
 * \brief
 * \author Marta Lepicka
 */

#include <memory>
#include <string>

#include "DrawCoordinateSystem.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

#include "Property.hpp"

namespace Processors {
namespace DrawCoordinateSystem {

DrawCoordinateSystem::DrawCoordinateSystem(const std::string & name) :
		Base::Component(name){
}

DrawCoordinateSystem::~DrawCoordinateSystem() {
}

void DrawCoordinateSystem::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_homogMatrix", &in_homogMatrix);
	registerStream("in_rvec", &in_rvec);
	registerStream("in_tvec", &in_tvec);
	registerStream("in_camera_matrix", &in_camera_matrix);

	registerStream("out_csystem", &out_csystem);
	registerStream("out_impoints", &out_impoints);

	// Register handlers
	registerHandler("projectPoints", boost::bind(&DrawCoordinateSystem::projectPoints, this));
	addDependency("projectPoints", &in_camera_matrix);
}

bool DrawCoordinateSystem::onInit() {

	return true;
}

bool DrawCoordinateSystem::onFinish() {
	return true;
}

bool DrawCoordinateSystem::onStop() {
	return true;
}

bool DrawCoordinateSystem::onStart() {
	return true;
}

void DrawCoordinateSystem::projectPoints(){
	cv::Mat_<double> rvec(3,1);
	cv::Mat_<double> tvec(3,1);
	Types::HomogMatrix homogMatrix;
	cv::Mat_<double> rotMatrix;
	rotMatrix.create(3,3);

	// Try to read rotation and translation from rvec and tvec or homogenous matrix.
	if(!in_rvec.empty()&&!in_tvec.empty()){
		rvec = in_rvec.read();
		tvec = in_tvec.read();
	}
	else if(!in_homogMatrix.empty()){
		homogMatrix = in_homogMatrix.read();

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				rotMatrix(i,j)=homogMatrix.elements[i][j];
			}
			tvec(i, 0) = homogMatrix.elements[i][3];
		}
		Rodrigues(rotMatrix, rvec);
	}
	else
		return;

	// Get camera info properties.
	Types::CameraInfo camera_matrix = in_camera_matrix.read();

	vector<cv::Point3f> object_points;
	vector<cv::Point2f> image_points;

	// Create four points constituting ends of three lines.
	object_points.push_back(cv::Point3f(0,0,0));
	object_points.push_back(cv::Point3f(0.1,0,0));
	object_points.push_back(cv::Point3f(0,0.1,0));
	object_points.push_back(cv::Point3f(0,0,0.1));

	// Project points taking into account the camera properties.
	cv::projectPoints(object_points, rvec, tvec, camera_matrix.cameraMatrix(), camera_matrix.distCoeffs(), image_points);

	// Draw lines between projected points.
	Types::Line ax(image_points[0], image_points[1]);
	ax.setCol(cv::Scalar(255, 0, 0));
	Types::Line ay(image_points[0], image_points[2]);
	ay.setCol(cv::Scalar(0, 255, 0));
	Types::Line az(image_points[0], image_points[3]);
	az.setCol(cv::Scalar(0, 0, 255));

	// Add lines to container.
	Types::DrawableContainer ctr;
	ctr.add(ax.clone());
	ctr.add(ay.clone());
	ctr.add(az.clone());

	CLOG(LINFO)<<"Image Points: "<<image_points;

	// Write output to dataports.
	out_csystem.write(ctr);
	out_impoints.write(image_points);
}


} //: namespace DrawCoordinateSystem
} //: namespace Processors
