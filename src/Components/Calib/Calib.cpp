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
		Base::Component(name)  {

}

Calib::~Calib() {
}

void Calib::prepareInterface() {
	// Register data streams
	registerStream("in_chessboard", &in_chessboard);
	registerStream("in_camerainfo", &in_camerainfo);

	registerStream("out_camerainfo", &out_camerainfo);

	// Register handlers.
	h_process_chessboard.setup(boost::bind(&Calib::process_chessboard, this));
	registerHandler("process_chessboard", &h_process_chessboard);
	addDependency("process_chessboard", &in_chessboard);
	addDependency("process_chessboard", &in_camerainfo);

	// Register handlers responsible for
	h_perform_calibration.setup(boost::bind(&Calib::perform_calibration, this));
	registerHandler("perform_calibration", &h_perform_calibration);
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


void Calib::perform_calibration()
{
    LOG(LINFO) << "Calib::perform_calibration()";

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

    //double calibrateCamera(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints, Size imageSize, InputOutputArray cameraMatrix, InputOutputArray distCoeffs, OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs, int flags=0, TermCriteria criteria=TermCriteria( TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON) )

    LOG(LNOTICE) << "Calibration ended with reprojection error =" << errors;
    LOG(LNOTICE) << "Camera matrix: " << cameraMatrix;
    LOG(LNOTICE) << "Distortion coefficients: " << distCoeffs;

    Types::CameraInfo camera_info;
    camera_info.setSize(imageSize);
    camera_info.setCameraMatrix(cameraMatrix);
    camera_info.setDistCoeffs(distCoeffs);

    out_camerainfo.write(camera_info);

    // Save calibration parameters.
    //saveParameters(cameraMatrix, distCoeffs);
}

} //: namespace Calib
} //: namespace Processors
