/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "CvAdaptiveThreshold.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace CvAdaptiveThreshold {

CvAdaptiveThreshold_Processor::CvAdaptiveThreshold_Processor(const std::string & name) :
	Base::Component(name), maxValue("maxValue"), method("method", string("ADAPTIVE_THRESH_MEAN_C"), "combo"),
			thresholdType("thresholdType", string("THRESH_BINARY"), "combo"), blockSize("blockSize"), C("C")
{
	LOG(LTRACE) << "Hello CvAdaptiveThreshold_Processor\n";

	method.addConstraint("ADAPTIVE_THRESH_MEAN_C");
	method.addConstraint("ADAPTIVE_THRESH_GAUSSIAN_C");
	thresholdType.addConstraint("THRESH_BINARY");
	thresholdType.addConstraint("THRESH_BINARY_INV");

	registerProperty(maxValue);
	registerProperty(method);
	registerProperty(thresholdType);
	registerProperty(blockSize);
	registerProperty(C);

}

CvAdaptiveThreshold_Processor::~CvAdaptiveThreshold_Processor()
{
	LOG(LTRACE) << "Good bye CvAdaptiveThreshold_Processor\n";
}

bool CvAdaptiveThreshold_Processor::onInit()
{
	LOG(LTRACE) << "CvAdaptiveThreshold_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	h_onNewImage.setup(this, &CvAdaptiveThreshold_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);
	return true;
}

bool CvAdaptiveThreshold_Processor::onFinish()
{
	LOG(LTRACE) << "CvAdaptiveThreshold_Processor::finish\n";

	return true;
}

bool CvAdaptiveThreshold_Processor::onStep()
{
	LOG(LTRACE) << "CvAdaptiveThreshold_Processor::step\n";
	return true;
}

bool CvAdaptiveThreshold_Processor::onStop()
{
	return true;
}

bool CvAdaptiveThreshold_Processor::onStart()
{
	return true;
}

void CvAdaptiveThreshold_Processor::onNewImage()
{
	Mat image = in_img.read();

	int aM = 0;
	if (method() == "ADAPTIVE_THRESH_MEAN_C") {
		aM = ADAPTIVE_THRESH_MEAN_C;
	} else if (method() == "ADAPTIVE_THRESH_GAUSSIAN_C") {
		aM = ADAPTIVE_THRESH_GAUSSIAN_C;
	} else {
		LOG(LFATAL) << name() << ": CvAdaptiveThreshold_Processor: adaptiveMethod is wrong (" << method() << ")";
	}

	int tT = 0;
	if (thresholdType() == "THRESH_BINARY") {
		tT = THRESH_BINARY;
	} else if (thresholdType() == "THRESH_BINARY_INV") {
		tT = THRESH_BINARY_INV;
	} else {
		LOG(LFATAL) << name() << ": CvAdaptiveThreshold_Processor: thresholdType is wrong (" << thresholdType()
				<< ")";
	}

	adaptiveThreshold(image, thresholdedImage, maxValue, aM, tT, blockSize, C);
	out_img.write(thresholdedImage);
	newImage->raise();
}

}//: namespace CvAdaptiveThreshold
}//: namespace Processors
