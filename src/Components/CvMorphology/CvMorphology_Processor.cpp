/*!
 * \file CvMorphology_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvMorphology_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvMorphology {

CvMorphology_Processor::CvMorphology_Processor(const std::string & name) : Base::Component(name),
		iterations("iterations", 1, "range"),
        type("type", MORPH_OPEN, "combo")
{
	LOG(LTRACE) << "Hello CvMorphology_Processor\n";

	iterations.addConstraint("0");
	iterations.addConstraint("255");
	registerProperty(iterations);

	type.addConstraint("MORPH_OPEN");
	type.addConstraint("MORPH_CLOSE");
	type.addConstraint("MORPH_GRADIENT");
	type.addConstraint("MORPH_TOPHAT");
	type.addConstraint("MORPH_BLACKHAT");
	registerProperty(type);
}

CvMorphology_Processor::~CvMorphology_Processor()
{
	LOG(LTRACE) << "Good bye CvMorphology_Processor\n";
}

void CvMorphology_Processor::prepareInterface() {
	h_onNewImage.setup(this, &CvMorphology_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);
}

bool CvMorphology_Processor::onInit()
{
	LOG(LTRACE) << "CvMorphology_Processor::initialize\n";

	return true;
}

bool CvMorphology_Processor::onFinish()
{
	LOG(LTRACE) << "CvMorphology_Processor::finish\n";

	return true;
}

bool CvMorphology_Processor::onStep()
{
	LOG(LTRACE) << "CvMorphology_Processor::step\n";
	return true;
}

bool CvMorphology_Processor::onStop()
{
	return true;
}

bool CvMorphology_Processor::onStart()
{
	return true;
}

void CvMorphology_Processor::onNewImage()
{
	LOG(LTRACE) << "CvMorphology_Processor::onNewImage\n";
	try {
        cv::Mat in = in_img.read();
        cv::Mat out = in.clone();
        cv::morphologyEx(in, out, type, cv::Mat(), Point(-1, -1), iterations);
        out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "CvMorphology_Processor::onNewImage failed\n";
	}
}

}//: namespace CvMorphology
}//: namespace Processors
