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
	h_calculate.setup(this, &CalcStatistics::calculate);
	registerHandler("calculate", &h_calculate);
	addDependency("calculate", &in_homogMatrix);
}

bool CalcStatistics::onInit() {
	//cumulatedHomogMatrix;
	cumulatedRvec.create(3, 1);
	cumulatedTvec.create(3, 1);
	cumulatedAxis.create(3, 3);
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
	cv::Mat_<float> rvec;
	cv::Mat_<float> tvec;
	cv::Mat_<float> axis;
	cv::Mat_<float> rotMatrix;
	float fi;

	rotMatrix.create(3,3);
	tvec.create(3,1);
	axis.create(3,1);

	//first homogMatrix on InputStream
	if(cumulatedFi == 0) {
		homogMatrix = in_homogMatrix.read();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				rotMatrix(i,j)=homogMatrix.elements[i][j];
			}
			tvec(i, 0) = homogMatrix.elements[i][3];

		}

		Rodrigues(rotMatrix, rvec);

		cumulatedHomogMatrix = homogMatrix;
		cumulatedTvec = tvec;
		cumulatedRvec = rvec;
		fi = sqrt((pow(rvec(0,0), 2) +
						pow(rvec(1,0), 2)+pow(rvec(2,0),2)));
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
			rotMatrix(i,j)=homogMatrix.elements[i][j];
		}
		tvec(i, 0) = homogMatrix.elements[i][3];
	}

	Rodrigues(rotMatrix, rvec);

	fi = sqrt((pow(rvec(0,0), 2) + pow(rvec(1,0), 2)+pow(rvec(2,0),2)));

	for(int k=0;k<3;k++) {
		for(int j=0;j<3;j++) {
			axis(k,j)=rvec(k,j)/fi;
		}
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
	cv::Mat_<float> rottMatrix;
	Rodrigues(avgRvec, rottMatrix);

	CLOG(LINFO)<<"Uśredniona macierz z "<<counter<<" macierzy \n";
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			hm.elements[i][j] = rottMatrix(i, j);
			CLOG(LINFO) << hm.elements[i][j] << "  ";
		}
		hm.elements[i][3] = avgTvec(i, 0);
		CLOG(LINFO) << hm.elements[i][3] <<" \n";
	}
	out_homogMatrix.write(hm);

	CLOG(LINFO)<<"Uśredniony kąt: " << avgFi;

	float standardDeviationFi = sqrt(pow(avgFi - fi, 2));

	CLOG(LINFO)<<"Odchylenie standardowe kąta: "<<standardDeviationFi;

}

} //: namespace CalcStatistics
} //: namespace Processors
