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

namespace Processors {
namespace CalcObjectLocation {

CalcObjectLocation::CalcObjectLocation(const std::string & name) :
		Base::Component(name)  {

}

CalcObjectLocation::~CalcObjectLocation() {
}

void CalcObjectLocation::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_homogMatrix", &in_homogMatrix);
	registerStream("out_homogMatrix", &out_homogMatrix);


	// Register handlers
	h_calculate.setup(this, &CalcObjectLocation::calculate);
	registerHandler("calculate", &h_calculate);
	addDependency("calculate", &in_homogMatrix);

}

bool CalcObjectLocation::onInit() {
	//meanHomogMatrix = new Types::HomogMatrix();
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

void CalcObjectLocation::calculate(){

	vector<Types::HomogMatrix> readedHomogMatrix;
	Types::HomogMatrix meanHomogMatrix;
	vector<cv::Mat_<double> > rvec;
	vector<cv::Mat_<double> > tvec;
	cv::Mat_<double> rvectemp;
	cv::Mat_<double> tvectemp;
	cv::Mat_<double> rotMatrix;

	stringstream ss;
	while (!in_homogMatrix.empty()){
		readedHomogMatrix.push_back(in_homogMatrix.read());
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				rotMatrix(i,j)=(readedHomogMatrix.back().elements[i][j]);
				ss << rotMatrix(i,j) << "  ";
			}
			tvectemp(i, 0) = readedHomogMatrix.back().elements[i][3];
			ss << tvectemp(i, 0) << "\n";
		}
		Rodrigues(rotMatrix, rvectemp);
		rvec.push_back(rvectemp);
		tvec.push_back(tvectemp);
	}

	float fi_1, fi_2, fi_avg;

	//przerobic na tablice i liczyc srednia z vektora ;D
	for(int i = 0; i < rvec.size(); i++){
		//do potegi drugiej
		fi_1 = abs(sqrt(rvec.at(i)(0,0) + rvec.at(i)(1,0)+rvec.at(i)(2,0)));
		if(rvec.size()>1)
			fi_2 = abs(sqrt(rvec.at(i+1)(0,0) + rvec.at(i+1)(1,0)+rvec.at(i+1)(2,0)));
		else
			fi_2 = 0;

		fi_avg = (fi_1+fi_2)/2.00;


	}
}

} //: namespace CalcObjectLocation
} //: namespace Processors
