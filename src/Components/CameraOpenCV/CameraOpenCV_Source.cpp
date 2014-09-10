/*!
 * \file CameraOpenCV_Source.cpp
 * \brief Class responsible for retrieving images from camera.
 * \author mstefanc
 * \date 2010-06-10
 */

#include <iostream>

#include <boost/bind.hpp>

#include "CameraOpenCV_Source.hpp"
#include "Logger.hpp"

namespace Sources {
namespace CameraOpenCV {

CameraOpenCV_Source::CameraOpenCV_Source(const std::string & name) : Base::Component(name),
		m_device("device", boost::bind(&CameraOpenCV_Source::onDeviceCahnged, this, _1, _2), 0),
		m_width("width", 640, "combo"),
		m_height("width", 480, "combo"),
		m_triggered("triggered", false)
{
	LOG(LTRACE) << "CameraOpenCV_Source::CameraOpenCV_Source()\n";
	trig = true;

	registerProperty(m_device);

	m_width.addConstraint("320");
	m_width.addConstraint("640");
	registerProperty(m_width);

	m_height.addConstraint("240");
	m_height.addConstraint("480");
	registerProperty(m_height);

	registerProperty(m_triggered);

	valid = false;
}

CameraOpenCV_Source::~CameraOpenCV_Source() {
	LOG(LTRACE) << "CameraOpenCV_Source::~CameraOpenCV_Source()\n";
}

void CameraOpenCV_Source::prepareInterface() {
	registerHandler("onTrigger", boost::bind(&CameraOpenCV_Source::onTrigger, this));

	registerHandler("onGrabFrame", boost::bind(&CameraOpenCV_Source::onGrabFrame, this));
	addDependency("onGrabFrame", NULL);

	registerStream("out_img", &out_img);
}

bool CameraOpenCV_Source::onInit() {
	LOG(LTRACE) << "CameraOpenCV_Source::initialize()\n";

	cap.open(m_device);

	if (cap.isOpened()) {
		LOG(LTRACE) << "CameraOpenCV: device opened\n";

		if (!cap.set(CV_CAP_PROP_FRAME_WIDTH, m_width))
			LOG(LWARNING) << "Couldn't set frame width.";
		if (!cap.set(CV_CAP_PROP_FRAME_HEIGHT, m_height))
			LOG(LWARNING) << "Couldn't set frame height.";


		LOG(LINFO) << "Frame width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH);
		LOG(LINFO) << "Frame height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	}
	else {
		LOG(LWARNING) << "CameraOpenCV: device NOT opened!\n";
	}

	valid = true;
	return cap.isOpened();
}


bool CameraOpenCV_Source::onFinish() {
	LOG(LTRACE) << "CameraOpenCV_Source::finish()\n";
	cap.release();

	return !cap.isOpened();
}


void CameraOpenCV_Source::onGrabFrame() {
	if (m_change_device)
		changeDevice();

	if (!valid)
		return;

	if (m_triggered && !trig)
		return;

	trig = false;
	cap >> frame;

	if (frame.empty()) {
		return;
	}

	LOG(LTRACE) << "CameraOpenCV: got frame!\n";

	out_img.write(frame);

}

bool CameraOpenCV_Source::onStart() {
	return true;
}

bool CameraOpenCV_Source::onStop() {
	return true;
}


void CameraOpenCV_Source::onTrigger() {
	trig = true;
}

void CameraOpenCV_Source::onDeviceCahnged(int old_device, int new_device) {
	if (!running()) return;
	m_change_device = true;
}

void CameraOpenCV_Source::changeDevice() {
	m_change_device = false;
	valid = false;
	cap.release();
	cap.open(m_device);

	if (!cap.isOpened()) {
		LOG(LWARNING) << "Couldn't set new device!";
	}

	valid = true;
}

}//: namespace CameraOpenCV
}//: namespace Sources
