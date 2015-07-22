/*!
 * \file
 * \brief
 * \author Marta Lepicka
 */

#include <memory>
#include <string>

#include "CalcObjectLocation.hpp"
#include "Common/Logger.hpp"
#include <stdlib.h>
#include <boost/bind.hpp>
#include "Types/Objects3D/Object3D.hpp"
#include "Types/HomogMatrix.hpp"
#include "Types/CameraInfo.hpp"
#include <Common/Timer.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <sstream>
namespace Processors {
namespace CalcObjectLocation {

CalcObjectLocation::CalcObjectLocation(const std::string & name) :
		Base::Component(name) {
}

CalcObjectLocation::~CalcObjectLocation() {
}

void CalcObjectLocation::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_homogMatrix", &in_homogMatrix);
	registerStream("out_homogMatrix", &out_homogMatrix);

	// Register handlers
	registerHandler("calculate", boost::bind(&CalcObjectLocation::calculate,this));
	addDependency("calculate", &in_homogMatrix);
}

bool CalcObjectLocation::onInit() {
	return true;
}

bool CalcObjectLocation::onFinish() {
	return true;
}

bool CalcObjectLocation::onStop() {
	return true;
}

bool CalcObjectLocation::onStart() {
	return true;
}

void CalcObjectLocation::calculate() {
	CLOG(LTRACE)<<"calculate()";

	Types::HomogMatrix homogMatrix;
	vector<cv::Mat_<double> > rvec;
	vector<cv::Mat_<double> > tvec;
	vector<cv::Mat_<double> > axis;
	vector<double> fi;
	cv::Mat_<double> tvectemp;
	cv::Mat_<double> rotMatrix;
	rotMatrix = cv::Mat_<double>::zeros(3,3);
	tvectemp = cv::Mat_<double>::zeros(3,1);

	while (!in_homogMatrix.empty()) {
		cv::Mat_<double> rvectemp;
		homogMatrix=in_homogMatrix.read();

		if (homogMatrix.isIdentity()) {
			continue;
		}

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
                rotMatrix(i,j)=homogMatrix(i, j);
			}
            tvectemp(i, 0) = homogMatrix(i, 3);
		}

		Rodrigues(rotMatrix, rvectemp);
		CLOG(LDEBUG) << "rvec = " << rvectemp << " tvec = " << tvectemp;
		rvec.push_back(rvectemp);
		tvec.push_back(tvectemp);
	}

	if (rvec.size() == 1) {
		out_homogMatrix.write(homogMatrix);
		return;
	}

	double fi_sum=0.0, fi_avg;
	cv::Mat_<double> axis_sum, axis_avg;
	cv::Mat_<double> rvec_avg;
	cv::Mat_<double> tvec_avg, tvec_sum;

	axis_sum = cv::Mat_<double>::zeros(3,1);
	tvec_sum = cv::Mat_<double>::zeros(3,1);

	for(int i = 0; i < rvec.size(); i++) {
		double fitmp = sqrt((pow(rvec.at(i)(0,0), 2) + pow(rvec.at(i)(1,0), 2)+pow(rvec.at(i)(2,0),2)));
		fi.push_back(fitmp);

		fi_sum+=fitmp;
		cv::Mat_<double> axistemp;
		axistemp.create(3,1);
		for(int k=0;k<3;k++) {
				axistemp(k,0)=rvec.at(i)(k,0)/fitmp;
		}

		double tmp_axis_length = sqrt((pow(axistemp(0,0), 2) + pow(axistemp(1,0), 2)+pow(axistemp(2,0),2)));
		CLOG(LDEBUG) << "axis_tmp = "<< axistemp << " fi_tmp=" << fitmp << " axis (vector) length = "<< tmp_axis_length;

		axis.push_back(axistemp);
		axis_sum+=axistemp;
		tvec_sum+=tvec.at(i);
	}

	fi_avg = fi_sum/fi.size();
	axis_avg = axis_sum/axis.size();
	rvec_avg = axis_avg * fi_avg;
	tvec_avg = tvec_sum/tvec.size();

	Types::HomogMatrix hm;
	cv::Mat_<double> rottMatrix;
	Rodrigues(rvec_avg, rottMatrix);

	CLOG(LDEBUG) << "rvec_avg = "<< rvec_avg << "  tvec_avg=" << tvec_avg;

	// Create homogenous matrix.
	cv::Mat pose = (cv::Mat_<double>(4, 4) <<
			rottMatrix(0,0), rottMatrix(0,1), rottMatrix(0,2), tvec_avg(0),
			rottMatrix(1,0), rottMatrix(1,1), rottMatrix(1,2), tvec_avg(1),
			rottMatrix(2,0), rottMatrix(2,1), rottMatrix(2,2), tvec_avg(2),
			0, 0, 0, 1);
	CLOG(LINFO) << " HomogMatrix:\n" << pose;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
            hm(i, j) =  rottMatrix(i, j);
		}
        hm(i, 3) = tvec_avg(i, 0);
	}
	out_homogMatrix.write(hm);
}

} //: namespace CalcObjectLocation
} //: namespace Processors
