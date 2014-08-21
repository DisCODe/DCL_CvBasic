/*!
 * \file
 * \brief
 * \author Tomasz Kornuta [tkornuta@ia.pw.edu.pl]
 */

#include <memory>
#include <string>

#include "Calib.hpp"
#include "Common/Logger.hpp"
#include "Types/MatrixTranslator.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace Calib {

Calib::Calib(const std::string & name) :
		Base::Component(name),
		continuous("continuous", false)
{
	addObject3D = false;
	// Register properties.
	registerProperty(continuous);
}

Calib::~Calib() {
}

void Calib::prepareInterface() {
	// Register data streams
	registerStream("in_object3d", &in_object3D);
	registerStream("in_camera_info", &in_camerainfo);
	registerStream("out_camera_info", &out_camerainfo);

	// Register handler processing the object3D.
	h_process_object3D.setup(boost::bind(&Calib::process_object3D, this));
	registerHandler("process_object3D", &h_process_object3D);
	addDependency("process_object3D", &in_object3D);
	addDependency("process_object3D", &in_camerainfo);

	// Register handler performing the calibration.
	h_perform_calibration.setup(boost::bind(&Calib::perform_calibration, this));
	registerHandler("perform_calibration", &h_perform_calibration);

	// Register handler setting the flag for acquisition of a single object3D.
	h_add_object3D.setup(boost::bind(&Calib::add_object3D, this));
	registerHandler("add_object3D", &h_add_object3D);

	// Register handler realizing the clearance of the whole dataset.
	h_clear_dataset.setup(boost::bind(&Calib::clear_dataset, this));
	registerHandler("clear_dataset", &h_clear_dataset);
}

bool Calib::onInit() {

	return true;
}

bool Calib::onFinish() {
	return true;
}

bool Calib::onStop() {
	return true;
}

bool Calib::onStart() {
	return true;
}

void Calib::process_object3D() {
	CLOG(LTRACE) << "Calib::process_chessboard";
    // Check component working mode.
    if (addObject3D || continuous) {
    	// Reset flag.
    	addObject3D = false;

		// Retrieve chessboard from the inputstream.
		Types::Objects3D::Object3D object = in_object3D.read();
		Types::CameraInfo camera_info = in_camerainfo.read();

		imageSize = camera_info.size();

		// Add image points.
		imagePoints.push_back(object.getImagePoints());

		// Add object points.
		objectPoints.push_back(object.getModelPoints());

		CLOG(LINFO) << "Registered new set of points";
    }

}

void Calib::add_object3D()
{
	addObject3D = true;
	CLOG(LINFO) << "Next set of points will be registered";
}

void Calib::clear_dataset()
{
	imagePoints.clear();
	objectPoints.clear();
	CLOG(LINFO) << "Dataset cleared";
}

void Calib::perform_calibration()
{
    CLOG(LINFO) << "Calib::perform_calibration()";

    if(imagePoints.size() > 0) {
		// The 3x3 camera matrix containing focal lengths fx,fy and displacement of the center of coordinates cx,cy.
		cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_32F);
		// Set two focal lengths in intrinsic matrix to have a ratio of 1.
		cameraMatrix.at<double>(0,0) = 1.0f;
		cameraMatrix.at<double>(1,1) = 1.0f;

		// Vector with distortion coefficients k_1, k_2, p_1, p_2, k_3.
		cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_32F);

		// The output vector of rotation vectors.
		vector<cv::Mat> rvecs;
		// The output vector of translation vectors.
		vector<cv::Mat> tvecs;

		// Calibrate camera.
		double errors = cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);

		// Display the results.
		LOG(LNOTICE) << "Calibration ended with reprojection error =" << errors;
		LOG(LNOTICE) << "Camera matrix: " << cameraMatrix;
		LOG(LNOTICE) << "Distortion coefficients: " << distCoeffs;

		Types::CameraInfo camera_info;
		camera_info.setSize(imageSize);
		camera_info.setCameraMatrix(cameraMatrix);
		camera_info.setDistCoeffs(distCoeffs);
		camera_info.setRotationMatrix(cv::Mat::eye(3, 3, CV_64F));
		camera_info.setTranlationMatrix(cv::Mat::zeros(3, 1, CV_64F));

		// Write parameters to the camerainfo
		out_camerainfo.write(camera_info);
    }
    else
		LOG(LERROR) << "Calib: dataset empty\n";

}

} //: namespace Calib
} //: namespace Processors
