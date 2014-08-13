/*
 * HomogenousMatrixProvider.cpp
 *
 *  Created on: 13-08-2014
 *      Author: lzmuda
 */

#include "HomogenousMatrixProvider.hpp"
#include "Logger.hpp"

#include <sstream>
#include "Property.hpp"
#include <boost/foreach.hpp>

namespace Processors {

namespace HomogenousMatrixProvider {

using namespace std;
using namespace cv;
using namespace boost;
using Types::HomogMatrix;
using namespace Types::Objects3D;

HomogenousMatrixProvider::HomogenousMatrixProvider(const std::string & n) :
	Component(n),
	prop_x("offset.x", 0),
	prop_y("offset.y", 0),
	prop_z("offset.z", 0),
	prop_roll("offset.roll", 0),
	prop_pitch("offset.pitch", 0),
	prop_yaw("offset.yaw", 0)
	{
		registerProperty(prop_x);
		registerProperty(prop_y);
		registerProperty(prop_z);
		registerProperty(prop_roll);
		registerProperty(prop_pitch);
		registerProperty(prop_yaw);
}

HomogenousMatrixProvider::~HomogenousMatrixProvider()
{
}

void HomogenousMatrixProvider::prepareInterface() {
	h_generateHomogenousMatrix.setup(this, &HomogenousMatrixProvider::generateHomogenousMatrix);
	registerHandler("generateHomogenousMatrix", &h_generateHomogenousMatrix);

	registerStream("out_homogMatrix", &out_homogMatrix);

}

bool HomogenousMatrixProvider::onStart()
{
	LOG(LTRACE) << "HomogenousMatrixProvider::onStart()\n";
	return true;
}

bool HomogenousMatrixProvider::onStop()
{
	LOG(LTRACE) << "HomogenousMatrixProvider::onStop()\n";
	return true;
}

bool HomogenousMatrixProvider::onInit()
{
	LOG(LTRACE) << "HomogenousMatrixProvider::onInit()\n";


	return true;
}

bool HomogenousMatrixProvider::onFinish()
{
	LOG(LTRACE) << "HomogenousMatrixProvider::onFinish()\n";
	return true;
}

bool HomogenousMatrixProvider::onStep()
{
	LOG(LTRACE) << "HomogenousMatrixProvider::onStep()\n";
	return true;
}

void HomogenousMatrixProvider::generateHomogenousMatrix()
{
	CLOG(LTRACE) << "HomogenousMatrixProvider::generateHomogenousMatrix()\n";

	Mat_<double> rvec;
	Mat_<double> tvec;
	Mat_<double> outputMatrix(4,4);

	// Create homogenous matrix.
	Mat_<double> rotationMatrix;
	Rodrigues(rvec, rotationMatrix);

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
	outputMatrix = t + yaw * pitch * roll;

	HomogMatrix hm;

	stringstream ss;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 4; ++j) {
			hm.elements[i][j] = outputMatrix.at<double>(i,j);
			ss << hm.elements[i][j] << "  ";
		}
	}
	CLOG(LDEBUG) << "HomogMatrix:\n" << ss.str() << endl;

	out_homogMatrix.write(hm);
}

} // namespace HomogenousMatrixProvider

} // namespace Processors
