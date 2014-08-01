/*!
 * \file
 * \brief
 * \author Marta Lepicka
 */

#include <memory>
#include <string>

#include "CalcStatistics.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CalcStatistics {

CalcStatistics::CalcStatistics(const std::string & name) :
		Base::Component(name)  {
}

CalcStatistics::~CalcStatistics() {
}

void CalcStatistics::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_rvec", &in_rvec);
	registerStream("in_tvec", &in_tvec);
	registerStream("in_rvec_d", &in_rvec_d);
	registerStream("in_tvec_d", &in_tvec_d);
	// Register handlers

	h_calculate.setup(this, &CalcStatistics::calculate);
	registerHandler("calculate", &h_calculate);
	addDependency("calculate", &in_tvec);
	addDependency("calculate", &in_rvec);

	h_calculateDiff.setup(this, &CalcStatistics::calculateDiff);
	registerHandler("calculateDiff", &h_calculateDiff);
	addDependency("calculate", &in_tvec);
	addDependency("calculate", &in_rvec);
	addDependency("calculateDiff", &in_tvec_d);
	addDependency("calculateDiff", &in_rvec_d);
}

bool CalcStatistics::onInit() {

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

void CalcStatistics::calculate(){

	cv::Mat tvec_new=in_tvec.read();
	cv::Mat rvec_new=in_rvec.read();

	//tvec=tvec-tvec_new;

	CLOG(LINFO)<<"tvec:"<<tvec;
	CLOG(LINFO)<<"rvec"<<rvec;
}

void CalcStatistics::calculateDiff(){


}

} //: namespace CalcStatistics
} //: namespace Processors
