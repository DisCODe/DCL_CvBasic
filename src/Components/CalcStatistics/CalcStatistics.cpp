/*!
 * \file
 * \brief
 * \author Marta Lepicka
 */

#include <memory>
#include <string>

#include "CalcStatistics.hpp"
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
namespace CalcStatistics {

CalcStatistics::CalcStatistics(const std::string & name) :
		Base::Component(name) {
}

CalcStatistics::~CalcStatistics() {
}

void CalcStatistics::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_homogMatrix", &in_homogMatrix);
	registerStream("out_homogMatrix", &out_homogMatrix);

	// Register handlers
	registerHandler("calculate", boost::bind(&CalcStatistics::calculate,this));
	addDependency("calculate", &in_homogMatrix);
}

bool CalcStatistics::onInit() {
	//cumulatedHomogMatrix;
	cumulatedRvec = cv::Mat_<double>::zeros(3,1);
	cumulatedTvec = cv::Mat_<double>::zeros(3,1);
	cumulatedAxis = cv::Mat_<double>::zeros(3,1);
	cumulatedFi = 0;
	counter = 0;
	return true;
}

bool CalcStatistics::onFinish() {
	return true;
}

bool CalcStatistics::onStop() {
	return true;
}

bool CalcStatistics::onStart() {
	return true;
}

void CalcStatistics::calculate() {
	CLOG(LDEBUG)<<"in calculate()";

	Types::HomogMatrix homogMatrix;
	cv::Mat_<double> rvec;
	cv::Mat_<double> tvec;
	cv::Mat_<double> axis;
	cv::Mat_<double> rotMatrix;
	float fi;

	rotMatrix= cv::Mat_<double>::zeros(3,3);
	tvec = cv::Mat_<double>::zeros(3,1);
	axis = cv::Mat_<double>::zeros(3,1);

	// first homogMatrix on InputStream
	if(counter == 0) {
		homogMatrix = in_homogMatrix.read();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
		                rotMatrix(i,j)=homogMatrix(i, j);
			}
			tvec(i, 0) = homogMatrix(i, 3);
		}

		Rodrigues(rotMatrix, rvec);

		cumulatedHomogMatrix = homogMatrix;
		cumulatedTvec = tvec;
		cumulatedRvec = rvec;
		fi = sqrt((pow(rvec(0,0), 2) + pow(rvec(1,0), 2)+pow(rvec(2,0),2)));
		cumulatedFi = fi;
		for(int k=0;k<3;k++) {
			axis(k,0)=rvec(k,0)/fi;
		}

		cumulatedAxis = axis;
		counter=1;
		return;
	}

	homogMatrix=in_homogMatrix.read();

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
            rotMatrix(i,j)=homogMatrix(i, j);
		}
        tvec(i, 0) = homogMatrix(i, 3);
	}

	Rodrigues(rotMatrix, rvec);

	fi = sqrt((pow(rvec(0,0), 2) + pow(rvec(1,0), 2)+pow(rvec(2,0),2)));

	for(int k=0;k<3;k++) {
			axis(k,0)=rvec(k,0)/fi;
	}
	cumulatedFi += fi;
	cumulatedTvec += tvec;
	cumulatedRvec += rvec;
	cumulatedAxis += axis;

	counter++;
	avgFi = cumulatedFi/counter;
	avgAxis = cumulatedAxis/counter;
	avgRvec = avgAxis * avgFi;
	avgTvec = cumulatedTvec/counter;

	Types::HomogMatrix hm;
	cv::Mat_<double> rottMatrix;
	Rodrigues(avgRvec, rottMatrix);

	CLOG(LINFO)<<"Uśredniona macierz z "<<counter<<" macierzy \n";
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
            hm(i, j) = rottMatrix(i, j);
            CLOG(LINFO) << hm(i, j) << "  ";
		}
        hm(i, 3) = avgTvec(i, 0);
        CLOG(LINFO) << hm(i, 3) <<" \n";
	}
	out_homogMatrix.write(hm);

	CLOG(LINFO)<<"Uśredniony kąt: " << avgFi;

	float standardDeviationFi = sqrt(pow(avgFi - fi, 2));

	CLOG(LINFO)<<"Odchylenie standardowe kąta: "<<standardDeviationFi;

}

} //: namespace CalcStatistics
} //: namespace Processors
