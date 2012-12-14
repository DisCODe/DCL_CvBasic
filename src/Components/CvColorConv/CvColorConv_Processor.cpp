/*!
 * \file CvColorConv_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>
#include <stdexcept>

#include "CvColorConv_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvColorConv {

CvColorConv_Processor::CvColorConv_Processor(const std::string & name) : Base::Component(name),
		conversion_type("type", CV_RGB2GRAY, "combo")
{
	// Register properties.
	registerProperty(conversion_type);

	LOG(LTRACE) << "Hello CvColorConv_Processor\n";
}

CvColorConv_Processor::~CvColorConv_Processor()
{
	LOG(LTRACE) << "Good bye CvColorConv_Processor\n";
}

void CvColorConv_Processor::prepareInterface() {
	CLOG(LTRACE) << "CvColorConv_Processor::prepareInterface\n";

	h_onNewImage.setup(this, &CvColorConv_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);
}

bool CvColorConv_Processor::onInit()
{
	LOG(LTRACE) << "CvColorConv_Processor::initialize\n";

	h_onNewImage.setup(this, &CvColorConv_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool CvColorConv_Processor::onFinish()
{
	LOG(LTRACE) << "CvColorConv_Processor::finish\n";

	return true;
}

bool CvColorConv_Processor::onStep()
{
	LOG(LTRACE) << "CvColorConv_Processor::step\n";
	return true;
}

bool CvColorConv_Processor::onStop()
{
	return true;
}

bool CvColorConv_Processor::onStart()
{
	return true;
}

void CvColorConv_Processor::onNewImage()
{
	LOG(LTRACE) << "CvColorConv_Processor::onNewImage\n";
	try {
		img = in_img.read();
		cvtColor(img, out, conversion_type);
		out_img.write(out);
		newImage->raise();
	} catch (const exception& ex) {
		LOG(LERROR) << "CvColorConv_Processor::onNewImage() failed. " << ex.what() << endl;
	}
}

}//: namespace CvColorConv
}//: namespace Processors
