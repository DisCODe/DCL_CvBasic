/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvFlann.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvFlann {

CvFlann::CvFlann(const std::string & name) :
		Base::Component(name)  {

}

CvFlann::~CvFlann() {
}

void CvFlann::prepareInterface() {
	// Register handlers with their dependencies.
	h_onNewImage.setup(this, &CvFlann::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img0);
	addDependency("onNewImage", &in_img1);

	// Input and output data streams.
	registerStream("in_img0", &in_img0);
	registerStream("in_img1", &in_img1);
	registerStream("out_img", &out_img);

}

bool CvFlann::onInit() {

	return true;
}

bool CvFlann::onFinish() {
	return true;
}

bool CvFlann::onStop() {
	return true;
}

bool CvFlann::onStart() {
	return true;
}



} //: namespace CvFlann
} //: namespace Processors
