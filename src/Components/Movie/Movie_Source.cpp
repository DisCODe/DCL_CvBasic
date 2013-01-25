/*!
 * \file Movie_Source.cpp
 * \brief Class responsible for retrieving images from movies.
 * \author mstefanc
 * \date 2010-05-14
 */

#include <iostream>

#include "Movie_Source.hpp"

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

namespace Sources {
namespace Movie {

Movie_Source::Movie_Source(const std::string & name) : Base::Component(name),
		filename("filename", std::string("movie.avi")),
		triggered("triggered", false)
{
	LOG(LTRACE) << "Movie_Source::Movie_Source()\n";

//	cap = NULL;
	trig = true;

	registerProperty(filename);
	registerProperty(triggered);
}

Movie_Source::~Movie_Source() {
	LOG(LTRACE) << "Movie_Source::~Movie_Source()\n";
}

void Movie_Source::prepareInterface() {
	registerStream("out_img", &out_img);

	h_onTrigger.setup(this, &Movie_Source::onTrigger);
	registerHandler("onTrigger", &h_onTrigger);

	h_onStep.setup(this, &Movie_Source::onStep);
	registerHandler("onStep", &h_onStep);
	addDependency("onStep", NULL);
}

bool Movie_Source::onInit() {
	LOG(LTRACE) << "Movie_Source::initialize()\n";

	if (!boost::filesystem::exists(std::string(filename))) {
		LOG(LERROR) << "File " << filename << " doesn't exist.";
		LOG(LNOTICE) << "Check config file or override movie filename thorugh -S switch.";
		return false;
	}

	cap.open(filename);

	if (!cap.isOpened()) {
		LOG(LERROR) << "Couldn't open movie " << filename;
		LOG(LNOTICE) << "Check if you have proper codecs installed.";
		return false;
	}

	return true;
}

bool Movie_Source::onFinish() {
	LOG(LTRACE) << "Movie_Source::finish()\n";
	cap.release();

	return true;
}

void Movie_Source::onStep() {
	if (triggered && !trig)
		return;

	trig = false;

	LOG(LTRACE) << "Movie_Source::step() start\n";
	cap >> frame;
	if (frame.empty()) {
		cap.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
		cap >> frame;
	}

	cv::Mat img = frame.clone();
	out_img.write(img);

	LOG(LTRACE) << "Movie_Source::step() end\n";
}

bool Movie_Source::onStart() {
	return true;
}

bool Movie_Source::onStop() {
	return true;
}

void Movie_Source::onTrigger() {
	trig = true;
}



}//: namespace Movie
}//: namespace Sources
