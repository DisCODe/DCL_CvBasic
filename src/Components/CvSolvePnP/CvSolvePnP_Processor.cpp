/*
 * CvSolvePnP_Processor.cpp
 *
 *  Created on: 18-10-2010
 *      Author: mateusz
 */

#include "CvSolvePnP_Processor.hpp"
#include "Logger.hpp"

#include <sstream>
#include "Property.hpp"
#include <boost/foreach.hpp>

namespace Processors {

namespace CvSolvePnP {

using namespace std;
using namespace cv;
using namespace boost;
using Types::HomogMatrix;
using namespace Types::Objects3D;

CvSolvePnP_Processor::CvSolvePnP_Processor(const std::string & n) :
	Component(n),
	prop_enable("enable", true),
	prop_x("offset.x", 0),
	prop_y("offset.y", 0),
	prop_z("offset.z", 0),
	prop_roll("offset.roll", 0),
	prop_pitch("offset.pitch", 0),
	prop_yaw("offset.yaw", 0),
	prop_rectified("rectified", false),
	flags("flags", CV_ITERATIVE)
{
	registerProperty(prop_x);
	registerProperty(prop_y);
	registerProperty(prop_z);
	registerProperty(prop_roll);
	registerProperty(prop_pitch);
	registerProperty(prop_yaw);
	registerProperty(prop_rectified);
}

CvSolvePnP_Processor::~CvSolvePnP_Processor()
{
}

void CvSolvePnP_Processor::prepareInterface() {
	registerHandler("onNewObject3D", boost::bind(&CvSolvePnP_Processor::onNewObject3D, this));
	addDependency("onNewObject3D", &in_object3d);
	addDependency("onNewObject3D", &in_camerainfo);

	registerStream("in_object3d", &in_object3d);
	registerStream("in_camera_info", &in_camerainfo);
	registerStream("out_homogMatrix", &out_homogMatrix);
	registerStream("out_rvec", &out_rvec);
	registerStream("out_tvec", &out_tvec);
}

bool CvSolvePnP_Processor::onStart()
{
	CLOG(LTRACE) << "CvSolvePnP_Processor::onStart()\n";
	return true;
}

bool CvSolvePnP_Processor::onStop()
{
	CLOG(LTRACE) << "CvSolvePnP_Processor::onStop()\n";
	return true;
}

bool CvSolvePnP_Processor::onInit()
{
	CLOG(LTRACE) << "CvSolvePnP_Processor::onInit()\n";


	return true;
}

bool CvSolvePnP_Processor::onFinish()
{
	CLOG(LTRACE) << "CvSolvePnP_Processor::onFinish()\n";
	return true;
}

void CvSolvePnP_Processor::onNewObject3D()
{
	CLOG(LTRACE) << "CvSolvePnP_Processor::onNewObject3D()\n";
	boost::shared_ptr <Types::Objects3D::Object3D> object3D = in_object3d.read();

	Types::CameraInfo camera_info = in_camerainfo.read();

	// Resulting translation vector.
	Mat_<double> tvec;
	// Resulting rotation matrix and vector.
	Mat_<double> rotationMatrix;
	Mat_<double> rvec;


//	vector<cv::Point3f> model=object3D->getModelPoints();
//
//	for(int i=0; i< model.size(); i++){
//		model[i].x += prop_X;
//		model[i].y += prop_Y;
//	}
//	object3D->setModelPoints(model);

	// Create matrices with model and image points.
	Mat modelPoints(object3D->getModelPoints());
	Mat imagePoints(object3D->getImagePoints());

	
	// Check whether the image is rectified.
	if (prop_rectified) {
		Mat camera_matrix;
		Mat rotMatrix1, rotVector2, trans1, trans2;
		Mat dist_coeffs;

		// If so, use data after rectification and decompose the projection matrix.
		decomposeProjectionMatrix(camera_info.projectionMatrix(), camera_matrix, rotMatrix1, trans1);

		CLOG(LDEBUG) << " After decomposition of projection matrix";
		CLOG(LDEBUG) << " Camera matrix = "<< camera_matrix;
		CLOG(LDEBUG) << " Rotation matrix = "<< rotMatrix1 << "  translation vector = " << trans1;

		// Empty distortion coefficients vector.
		dist_coeffs = Mat ();

		// Solve PnP for 3d-2d pairs of points.
		solvePnP(modelPoints, imagePoints, camera_matrix, dist_coeffs, rotVector2, trans2, false);

		CLOG(LDEBUG) << "SolvePnP: rot = "<< rotVector2 << "  trans=" << trans2;

		Mat_<double> rotMatrix2;
		// Use Rodriques transformation to get rotation matrix.
		Rodrigues(rotVector2, rotMatrix2);

		// Update rotation and translation.
		rotationMatrix = rotMatrix1 * rotMatrix2;
		tvec = rotMatrix1 * trans2 + trans1.rowRange(Range(0,3));
		CLOG(LDEBUG) << " Tvec = rotMatrix1 * trans2 + trans1 = "<< tvec ;
		
		// Update rotation vector.
		Rodrigues(rotationMatrix, rvec);

	} else {
		// Solve PnP for 3d-2d pairs of points for camera matrix and distortion coefficients.
		solvePnP(modelPoints, imagePoints, camera_info.cameraMatrix(), camera_info.distCoeffs(), rvec, tvec, false, flags);
		// Use Rodriques transformation to get rotation matrix.
		Rodrigues(rvec, rotationMatrix);
	}//: else

	CLOG(LINFO) << "rvec = "<< rvec << "  tvec=" << tvec;

	// Create homogenous matrix.
	cv::Mat pattern_pose = (cv::Mat_<double>(4, 4) <<
			rotationMatrix(0,0), rotationMatrix(0,1), rotationMatrix(0,2), tvec(0),
			rotationMatrix(1,0), rotationMatrix(1,1), rotationMatrix(1,2), tvec(1),
			rotationMatrix(2,0), rotationMatrix(2,1), rotationMatrix(2,2), tvec(2),
			0, 0, 0, 1);

    CLOG(LINFO) << "pattern_pose:\n" << pattern_pose;
	
	HomogMatrix hm;
	if (prop_enable){
		// Roll - rotation around the X (blue) axis.
		cv::Mat roll = (cv::Mat_<double>(4, 4) <<
					  1,          0,           0, 0,
					  0, cos(prop_roll), -sin(prop_roll), 0,
					  0, sin(prop_roll),  cos(prop_roll), 0,
					  0, 0, 0, 1 );

		// Pitch - rotation around the Y (green) axis.
		cv::Mat pitch = (cv::Mat_<double>(4, 4) <<
				cos(prop_pitch), 0, sin(prop_pitch), 0,
				0, 1, 0, 0,
				-sin(prop_pitch),  0,	cos(prop_pitch), 0,
				0, 0, 0, 1 );

		// Yaw - rotation around the Z (red) axis.
		cv::Mat yaw = (cv::Mat_<double>(4, 4) <<
				cos(prop_yaw), -sin(prop_yaw), 0, 0,
				sin(prop_yaw),  cos(prop_yaw), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1 );

		// translation
		cv::Mat t = (cv::Mat_<double>(4, 4) <<
							0, 0, 0, prop_x,
							0, 0, 0, prop_y,
							0, 0, 0, prop_z,
							0, 0, 0, 0 );

		//rottMatrix = rottMatrix * RX;
		// transform
		cv::Mat tmp = (pattern_pose * (t + yaw * pitch * roll));
        hm = tmp;
	}
    else
        hm = pattern_pose;
		
    CLOG(LINFO) << "HomogMatrix:\n" << hm;
	

/*
	TODO: fix
	out_rvec.write(rvec.clone());
	out_tvec.write(tvec.clone());*/
	out_homogMatrix.write(hm);
}

} // namespace CvSolvePnP

} // namespace Processors
