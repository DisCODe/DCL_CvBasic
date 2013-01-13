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
	addChessboard = false;
	// Register properties.
	registerProperty(continuous);
}

Calib::~Calib() {
}

void Calib::prepareInterface() {
	// Register data streams
	registerStream("in_chessboard", &in_chessboard);
	registerStream("in_camerainfo", &in_camerainfo);
	registerStream("out_camerainfo", &out_camerainfo);

	// Register handler processing the chessboard.
	h_process_chessboard.setup(boost::bind(&Calib::process_chessboard, this));
	registerHandler("process_chessboard", &h_process_chessboard);
	addDependency("process_chessboard", &in_chessboard);
	addDependency("process_chessboard", &in_camerainfo);

	// Register handler performing the calibration.
	h_perform_calibration.setup(boost::bind(&Calib::perform_calibration, this));
	registerHandler("perform_calibration", &h_perform_calibration);

	// Register handler setting the flag for acquisition of a single chessboard.
	h_add_chessboard.setup(boost::bind(&Calib::add_chessboard, this));
	registerHandler("add_chessboard", &h_add_chessboard);

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

void Calib::process_chessboard() {
    // Check component working mode.
    if (addChessboard || continuous) {
    	// Reset flag.
    	addChessboard = false;

		// Retrieve chessboard from the inputstream.
		Types::Objects3D::Chessboard chessboard = in_chessboard.read();
		Types::CameraInfo camera_info = in_camerainfo.read();

		imageSize = camera_info.size();

		// Add image points.
		imagePoints.push_back(chessboard.getImagePoints());

		// Add object points.
		objectPoints.push_back(chessboard.getModelPoints());

		CLOG(LINFO) << "Registered new set of points";
    }

}

void Calib::add_chessboard()
{
	addChessboard = true;
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
    LOG(LINFO) << "Calib::perform_calibration()";

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

		// Write parameters to the camerainfo
		out_camerainfo.write(camera_info);
    }
    else
		LOG(LERROR) << "Calib: dataset empty\n";

}

} //: namespace Calib
} //: namespace Processors
