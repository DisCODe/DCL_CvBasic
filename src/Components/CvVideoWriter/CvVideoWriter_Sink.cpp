/*!
 * \file CvVideoWriter_Sink.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-15
 */

#include <memory>
#include <string>
#include <iostream>

#include "CvVideoWriter_Sink.hpp"
#include "Logger.hpp"
#include "Types/Drawable.hpp"

namespace Sinks {
namespace CvVideoWriter {

CvVideoWriter_Sink::CvVideoWriter_Sink(const std::string & name) : Base::Component(name),
		filename("filename", std::string("out.avi")),
		fourcc("fourcc", CV_FOURCC_DEFAULT),
		width("width", 640),
		height("height", 480),
		fps("fps", 25)
{
	LOG(LTRACE)<<"Hello CvVideoWriter_Sink\n";

	width.addConstraint("0");
	width.addConstraint("1280");
	registerProperty(width);

	height.addConstraint("0");
	height.addConstraint("1280");
	registerProperty(height);

	registerProperty(filename);
	registerProperty(fourcc);
	registerProperty(fps);
}

CvVideoWriter_Sink::~CvVideoWriter_Sink() {
	LOG(LTRACE)<<"Good bye CvVideoWriter_Sink\n";
}

void CvVideoWriter_Sink::prepareInterface() {
	registerHandler("onNewImage", boost::bind(&CvVideoWriter_Sink::onNewImage, this));
	addDependency("onNewImage", &in_img);

	registerStream("in_img", &in_img);
}

bool CvVideoWriter_Sink::onInit() {
	LOG(LTRACE) << "CvVideoWriter_Sink::initialize\n";

	writer.open(filename, fourcc, fps, cv::Size(width, height));

	if (writer.isOpened())
		LOG(LTRACE) << "CameraOpenCV: device opened\n";
	else
		LOG(LWARNING) << "CameraOpenCV: device NOT opened!\n";

	return writer.isOpened();
}

bool CvVideoWriter_Sink::onFinish() {
	LOG(LTRACE) << "CvVideoWriter_Sink::finish\n";

	return true;
}

bool CvVideoWriter_Sink::onStop()
{
	return true;
}

bool CvVideoWriter_Sink::onStart()
{
	return true;
}

void CvVideoWriter_Sink::onNewImage() {
	LOG(LTRACE)<<"CvVideoWriter_Sink::onNewImage\n";

	try {
		cv::Mat img = in_img.read().clone();

		writer << img;
	}
	catch(...) {
		LOG(LERROR) << "CvVideoWriter::onNewImage failed\n";
	}
}


}//: namespace CvVideoWriter
}//: namespace Sinks
