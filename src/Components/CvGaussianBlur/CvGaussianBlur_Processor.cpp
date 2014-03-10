/*!
 * \file CvGaussianBlur_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvGaussianBlur_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvGaussianBlur {

CvGaussianBlur_Processor::CvGaussianBlur_Processor(const std::string & name) : Base::Component(name),
		sigmax("sigma.x", 0.0),
		sigmay("sigma.y", 0.0),
		kernel_width("kernel.width", 1, "range"),
		kernel_height("kernel.height", 1, "range")
{
	// Set properties ranges and tooltips.
	sigmax.setToolTip("Gaussian kernel standard deviation in X direction");
	sigmay.setToolTip("Gaussian kernel standard deviation in Y direction");

	kernel_width.addConstraint("0");
	kernel_width.addConstraint("10");
	kernel_height.addConstraint("0");
	kernel_height.addConstraint("10");

	// Register properties.
	registerProperty(sigmax);
	registerProperty(sigmay);
	registerProperty(kernel_height);
	registerProperty(kernel_width);

	LOG(LTRACE) << "Hello CvGaussianBlur_Processor\n";
}

CvGaussianBlur_Processor::~CvGaussianBlur_Processor()
{
	LOG(LTRACE) << "Good bye CvGaussianBlur_Processor\n";
}

void CvGaussianBlur_Processor::prepareInterface() {
	CLOG(LTRACE) << "CvGaussianBlur_Processor::prepareInterface\n";

	h_onNewImage.setup(this, &CvGaussianBlur_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);
}



bool CvGaussianBlur_Processor::onInit()
{
	LOG(LTRACE) << "CvGaussianBlur_Processor::initialize\n";

	return true;
}

bool CvGaussianBlur_Processor::onFinish()
{
	LOG(LTRACE) << "CvGaussianBlur_Processor::finish\n";

	return true;
}

bool CvGaussianBlur_Processor::onStep()
{
	LOG(LTRACE) << "CvGaussianBlur_Processor::step\n";
	return true;
}

bool CvGaussianBlur_Processor::onStop()
{
	return true;
}

bool CvGaussianBlur_Processor::onStart()
{
	return true;
}

void CvGaussianBlur_Processor::onNewImage()
{
	LOG(LTRACE) << "CvGaussianBlur_Processor::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat gray;
		//cvtColor(img, gray, COLOR_BGR2GRAY);
		//cv::Mat out = img.clone();
		cv::GaussianBlur(img, img, cv::Size(kernel_width, kernel_height), sigmax, sigmay);
		out_img.write(img);
	} catch (...) {
		LOG(LERROR) << "CvGaussianBlur_Processor::onNewImage failed\n";
	}
}

}//: namespace CvGaussianBlur
}//: namespace Processors
